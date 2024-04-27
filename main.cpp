/*
TO DO:
- transparent backgound with win32 :/
- error handling
- optimization
*/

#define SDL_MAIN_HANDLED
#define _WIN32_WINNT 0x0500
#include "MainWindow.h"
#include <windows.h>
#include <SDL.h>
#undef main
#include <SDL_mixer.h>
#include <thread>

#include <deque>

using namespace std::chrono_literals;

std::deque<SDL_Window*> windows_list;
std::deque<double> opacity_list;

SDL_Surface* optimized_surface;

Mix_Chunk* neowSound = NULL;

// Mouse windows hook

static LRESULT CALLBACK mouseHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        if (wParam == WM_LBUTTONUP) {
            int mx = reinterpret_cast<MSLLHOOKSTRUCT*>(lParam)->pt.x;
            int my = reinterpret_cast<MSLLHOOKSTRUCT*>(lParam)->pt.y;
            SDL_Window* window = SDL_CreateWindow("",
                mx + 8,
                my + 8,
                35, 34,     // 35, 34,
                SDL_WINDOW_BORDERLESS); // SDL_WINDOW_BORDERLESS
            if (window) {
                SDL_SetWindowAlwaysOnTop(window, SDL_TRUE);
                SDL_Surface* window_surface = SDL_GetWindowSurface(window);

                SDL_BlitSurface(optimized_surface, NULL, window_surface, NULL);
                SDL_UpdateWindowSurface(window);

                // update arrays
                windows_list.push_back(window);
                opacity_list.push_back(1.0);
                Mix_PlayChannel(-1, neowSound, 0);
            }
            else {
                //std::cout << "Couldnt create subWindow." << GetLastError() << std::endl;
            }
        }
    }
    return CallNextHookEx(0, nCode, wParam, lParam);
}


//HHOOK mouseHook;
int CALLBACK WinMain(HINSTANCE   hInstance,
                    HINSTANCE   hPrevInstance,
                    LPSTR       lpCmdLine,   
                    int         nCmdShow)
{
    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) < 0) {
        return -1;
    }
    SDL_Window* window_temp = SDL_CreateWindow("",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        320, 240,
        SDL_WINDOW_BORDERLESS); // SDL_WINDOW_BORDERLESS
    if (!window_temp) {
        //std::cout << "Failed to create window\n";
        return -1;
    }

    // sound music
    Mix_Music* gMusic = NULL;

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        //std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return 1;
    }
    neowSound = Mix_LoadWAV("neow.wav");
    if (neowSound == NULL) {
        //printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        return 2;
    }

    // load image and optimise it, I guess
    SDL_Surface* window_surface{ nullptr };
    SDL_Surface* image_surface{ nullptr };
    const std::string path = "lapka.bmp";
    window_surface = SDL_GetWindowSurface(window_temp);
    image_surface = SDL_LoadBMP(path.c_str());
    if (image_surface == NULL) {
        //std::cout << "Could not load image: " << SDL_GetError() << "\n";
        return 3;
    }
    optimized_surface = SDL_ConvertSurface(image_surface, window_surface->format, 0);
    if (optimized_surface == NULL) {
        //printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        optimized_surface = window_surface;
    }
    SDL_FreeSurface(image_surface);
    SDL_DestroyWindow(window_temp);



    // creating main Window
    Window* mainWindow_ptr = new Window();


    // windows mouse hook
    HHOOK mouseHook = SetWindowsHookExA(WH_MOUSE_LL, mouseHookProc, GetModuleHandle(NULL), 0);
    if (mouseHook == NULL) {
        //std::cout << "ERROR CREATING HOOK: " << GetLastError() << std::endl;
        return 1;
    }


    // another thread for decreasing windows opacity
    auto opacityDecreasor = [mainWindow_ptr](void) {
        bool running = true;

        int elems;
        while (running) {
            if (!mainWindow_ptr->processMessages()) {
                running = false;
            }
            if (!windows_list.empty()) {
                elems = 0;
                for (int i = 0; i < windows_list.size(); ++i) {
                    opacity_list[i] -= 0.1;
                    if (opacity_list[i] <= 0) {
                        SDL_DestroyWindow(windows_list[i]);
                        ++elems;
                    }
                    else {
                        SDL_SetWindowOpacity(windows_list[i], opacity_list[i]);
                    }
                }
                for (int i = 0; i < elems; ++i) {
                    windows_list.pop_front();
                    opacity_list.pop_front();
                    SDL_CloseAudio();
                }
            }

            std::this_thread::sleep_for(100ms);
        }
        };

    std::thread thread_opacityDecreasion(opacityDecreasor);
    thread_opacityDecreasion.detach();


    // check for mouse messages and prevent program from closing
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) != 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }


    // cleaning up
    for (int i = 0; i < windows_list.size(); ++i) {
        SDL_DestroyWindow(windows_list[i]);
        opacity_list[i] = 0;
    }
    windows_list.clear();
    opacity_list.clear();


    UnhookWindowsHookEx(mouseHook);

    delete mainWindow_ptr;

    Mix_FreeChunk(neowSound);
    Mix_FreeMusic(gMusic);
    Mix_Quit();
    SDL_Quit();

    return 0;
}

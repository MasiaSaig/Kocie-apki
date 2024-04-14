#define SDL_MAIN_HANDLED
#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>


// 1000 / fps
// 1000 / 60 == 16.666f
// 1000 / 30 == 33.333f
// 1000 / 20 == 50.0f
#define FPS 50.0f

int keyPressed(int key) {
    return (GetAsyncKeyState(key) & 0x8000 != 0);
}

int main(void) //int argc, char* argv[]
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Failed to initialize the SDL2 library\n";
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("SDL2 Window",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        320, 240,
        0); // SDL_WINDOW_BORDERLESS

    if (!window){
        std::cout << "Failed to create window\n";
        return -1;
    }

    // Enable capturing of a mouse, outside window.
    //SDL_CaptureMouse(SDL_TRUE);
    bool running = true;
    SDL_Surface* window_surface{ nullptr };
    SDL_Surface* image_surface{ nullptr };

    // load image
    const std::string path = ".\\obraz.png";
    window_surface = SDL_GetWindowSurface(window);
    image_surface = IMG_Load(path.c_str());
    if (image_surface == NULL) {
        std::cout << "Could not load image: " << SDL_GetError() << "\n";
        return -1;
    }
    SDL_Surface* optimized_surface = SDL_ConvertSurface(image_surface, window_surface->format, 0);
    if (optimized_surface == NULL){
        printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
    }
    SDL_FreeSurface(image_surface);
    SDL_BlitSurface(optimized_surface, NULL, window_surface, NULL);
    SDL_UpdateWindowSurface(window);

    // program loop
    int mx, my;
    SDL_Event e;
    Uint64 start=0, elapsedTime = 999999, start_frame_limit, elapsed_time_limit;

//background
    //SDL_Renderer* renderer;
    //renderer = SDL_CreateRenderer(window, -1, 0);
    //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

#define SEC 3000
    while (running) {
        start_frame_limit = SDL_GetPerformanceCounter();
        if (elapsedTime >= SEC) {
            start = SDL_GetTicks();
        }
        elapsedTime = SDL_GetTicks() - start;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT:
                running = false;
                SDL_Quit();
                break;
            //case SDL_MOUSEBUTTONDOWN:
            //    std::cout << ">";
            //    if (e.button.button == SDL_BUTTON_LEFT) {
            //        // onClick()
            //        SDL_GetGlobalMouseState(&mx, &my);
            //        std::cout << "x: " << mx << " y: " << my << std::endl;
            //        SDL_SetWindowPosition(window, mx, my);
            //    } break;
            }
            //SDL_RenderClear(renderer);
            //SDL_RenderPresent(renderer);
        }
        if (keyPressed(VK_LBUTTON)) {
            SDL_GetGlobalMouseState(&mx, &my);
            std::cout << "x: " << mx << " y: " << my << std::endl;
            SDL_SetWindowPosition(window, mx, my);
        }
        if (keyPressed(VK_RBUTTON)) {
            printf("%s\n", "Click Derecho");
        }
            
        
        SDL_SetWindowOpacity(window, (static_cast<float>(elapsedTime) / static_cast<float>(SEC) ));
        
        
        /*if ((elapsedTime >= 100) && (elapsedTime <= 110)) {
            std::cout << "\nt1: " << static_cast<float>(elapsedTime) / static_cast<float>(SEC);
        }
        if ((elapsedTime >= 500) && (elapsedTime <= 510)) {
            std::cout << "\nt2: " << static_cast<float>(elapsedTime) / static_cast<float>(SEC);
        }
        if ((elapsedTime >= 1300) && (elapsedTime <= 1310)) {
            std::cout << "\nt3: " << static_cast<float>(elapsedTime) / static_cast<float>(SEC);
        }
        if ((elapsedTime == 2200) && (elapsedTime <= 2210)) {
            std::cout << "\nt4: " << static_cast<float>(elapsedTime) / static_cast<float>(SEC);
        }*/

        elapsed_time_limit = (SDL_GetPerformanceCounter() - start_frame_limit) / SDL_GetPerformanceFrequency();
        if (elapsed_time_limit < FPS) {
            SDL_Delay(floor(FPS - (elapsed_time_limit * 1000.0f)));   // delay in miliseconds
        }
    }

    return 0;
}

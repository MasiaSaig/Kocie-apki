## Kocielapki
A short and a silly program, made for a friend. It simply shows a cat's paw and plays meowing sound, every time a user presses left mouse button.

The program, uses:
- Win32 api for creating main window, catching and handling global mouse left click event.
- SDL2 for creating a borderless window with a displayed paw on it.
- SDL2 Mixer for managing and playing sound.

#### Send to a friend ####
If you would like to send it to someone else, you need to (compile it of course) keep following files in the same folder:
- SDL2.dll
- SDL2_mixer.dll
- Kocielapki.exe (compiled program)
- neow.wav (or any other sound effect with the same name and file extension .wav)
- lapka.bmp (or any other image with resolution 35x34 pixels, file with the same name and file extension .bmp)

#### Simple explanation of how it works ####
It uses a windows mouse hook to listen to global mouse events. If the user pressed left mouse button, it creates a borderless window with an image using SDL2 and plays and audio using SDL_mixer.

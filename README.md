## Kocielapki
A short and a silly program, made for a friend. It simply shows a cat's paw and plays meowing sound, every time a user presses left mouse button.

The program, uses:
- Win32 api for creating main window, catching and handling global mouse left click event.
- SDL2 for creating a borderless window with a displayed paw on it.
- SDL2 Mixer for managing and playing sound.

#### Simple explanation of how it works ####
It uses a windows mouse hook to listen to global mouse events. If the user pressed left mouse button, it creates a borderless window with an image using SDL2 and plays and audio using SDL_mixer.

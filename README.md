## Kocielapki
A short program, made for a friend. It simply shows a cat's paw and plays meowing sound, every time a user pressed left mouse button.

The program, uses:
- Win32 api for creating main window, catching and handling global mouse left click event.
- SDL2 for creating a borderless window with a displayed paw on it.
- SDL2 Mixer for managing and playing sound.

If you would like to send it to someone else, you need to (compile it of course) and keep following files in the same folder:
- SDL2.dll
- SDL2_mixer.dll
- Kocielapki.exe (compiled program)
- neow.wav (or any other sound effect with the same name and file extension .wav)
- lapka.bmp (or any other image with resolution 35x34 pixels, file with the same name and file extension .bmp)

When compiling in Visual Studio, it's important to go to project's properties -> Configuration Properties -> C/C++ -> Code Generation and set Runtime Library to "Multi-threaded(/MT)" option.

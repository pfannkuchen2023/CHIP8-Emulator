# CHIP-8 Emulator Attempt
This is my attempt at a basic CHIP-8 Emulator in C++. It is also my first proper C++ project!

## Features
- Pause/Unpause
- Reset state

## Controls
```
Keypad                   Keyboard
+-+-+-+-+                +-+-+-+-+
|1|2|3|C|                |1|2|3|4|
+-+-+-+-+                +-+-+-+-+
|4|5|6|D|                |Q|W|E|R|
+-+-+-+-+       =>       +-+-+-+-+
|7|8|9|E|                |A|S|D|F|
+-+-+-+-+                +-+-+-+-+
|A|0|B|F|                |Z|X|C|V|
+-+-+-+-+                +-+-+-+-+
```
(From multigesture.net)
### Hotkeys
- Space to pause/unpause
- Return to reset emulator
- ESC to quit
  
## Dependencies
Requires:
- SDL2
- CMAKE

## Known Issues
- Some games like Brick have flickering player

## TODO
- Add ability to play audio
- Add ability to load games from within the programme

## Resources
- Thanks to Austin Morlan website for guiding me through the project https://austinmorlan.com/posts/chip8_emulator/#the-main-loop
- Thanks to desertedfreeman for post on reddit that inspired this project! Check out their attempt at CHIP8 here: https://github.com/desertedman/CHIP-8
- https://multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/
- https://en.wikipedia.org/wiki/CHIP-8
- https://github.com/dmatlack/chip8/tree/master/roms/games

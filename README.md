# c2048

c2048 is a cross platform terminal-based C89 implementation of Gabriele Cirulli's
[2048](https://github.com/gabrielecirulli/2048), by [DaCukiMonsta](https://github.com/DaCukiMonsta).

# How to Play
Use the arrow keys or WASD to move the board. Every time two of the same number collide,
they merge into their sum! The goal is to get to the 2048 tile before you fill up the board
and can no longer move, in which case the game is over.

# Compilation
Pre-built binaries are available to download for Windows, see the releases tab on GitHub.

## Windows
Open `c2048.sln` in Visual Studio (2010 or above) with Visual C support, and press Build.

## Linux, macOS, POSIX etc...
Run `make`. If rebuilding, run `make clean` first to remove old build files.

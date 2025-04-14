# 🎮 Arcade

A retro gaming platform that lets you switch between different graphical libraries and games dynamically.

## 📋 Overview

Arcade is a gaming platform that implements multiple display libraries and games, allowing users to switch between them at runtime. The program is built with a modular architecture using dynamic libraries.

## 🚀 Features

- Multiple display libraries:
  - SDL2
  - SFML
  - NCurses
  - LibCaca
- Multiple display games:
  - Snake
  - Minesweeper
  - Pacman
- Dynamic library switching
- Score tracking system
- User profiles
- Menu interface

## 🛠️ Building the Project

### Prerequisites

- C++ Compiler (GCC/Clang) with C++20 support
- CMake (version 3.15 or higher)
- Required development libraries:
  ```bash
  # Arch Linux
  sudo pacman -S sdl2 sdl2_ttf sfml ncurses libcaca glfw-x11 glew

  # Ubuntu/Debian
  sudo apt install libsdl2-dev libsdl2-ttf-dev libsfml-dev libncurses-dev libcaca-dev libglfw3-dev libglew-dev
  ```

### Build Instructions

1. Clone the repository
   ```bash
   git clone <repository-url>
   cd arcade
   ```

2. Build the project
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```

## 🎯 Usage

Run the arcade executable with a graphics library as argument:
```bash
./arcade ./lib/arcade_graphical_librairie.so
```

### Controls

- `Z/↑`: Move up
- `S/↓`: Move down
- `Q/←`: Move left
- `D/→`: Move right
- `L`: Switch graphics library
- `G`: Switch game
- `M`: Return to menu
- `R`: Restart game
- `Escape`: Quit

## 📝 Documentation

This module provides functionality for managing games and graphic libraries.

Documentation on how to add a game or graphic library is available in the `./doc` directory.

## 🤝 Collaboration

We collaborated with another group to design and implement the interfaces for the game and display modules. This joint effort ensured compatibility and modularity between the components, allowing seamless integration of new games and graphical libraries.


- maxime.keller@epitech.eu
- thibaud.wimmer@epitech.eu

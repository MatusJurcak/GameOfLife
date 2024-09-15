# Game of Life
Implementation of Conway's Game of Life (https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life) in C++ using Raylib library.

## How to play
At the "Rules" window you may change the values for underpopulation, overpopulation and reproduction.
The official Conway's Game of Life rules states that:
* Any live cell with fewer than two live neighbours dies, as if by underpopulation.
* Any live cell with two or three live neighbours lives on to the next generation.
* Any live cell with more than three live neighbours dies, as if by overpopulation.
* Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.

These values can be changed by the user, by clicking on the right input box and then typing the number that user wants.
After clicking on the `PLAY` button, user is redirected to the window with the grid.

All the cells on the grid are interactive, by clicking a cell you can toggle if it is alive or dead.

The user have 4 buttons on the top:
* `RESTART` - this button sets every cell's value to `dead`
* `START/PAUSE` - this button starts and pauses the "time" for the cells
* `RANDOMIZE` - this button sets a random value to each cell. Every cell has 50% chance of being alive and 50% chance of being dead.
* `RULES` - this button pauses the time and redirects to the rules window 

## How to build the app (Linux/macOS)
**Special thanks to [@siddharthroy12](https://github.com/siddharthroy12) for making this template**
1. Clone the repository using `git clone https://github.com/MatusJurcak/GameOfLife`
1. Move to the GameOfLife directory
   `cd GameOfLife`
1. Make a build folder and cd to it with `mkdir build && cd build`
1. Setup cmake `cmake .. -DCMAKE_BUILD_TYPE=Release`
1. Run `make` to compile the project
1. Make the binary executable `chmod +x GameOfLife`
1. Run the binary with `./GameOfLife`

# Code sum-up

## Game.cpp
The main algorithm just takes the 2D grid and for each cell it checks the number of alive neighbours and with the given rules it then sets the value to each cell.
All of this is handled in the Game.cpp file as well as all the drawing.


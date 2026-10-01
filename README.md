# Advanced Algorithms : A\* Puzzle Solver

A command line puzzle solver using the A\* search algorithm.

The program allows users to create puzzles, generate random solvable puzzles, compare heuristics and display search statistics.

## How it Works

The solver uses the A\* search algorithm, where for each puzzle state it calculates:

f(n) = g(n) + h(n)

where:

- g(n) is the cost of reaching the current state from the starting state.
- h(n) is the estimated cost from the current state to the goal.
- f(n) is the total estimated cost used to prioritise states for exploration.

Each move of the blank space has a cost of 1.

## Running the Program

### Requirements

- C++20-compatible compiler
- g++

### Compile

From the project root:

g++ -std=c++20 src/main.cpp src/astarAlgorithm/astarAlgorithm.cpp -o puzzle_solver.exe

### Execute

.\puzzle_solver.exe

## Running Tests

### Compile

g++ -std=c++20 tests/AStarTests.cpp src/astarAlgorithm/astarAlgorithm.cpp -o tests.exe

### Execute

.\tests.exe

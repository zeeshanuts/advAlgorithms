#include "../src/astarAlgorithm/astarAlgorithm.hpp"

#include <array>
#include <cassert>
#include <iostream>

void testAlreadySolved() {
    AStar solver;

    PuzzleState puzzle{
        {1, 2, 3,
         4, 5, 6,
         7, 8, 0},
        0,
        0
    };

    SolveResult result = solver.solve(puzzle);

    assert(!result.path.empty());
    assert(result.path.size() == 1);
    assert(result.path.back() == puzzle);

    assert(result.stats.statesExpanded > 0);
}

void testOneMove() {
    AStar solver;

    PuzzleState puzzle{
        {1, 2, 3,
         4, 5, 6,
         7, 0, 8},
        0,
        0
    };

    SolveResult result = solver.solve(puzzle);

    std::array<int, 9> expected = {
        1, 2, 3,
        4, 5, 6,
        7, 8, 0
    };

    assert(!result.path.empty());
    assert(result.path.size() == 2);
    assert(result.path.back().board == expected);

    assert(result.stats.statesExpanded > 0);
    assert(result.stats.statesGenerated > 0);
}

void testTwoMoves() {
    AStar solver;

    PuzzleState puzzle{
        {1, 2, 3,
         4, 0, 6,
         7, 5, 8},
        0,
        0
    };

    SolveResult result = solver.solve(puzzle);

    std::array<int, 9> expected = {
        1, 2, 3,
        4, 5, 6,
        7, 8, 0
    };

    assert(!result.path.empty());
    assert(result.path.size() == 3);
    assert(result.path.back().board == expected);

    assert(result.stats.statesExpanded > 0);
    assert(result.stats.statesGenerated > 0);
}

void testManhattanHeuristic() {
    AStar solver(Heuristic::Manhattan);

    PuzzleState puzzle{
        {1, 2, 3,
         4, 0, 6,
         7, 5, 8},
        0,
        0
    };

    SolveResult result = solver.solve(puzzle);

    std::array<int, 9> expected = {
        1, 2, 3,
        4, 5, 6,
        7, 8, 0
    };

    assert(!result.path.empty());
    assert(result.path.size() == 3);
    assert(result.path.back().board == expected);
}

void testMisplacedTilesHeuristic() {
    AStar solver(Heuristic::MisplacedTiles);

    PuzzleState puzzle{
        {1, 2, 3,
         4, 0, 6,
         7, 5, 8},
        0,
        0
    };

    SolveResult result = solver.solve(puzzle);

    std::array<int, 9> expected = {
        1, 2, 3,
        4, 5, 6,
        7, 8, 0
    };

    assert(!result.path.empty());
    assert(result.path.size() == 3);
    assert(result.path.back().board == expected);
}

void testHeuristicsProduceSameSolutionLength() {
    PuzzleState puzzle{
        {2, 3, 6,
         1, 5, 8,
         4, 7, 0},
        0,
        0
    };

    AStar manhattanSolver(Heuristic::Manhattan);
    AStar misplacedSolver(Heuristic::MisplacedTiles);

    SolveResult manhattanResult =
        manhattanSolver.solve(puzzle);

    SolveResult misplacedResult =
        misplacedSolver.solve(puzzle);

    assert(!manhattanResult.path.empty());
    assert(!misplacedResult.path.empty());

    assert(
        manhattanResult.path.size() ==
        misplacedResult.path.size()
    );
}

int main() {
    testAlreadySolved();
    testOneMove();
    testTwoMoves();

    testManhattanHeuristic();
    testMisplacedTilesHeuristic();
    testHeuristicsProduceSameSolutionLength();

    std::cout << "All tests passed.\n";

    return 0;
}
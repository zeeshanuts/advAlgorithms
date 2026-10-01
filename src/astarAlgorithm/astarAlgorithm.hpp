#pragma once

#include <array>
#include <vector>

struct PuzzleState {
    std::array<int, 9> board;

    /* g(n) Cost from the starting state */
    int gCost;

    /* h(n) Estimated cost to the goal */
    int hCost;

    /* f(n) = g(n) + h(n) */
    int fCost() const {
        return gCost + hCost;
    }

    bool operator==(const PuzzleState& other) const {
        return board == other.board;
    }
};

struct SearchStats {
    int statesExpanded = 0;
    int statesGenerated = 0;
    int maxOpenSetSize = 0;
};

struct SolveResult {
    std::vector<PuzzleState> path;
    SearchStats stats;
};

enum class Heuristic {
    Manhattan,
    MisplacedTiles
};

class AStar {
public:
    explicit AStar(Heuristic heuristic = Heuristic::Manhattan);

    SolveResult solve(const PuzzleState& start);

private:
    Heuristic heuristic;

    int calculateHeuristic(const PuzzleState& state);
    int manhattanDistance(const PuzzleState& state);
    int misplacedTiles(const PuzzleState& state);

    std::vector<PuzzleState> getNeighbours(
        const PuzzleState& state
    );

    bool isGoal(const PuzzleState& state);
};
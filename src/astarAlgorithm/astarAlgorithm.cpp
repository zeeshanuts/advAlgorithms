#include "astarAlgorithm.hpp"

#include <algorithm>
#include <cmath>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace {
    const std::array<int, 9> GOAL = {
        1, 2, 3,
        4, 5, 6,
        7, 8, 0
    };

    std::string boardKey(const std::array<int, 9>& board) {
        std::string key;

        for (int value : board) {
            key += std::to_string(value);
        }

        return key;
    }

    struct ComparePuzzleStates {
        bool operator()(
            const PuzzleState& a,
            const PuzzleState& b
        ) const {
            return a.fCost() > b.fCost();
        }
    };
}

AStar::AStar(Heuristic heuristic)
    : heuristic(heuristic) {}

bool AStar::isGoal(const PuzzleState& state) {
    return state.board == GOAL;
}

int AStar::manhattanDistance(const PuzzleState& state) {
    int distance = 0;

    for (int i = 0; i < 9; ++i) {
        int tile = state.board[i];

        if (tile == 0) {
            continue;
        }

        int currentRow = i / 3;
        int currentCol = i % 3;

        int goalIndex = tile - 1;
        int goalRow = goalIndex / 3;
        int goalCol = goalIndex % 3;

        distance += std::abs(currentRow - goalRow);
        distance += std::abs(currentCol - goalCol);
    }

    return distance;
}

int AStar::misplacedTiles(const PuzzleState& state) {
    int misplaced = 0;

    for (int i = 0; i < 9; ++i) {
        if (state.board[i] == 0) {
            continue;
        }

        if (state.board[i] != GOAL[i]) {
            ++misplaced;
        }
    }

    return misplaced;
}

int AStar::calculateHeuristic(const PuzzleState& state) {
    if (heuristic == Heuristic::Manhattan) {
        return manhattanDistance(state);
    }

    return misplacedTiles(state);
}

std::vector<PuzzleState> AStar::getNeighbours(
    const PuzzleState& state
) {
    std::vector<PuzzleState> neighbours;

    int blank = 0;

    for (int i = 0; i < 9; ++i) {
        if (state.board[i] == 0) {
            blank = i;
            break;
        }
    }

    int row = blank / 3;
    int col = blank % 3;

    const std::array<std::pair<int, int>, 4> directions = {{
        {-1, 0},
        {1, 0},
        {0, -1},
        {0, 1}
    }};

    for (auto [dr, dc] : directions) {
        int newRow = row + dr;
        int newCol = col + dc;

        if (newRow < 0 || newRow >= 3 ||
            newCol < 0 || newCol >= 3) {
            continue;
        }

        int newIndex = newRow * 3 + newCol;

        PuzzleState neighbour = state;

        std::swap(
            neighbour.board[blank],
            neighbour.board[newIndex]
        );

        neighbour.gCost = state.gCost + 1;
        neighbour.hCost = calculateHeuristic(neighbour);

        neighbours.push_back(neighbour);
    }

    return neighbours;
}

SolveResult AStar::solve(
    const PuzzleState& start
) {
    SearchStats stats;

    std::priority_queue<
        PuzzleState,
        std::vector<PuzzleState>,
        ComparePuzzleStates
    > openSet;

    std::unordered_set<std::string> visited;

    std::unordered_map<std::string, std::string> parent;

    std::unordered_map<std::string, PuzzleState> states;

    PuzzleState initial = start;
    initial.gCost = 0;
    initial.hCost = calculateHeuristic(initial);

    std::string startKey = boardKey(initial.board);

    openSet.push(initial);

    stats.maxOpenSetSize =
        static_cast<int>(openSet.size());

    states[startKey] = initial;

    while (!openSet.empty()) {
        PuzzleState current = openSet.top();
        openSet.pop();

        std::string currentKey = boardKey(current.board);

        if (visited.contains(currentKey)) {
            continue;
        }

        visited.insert(currentKey);

        stats.statesExpanded++;

        if (isGoal(current)) {
            std::vector<PuzzleState> path;

            std::string key = currentKey;

            while (true) {
                path.push_back(states[key]);

                if (key == startKey) {
                    break;
                }

                key = parent[key];
            }

            std::reverse(path.begin(), path.end());

            return {path, stats};
        }

        for (PuzzleState neighbour : getNeighbours(current)) {
            stats.statesGenerated++;

            std::string neighbourKey =
                boardKey(neighbour.board);

            if (visited.contains(neighbourKey)) {
                continue;
            }

            if (!states.contains(neighbourKey) ||
                neighbour.gCost < states[neighbourKey].gCost) {

                states[neighbourKey] = neighbour;
                parent[neighbourKey] = currentKey;

                openSet.push(neighbour);

                stats.maxOpenSetSize =
                    std::max(
                        stats.maxOpenSetSize,
                        static_cast<int>(openSet.size())
                    );
            }
        }
    }

    return {{}, stats};
}
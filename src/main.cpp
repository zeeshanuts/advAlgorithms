#include "astarAlgorithm/astarAlgorithm.hpp"

#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

void printBoard(const PuzzleState& state) {
    for (int i = 0; i < 9; ++i) {
        if (state.board[i] == 0) {
            std::cout << "  ";
        } else {
            std::cout << state.board[i] << ' ';
        }

        if (i % 3 == 2) {
            std::cout << '\n';
        }
    }

    std::cout << '\n';
}

bool isValidPuzzle(const PuzzleState& state) {
    bool seen[9] = {};

    for (int value : state.board) {
        if (value < 0 || value > 8) {
            return false;
        }

        if (seen[value]) {
            return false;
        }

        seen[value] = true;
    }

    return true;
}

bool isSolvable(const PuzzleState& state) {
    int inversions = 0;

    for (int i = 0; i < 9; ++i) {
        if (state.board[i] == 0) {
            continue;
        }

        for (int j = i + 1; j < 9; ++j) {
            if (state.board[j] == 0) {
                continue;
            }

            if (state.board[i] > state.board[j]) {
                ++inversions;
            }
        }
    }

    return inversions % 2 == 0;
}

PuzzleState generateRandomPuzzle(int moves = 30) {
    PuzzleState puzzle{
        {1, 2, 3,
         4, 5, 6,
         7, 8, 0},
        0,
        0
    };

    std::random_device rd;
    std::mt19937 generator(rd());

    for (int move = 0; move < moves; ++move) {
        int blank = 0;

        for (int i = 0; i < 9; ++i) {
            if (puzzle.board[i] == 0) {
                blank = i;
                break;
            }
        }

        int row = blank / 3;
        int col = blank % 3;

        std::vector<int> possibleMoves;

        if (row > 0) {
            possibleMoves.push_back(blank - 3);
        }

        if (row < 2) {
            possibleMoves.push_back(blank + 3);
        }

        if (col > 0) {
            possibleMoves.push_back(blank - 1);
        }

        if (col < 2) {
            possibleMoves.push_back(blank + 1);
        }

        std::uniform_int_distribution<int> distribution(
            0,
            static_cast<int>(possibleMoves.size()) - 1
        );

        int newPosition =
            possibleMoves[distribution(generator)];

        std::swap(
            puzzle.board[blank],
            puzzle.board[newPosition]
        );
    }

    return puzzle;
}

void solvePuzzle(
    const PuzzleState& start,
    Heuristic heuristic
) {
    AStar solver(heuristic);

    SolveResult result = solver.solve(start);

    if (result.path.empty()) {
        std::cout << "\nNo solution found.\n";
        return;
    }

    std::cout << "\nSolution found in "
              << result.path.size() - 1
              << " moves.\n\n";

    for (std::size_t i = 0; i < result.path.size(); ++i) {
        std::cout << "Step " << i << ":\n";
        printBoard(result.path[i]);
    }

    std::cout << "Search statistics:\n";

    std::cout << "States expanded: "
              << result.stats.statesExpanded
              << '\n';

    std::cout << "States generated: "
              << result.stats.statesGenerated
              << '\n';

    std::cout << "Maximum open set size: "
              << result.stats.maxOpenSetSize
              << "\n\n";
}

void compareHeuristics(const PuzzleState& start) {
    AStar manhattanSolver(Heuristic::Manhattan);
    AStar misplacedSolver(Heuristic::MisplacedTiles);

    SolveResult manhattanResult =
        manhattanSolver.solve(start);

    SolveResult misplacedResult =
        misplacedSolver.solve(start);

    if (manhattanResult.path.empty() ||
        misplacedResult.path.empty()) {
        std::cout << "\nNo solution found.\n";
        return;
    }

    std::cout
        << "\n========== Heuristic Comparison ==========\n\n";

    std::cout
        << "Metric"
        << "\t\tManhattan"
        << "\tMisplaced Tiles\n";

    std::cout
        << "------------------------------------------\n";

    std::cout
        << "Solution moves"
        << "\t"
        << manhattanResult.path.size() - 1
        << "\t\t"
        << misplacedResult.path.size() - 1
        << '\n';

    std::cout
        << "States expanded"
        << "\t"
        << manhattanResult.stats.statesExpanded
        << "\t\t"
        << misplacedResult.stats.statesExpanded
        << '\n';

    std::cout
        << "States generated"
        << "\t"
        << manhattanResult.stats.statesGenerated
        << "\t\t"
        << misplacedResult.stats.statesGenerated
        << '\n';

    std::cout
        << "Max open set"
        << "\t"
        << manhattanResult.stats.maxOpenSetSize
        << "\t\t"
        << misplacedResult.stats.maxOpenSetSize
        << "\n\n";
}

bool readPuzzle(PuzzleState& puzzle) {
    std::cout << "\nEnter the 9 numbers separated by spaces.\n";
    std::cout << "Use 0 for the blank space.\n";
    std::cout << "Example: 1 2 3 4 5 6 7 0 8\n\n";
    std::cout << "Puzzle: ";

    std::string input;
    std::getline(std::cin >> std::ws, input);

    std::stringstream stream(input);

    std::vector<int> values;
    int value;

    while (stream >> value) {
        values.push_back(value);
    }

    if (!stream.eof()) {
        std::cout
            << "\nInvalid input. Please enter only numbers.\n\n";
        return false;
    }

    if (values.size() != 9) {
        std::cout
            << "\nInvalid input. You must enter exactly 9 numbers.\n\n";
        return false;
    }

    for (int i = 0; i < 9; ++i) {
        puzzle.board[i] = values[i];
    }

    if (!isValidPuzzle(puzzle)) {
        std::cout << "\nInvalid puzzle configuration.\n";
        std::cout
            << "The puzzle must contain each number from 0 to 8 exactly once.\n\n";
        return false;
    }

    if (!isSolvable(puzzle)) {
        std::cout << "\nThis puzzle is not solvable.\n\n";
        return false;
    }

    return true;
}

bool readMenuChoice(int& choice) {
    std::string input;

    std::getline(std::cin >> std::ws, input);

    std::stringstream stream(input);

    if (!(stream >> choice)) {
        return false;
    }

    std::string extra;

    if (stream >> extra) {
        return false;
    }

    return true;
}

bool readHeuristic(Heuristic& heuristic) {
    std::cout << "\nSelect heuristic:\n";
    std::cout << "1. Manhattan Distance\n";
    std::cout << "2. Misplaced Tiles\n";
    std::cout << "\nSelect an option: ";

    std::string input;
    std::getline(std::cin >> std::ws, input);

    std::stringstream stream(input);

    int choice;

    if (!(stream >> choice)) {
        std::cout
            << "\nInvalid input. Please enter 1 or 2.\n\n";
        return false;
    }

    std::string extra;

    if (stream >> extra) {
        std::cout
            << "\nInvalid input. Please enter 1 or 2.\n\n";
        return false;
    }

    if (choice == 1) {
        heuristic = Heuristic::Manhattan;
        return true;
    }

    if (choice == 2) {
        heuristic = Heuristic::MisplacedTiles;
        return true;
    }

    std::cout
        << "\nInvalid option. Please select 1 or 2.\n\n";

    return false;
}

int main() {
    while (true) {
        std::cout << "---------------------------------\n";
        std::cout << "       A* 8-Puzzle Solver\n";
        std::cout << "---------------------------------\n\n";

        std::cout << "1. Solve a puzzle\n";
        std::cout << "2. Generate a random puzzle\n";
        std::cout << "3. Compare heuristics\n";
        std::cout << "4. Exit\n\n";

        std::cout << "Select an option: ";

        int choice;

        if (!readMenuChoice(choice)) {
            std::cout
                << "\nInvalid input. Please enter 1, 2, 3, or 4.\n\n";
            continue;
        }

        if (choice == 1) {
            PuzzleState puzzle{};

            if (!readPuzzle(puzzle)) {
                continue;
            }

            Heuristic heuristic;

            if (!readHeuristic(heuristic)) {
                continue;
            }

            std::cout << "\nStarting puzzle:\n";
            printBoard(puzzle);

            solvePuzzle(puzzle, heuristic);
        }
        else if (choice == 2) {
            PuzzleState puzzle = generateRandomPuzzle();

            std::cout << "\nGenerated puzzle:\n\n";
            printBoard(puzzle);

            Heuristic heuristic;

            if (!readHeuristic(heuristic)) {
                continue;
            }

            std::cout << "Solving...\n";

            solvePuzzle(puzzle, heuristic);
        }
        else if (choice == 3) {
            PuzzleState puzzle{};

            if (!readPuzzle(puzzle)) {
                continue;
            }

            std::cout << "\nPuzzle to compare:\n";
            printBoard(puzzle);

            compareHeuristics(puzzle);
        }
        else if (choice == 4) {
            break;
        }
        else {
            std::cout
                << "\nInvalid option. Please select 1, 2, 3, or 4.\n\n";
        }
    }

    return 0;
}
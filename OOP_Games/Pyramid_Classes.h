#ifndef PYRAMID_CLASSES_H
#define PYRAMID_CLASSES_H

#include "BoardGame_Classes.h"
#include <vector>
#include <string>
#include <algorithm>
#include <limits>

using namespace std;

class Pyramid_Board : public Board<char> {
public:
    Pyramid_Board();

    // Core game logic overrides
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;


    bool is_valid_position(int row, int col);

    // Return all empty valid positions as possible moves
    vector<pair<int, int>> get_valid_moves();

    // --- Minimax helpers for AI ---

    // Check win for a specific symbol (used directly by minimax)
    bool check_win_symbol(char symbol);

    // Compute the best (row, col) move for the AI using minimax
    pair<int, int> get_best_move(char aiSymbol);

    // Minimax search:
    //  - depth: current search depth
    //  - isMaximizing: true -> AI turn, false -> human turn
    int minimax(int depth, bool isMaximizing,
                char aiSymbol, char humanSymbol);
};

// -----------------------------------------------------------------------------
// Pyramid_UI

class Pyramid_UI : public UI<char> {
private:
    Pyramid_Board* board_ptr;

public:
    explicit Pyramid_UI(Pyramid_Board* board);

    // Create a new player instance for this UI
    Player<char>* create_player(string& name,
                                char symbol,
                                PlayerType type) override;

    // Get next move (either from human input or AI using minimax)
    Move<char>* get_move(Player<char>* player) override;
};

#endif // PYRAMID_CLASSES_H

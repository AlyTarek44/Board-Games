#ifndef FOURINROW_CLASSES_H
#define FOURINROW_CLASSES_H

#include "BoardGame_Classes.h"
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class FourInRow_Board : public Board<char> {
private:
    char blank_symbol = '.';

    // Core game logic helpers
    bool check_direction(int row, int col, int delta_row, int delta_col, char symbol);
    bool check_win_condition(int row, int col, char symbol);
    int get_next_empty_row(int col);

    // AI Helper Functions
    // Scores a "window" of 4 cells (e.g., X X . X)
    int evaluate_window(const vector<char>& window, char piece);
    // Evaluates the entire board state
    int score_position(char piece);
    // The recursive Minimax algorithm with Alpha-Beta Pruning
    int minimax(int depth, int alpha, int beta, bool maximizingPlayer, char ai_symbol);

public:
    FourInRow_Board();

    // Base class overrides
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

    // Helper for UI and AI
    bool is_valid_move(int col);
    vector<int> get_valid_moves();

    // AI Entry Point
    // Returns the best column to play
    int get_best_move(char ai_symbol);
};


class FourInRow_UI : public UI<char> {
private:
    FourInRow_Board* board_ptr;

public:
    FourInRow_UI(FourInRow_Board* board);
    ~FourInRow_UI() {}

    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* player) override;
    Player<char>** setup_players() override;
};

#endif
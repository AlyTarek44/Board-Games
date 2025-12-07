#ifndef FOURBYFOUR_CLASSES_H
#define FOURBYFOUR_CLASSES_H

#include "BoardGame_Classes.h"
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

using namespace std;

// New move class for 4x4 game
class FourByFour_Move : public Move<char> {
    int from_r;
    int from_c;
    int to_r;
    int to_c;

public:
    FourByFour_Move(int from_r, int from_c,
                    int to_r,   int to_c,
                    char symbol)
        // We still call base constructor, but we treat x,y as "from" only.
        : Move<char>(from_r, from_c, symbol),
          from_r(from_r), from_c(from_c),
          to_r(to_r), to_c(to_c) {}

    int get_from_r() const { return from_r; }
    int get_from_c() const { return from_c; }
    int get_to_r()   const { return to_r;   }
    int get_to_c()   const { return to_c;   }
};

class FourByFour_Board : public Board<char> {
public:
    FourByFour_Board();

    // Core game logic
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

    // Helper functions for AI
    vector<FourByFour_Move> get_valid_moves(char symbol);
    bool check_win_for_symbol(char symbol);
    int calc_heuristic_value(char ai_symbol, char opp_symbol);

    // Direct board manipulation for AI simulation
    void set_cell(int r, int c, char symbol) { board[r][c] = symbol; }
    char get_cell(int r, int c) const { return board[r][c]; }
};

class FourByFour_UI : public UI<char> {
private:
    FourByFour_Board* board_ptr;

    // Minimax Algorithm
    int minimax(int depth, bool isMaximizing, char ai_symbol, char opp_symbol);

    FourByFour_Move find_best_move(char ai, char opp, int depth);

public:
    FourByFour_UI(FourByFour_Board* board);

    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* player) override;
};

#endif

/**
 * @file FourByFour_Classes.h
 * @brief Header for 4x4 Tic-Tac-Toe variant.
 *
 * This game involves moving existing tokens on a 4x4 grid rather than placing new ones.
 */

#ifndef FOURBYFOUR_CLASSES_H
#define FOURBYFOUR_CLASSES_H

#include "BoardGame_Classes.h"
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

using namespace std;

/**
 * @class FourByFour_Move
 * @brief Represents a move in the 4x4 game (Source -> Destination).
 */
class FourByFour_Move : public Move<char> {
    int from_r; ///< Source row.
    int from_c; ///< Source column.
    int to_r;   ///< Destination row.
    int to_c;   ///< Destination column.

public:
    /**
     * @brief Constructs a move object.
     */
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

/**
 * @class FourByFour_Board
 * @brief Logic for 4x4 Sliding Tic-Tac-Toe.
 */
class FourByFour_Board : public Board<char> {
public:
    /**
     * @brief Constructor for 4x4 Board.
     */
    FourByFour_Board();

    // Core game logic

    /**
     * @brief Updates the board (moves a piece from A to B).
     * @param move Move object containing source and dest.
     * @return true if valid move.
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Checks if player has won (3-in-a-row).
     * @param player Pointer to player.
     * @return true if win.
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief Checks if player has lost.
     * @return false.
     */
    bool is_lose(Player<char>* player) override;

    /**
     * @brief Checks if draw.
     * @return true if draw condition met.
     */
    bool is_draw(Player<char>* player) override;

    /**
     * @brief Checks if game is over.
     * @return true if win or draw.
     */
    bool game_is_over(Player<char>* player) override;

    // Helper functions for AI

    /**
     * @brief Gets all valid sliding moves for a symbol.
     */
    vector<FourByFour_Move> get_valid_moves(char symbol);

    /**
     * @brief Checks if a specific symbol has a winning line.
     */
    bool check_win_for_symbol(char symbol);

    /**
     * @brief Heuristic evaluation for AI.
     */
    int calc_heuristic_value(char ai_symbol, char opp_symbol);

    // Direct board manipulation for AI simulation
    void set_cell(int r, int c, char symbol) { board[r][c] = symbol; }
    char get_cell(int r, int c) const { return board[r][c]; }
};

/**
 * @class FourByFour_UI
 * @brief User interface and AI solver for 4x4 game.
 */
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
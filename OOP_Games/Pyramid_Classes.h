/**
 * @file Pyramid_Classes.h
 * @brief Header file for Pyramid Tic-Tac-Toe.
 *
 * Defines a board with a pyramid shape (1 cell top, 3 middle, 5 bottom).
 * Includes AI logic using Minimax algorithm.
 */

#ifndef PYRAMID_CLASSES_H
#define PYRAMID_CLASSES_H

#include "BoardGame_Classes.h"
#include <vector>
#include <string>
#include <algorithm>
#include <limits>

using namespace std;

/**
 * @class Pyramid_Board
 * @brief Represents the pyramid-shaped board.
 *
 * The internal board is a 3x5 matrix, but only specific cells form the pyramid.
 * Valid positions:
 * - Row 0: Col 2
 * - Row 1: Cols 1, 2, 3
 * - Row 2: Cols 0, 1, 2, 3, 4
 */
class Pyramid_Board : public Board<char> {
public:
    /**
     * @brief Constructor. Initializes the pyramid shape.
     */
    Pyramid_Board();

    /**
     * @brief Updates the board with a player's move.
     * @param move Pointer to the Move object.
     * @return true if valid move, false otherwise.
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Checks if the given player has won.
     * @param player Pointer to the player.
     * @return true if player has 3 in a row/diagonal, false otherwise.
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief Checks if the player has lost.
     * @param player Pointer to the player.
     * @return Always false for standard Pyramid TTT.
     */
    bool is_lose(Player<char>* player) override;

    /**
     * @brief Checks if the game is a draw.
     * @param player Pointer to the player.
     * @return true if board full and no winner.
     */
    bool is_draw(Player<char>* player) override;

    /**
     * @brief Checks if the game is over.
     * @param player Pointer to the player.
     * @return true if win or draw condition met.
     */
    bool game_is_over(Player<char>* player) override;

    /**
     * @brief Checks if a matrix coordinate corresponds to a valid pyramid cell.
     * @param row Matrix row index (0-2).
     * @param col Matrix column index (0-4).
     * @return true if valid, false otherwise.
     */
    bool is_valid_position(int row, int col);

    /**
     * @brief Returns a list of all valid empty cells (used by AI).
     * @return Vector of valid coordinates.
     */
    vector<pair<int, int>> get_valid_moves();

    // --- Minimax helpers for AI ---

    /**
     * @brief Checks if a specific symbol has won.
     * Useful for checking hypothetical states in Minimax.
     * @param symbol The symbol to check.
     * @return true if symbol has a winning line.
     */
    bool check_win_symbol(char symbol);

    /**
     * @brief Computes the best move for the AI using Minimax.
     * @param aiSymbol The AI's symbol.
     * @return The best (row, col) pair.
     */
    pair<int, int> get_best_move(char aiSymbol);

    /**
     * @brief The Minimax recursive algorithm.
     * @param depth Current depth of the recursion.
     * @param isMaximizing true if it's the AI's turn to maximize score.
     * @param aiSymbol AI's symbol.
     * @param humanSymbol Opponent's symbol.
     * @return The score of the board state.
     */
    int minimax(int depth, bool isMaximizing, char aiSymbol, char humanSymbol);
};

/**
 * @class Pyramid_UI
 * @brief UI handling for Pyramid Tic-Tac-Toe.
 */
class Pyramid_UI : public UI<char> {
private:
    Pyramid_Board* board_ptr;

public:
    /**
     * @brief Constructor for Pyramid UI.
     * @param board Pointer to the Pyramid board.
     */
    explicit Pyramid_UI(Pyramid_Board* board);

    /**
     * @brief Creates a player.
     * @param name Name of player.
     * @param symbol Symbol for player.
     * @param type Human or AI.
     * @return Pointer to Player object.
     */
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;

    /**
     * @brief Requests a move from the player.
     * @param player Pointer to player.
     * @return Pointer to Move object.
     */
    Move<char>* get_move(Player<char>* player) override;
};

#endif // PYRAMID_CLASSES_H
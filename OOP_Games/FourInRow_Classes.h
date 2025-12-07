/**
 * @file FourInRow_Classes.h
 * @brief Header file for the Four-in-a-Row game classes.
 *
 * This file declares the board logic and user interface for the Connect 4 game,
 * including AI implementation using Minimax with Alpha-Beta Pruning.
 */

#ifndef FOURINROW_CLASSES_H
#define FOURINROW_CLASSES_H

#include "BoardGame_Classes.h"
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

/**
 * @class FourInRow_Board
 * @brief Represents the 6x7 board for the Four-in-a-Row game.
 *
 * Handles board updates (gravity logic), win detection across 4 directions,
 * and heuristic evaluation for the AI.
 */
class FourInRow_Board : public Board<char> {
private:
    char blank_symbol = '.'; ///< Symbol representing an empty slot.

    // --- Core game logic helpers ---

    /**
     * @brief Checks a specific direction for 4 consecutive symbols.
     * @param row Starting row.
     * @param col Starting column.
     * @param delta_row Row increment (direction).
     * @param delta_col Column increment (direction).
     * @param symbol The player symbol to check.
     * @return true if 4 connected symbols are found.
     */
    bool check_direction(int row, int col, int delta_row, int delta_col, char symbol);

    /**
     * @brief Checks if a move at (row, col) triggered a win condition.
     * @param row Row index.
     * @param col Column index.
     * @param symbol Player symbol.
     * @return true if the move completes a line of 4.
     */
    bool check_win_condition(int row, int col, char symbol);

    /**
     * @brief Finds the lowest available row in a specific column (Gravity logic).
     * @param col The column index.
     * @return The row index if available, or -1 if the column is full.
     */
    int get_next_empty_row(int col);


    /**
     * @brief Evaluates a window of 4 cells for scoring.
     * @param window A vector of 4 characters representing a segment.
     * @param piece The AI's symbol.
     * @return An integer score for this window.
     */
    int evaluate_window(const vector<char>& window, char piece);

    /**
     * @brief Calculates the total heuristic score of the board.
     * @param piece The AI's symbol.
     * @return The total score (higher is better for 'piece').
     */
    int score_position(char piece);

    /**
     * @brief Minimax algorithm with Alpha-Beta Pruning.
     * @param depth Current depth of the recursion tree.
     * @param alpha The best value that the maximizer can guarantee.
     * @param beta The best value that the minimizer can guarantee.
     * @param maximizingPlayer Boolean flag (true for AI, false for Opponent).
     * @param ai_symbol The AI's character.
     * @return The evaluation score of the board state.
     */
    int minimax(int depth, int alpha, int beta, bool maximizingPlayer, char ai_symbol);

public:
    /**
     * @brief Constructor initializing a 6x7 board.
     */
    FourInRow_Board();


    /**
     * @brief Updates the board by dropping a piece into a column.
     * @param move The move containing the column index (y-coordinate).
     * @return true if the move is valid, false otherwise.
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Checks if the player has won.
     * @param player Pointer to player.
     * @return true if player has 4 connected.
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief Checks if the player has lost (not strictly used in this logic).
     * @return Always false in this implementation.
     */
    bool is_lose(Player<char>* player) override;

    /**
     * @brief Checks if the board is full without a winner.
     * @param player Pointer to player.
     * @return true if draw.
     */
    bool is_draw(Player<char>* player) override;

    /**
     * @brief Checks if the game is over (Win or Draw).
     * @param player Pointer to player.
     * @return true if game ended.
     */
    bool game_is_over(Player<char>* player) override;


    /**
     * @brief Validates if a move can be made in the given column.
     * @param col Column index.
     * @return true if valid.
     */
    bool is_valid_move(int col);

    /**
     * @brief Returns a list of all valid column indices.
     * @return Vector of valid columns.
     */
    vector<int> get_valid_moves();


    /**
     * @brief Determines the best move for the AI using Minimax.
     * @param ai_symbol The AI's symbol.
     * @return The best column index.
     */
    int get_best_move(char ai_symbol);
};


/**
 * @class FourInRow_UI
 * @brief User Interface for Four-in-a-Row.
 */
class FourInRow_UI : public UI<char> {
private:
    FourInRow_Board* board_ptr; ///< Pointer to the specific board logic.

public:
    /**
     * @brief Constructor.
     * @param board Pointer to the FourInRow_Board.
     */
    FourInRow_UI(FourInRow_Board* board);

    /**
     * @brief Destructor.
     */
    ~FourInRow_UI() {}

    /**
     * @brief Creates a player (Human or Computer).
     */
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;

    /**
     * @brief Gets the column input from the user or calculates AI move.
     * @param player Pointer to player.
     * @return Move object.
     */
    Move<char>* get_move(Player<char>* player) override;

    /**
     * @brief Sets up the two players.
     */
    Player<char>** setup_players() override;
};

#endif
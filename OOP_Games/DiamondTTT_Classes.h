/**
 * @file DiamondTTT_Classes.h
 * @brief Header file for Diamond Tic-Tac-Toe.
 *
 * Defines the logic for a 5x5 grid where valid cells form a diamond shape.
 * Winning requires forming both a line of 3 and a line of 4 simultaneously (or variant logic).
 */

#ifndef DIAMONDTTT_CLASSES_H
#define DIAMONDTTT_CLASSES_H

#include "BoardGame_Classes.h"
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

/**
 * @class Diamond_Board
 * @brief Represents the diamond-shaped board.
 *
 * The board is technically 5x5, but only cells with Manhattan distance <= 2 from center (2,2) are valid.
 */
class Diamond_Board : public Board<char> {
public:
    /**
     * @brief Constructor for Diamond Board.
     */
    Diamond_Board();
    virtual ~Diamond_Board() {}

    // --- Core Game Logic ---

    /**
     * @brief Updates board with player move.
     * @param move Pointer to move object.
     * @return true if move valid and applied.
     */
    virtual bool update_board(Move<char>* move) override;

    /**
     * @brief Checks if player has won (e.g. formed specific patterns).
     * @param player Pointer to player.
     * @return true if win condition met.
     */
    virtual bool is_win(Player<char>* player) override;

    /**
     * @brief Checks if player has lost.
     * @return false (standard rules).
     */
    virtual bool is_lose(Player<char>* player) override;

    /**
     * @brief Checks if game is a draw.
     * @return true if board full and no winner.
     */
    virtual bool is_draw(Player<char>* player) override;

    /**
     * @brief Checks if game is over.
     * @return true if win or draw.
     */
    virtual bool game_is_over(Player<char>* player) override;

    // --- Helper Functions ---

    /**
     * @brief Checks if a coordinate is within the diamond shape.
     * @param r Row index.
     * @param c Column index.
     * @return true if valid diamond cell (Manhattan distance <= 2).
     */
    bool valid_cell(int r, int c) const;

    /**
     * @brief Gets all valid coordinates in the diamond.
     */
    vector<pair<int,int>> get_valid_positions() const;

    /**
     * @brief Gets all currently empty valid positions.
     */
    vector<pair<int,int>> get_empty_positions() const;

    // --- AI Simulation Helpers ---
    bool make_temp_move(int r, int c, char symbol);
    void undo_temp_move(int r, int c);

    /**
     * @brief Checks if placing a symbol at (r,c) creates the winning configuration.
     * A win usually requires specific lines (e.g., 3-line and 4-line) intersecting or existing.
     */
    bool placement_creates_win(int r, int c, char symbol) const;

    static const char blank_symbol;

private:
    const int dr[4] = {0, 1, 1, 1};  ///< Row direction vectors.
    const int dc[4] = {1, 0, 1, -1}; ///< Column direction vectors.

    bool exists_contiguous_line_including(int r, int c, int len, int dir, char symbol) const;

    vector<pair<int,int>> valid_positions; ///< Caches valid diamond coordinates.
};

/**
 * @class Diamond_AIPlayer
 * @brief AI player using backtracking/minimax with memoization.
 */
class Diamond_AIPlayer : public Player<char> {
public:
    Diamond_AIPlayer(const string& name, char symbol);
    virtual ~Diamond_AIPlayer() {}

    /**
     * @brief Calculates best move for AI.
     */
    Move<char>* get_smart_move(Diamond_Board* board, char opponent_symbol);

private:
    int backtrack_minimax(Diamond_Board* board, char current_symbol, int alpha, int beta);
    string board_key(Diamond_Board* board) const;
    unordered_map<string,int> memo; ///< Transposition table for caching board states.
    pair<int,int> tiebreak_choose(const vector<pair<int,int>>& choices) const;
};

/**
 * @class Diamond_UI
 * @brief UI handling for Diamond Tic-Tac-Toe.
 */
class Diamond_UI : public UI<char> {
    Diamond_Board* board_ptr;
public:
    Diamond_UI(Diamond_Board* board);
    virtual ~Diamond_UI() {}

    virtual Player<char>** setup_players() override;
    virtual Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    virtual Move<char>* get_move(Player<char>* player) override;
    virtual void display_board_matrix(const vector<vector<char>>& matrix) const override;
};

#endif
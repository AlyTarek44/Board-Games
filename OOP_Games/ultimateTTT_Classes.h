/**
 * @file ultimateTTT_Classes.h
 * @brief Header for Ultimate Tic-Tac-Toe.
 *
 * A 9x9 board divided into nine 3x3 sub-boards. Winning a sub-board grants the cell in the main 3x3 board.
 */

#ifndef ULTIMATE_CLASSES_H
#define ULTIMATE_CLASSES_H

#include "BoardGame_Classes.h"
#include <string>
#include <vector>

using namespace std;

/**
 * @class Ultimate_Board
 * @brief Represents the 9x9 Ultimate Tic-Tac-Toe board.
 */
class Ultimate_Board : public Board<char> {
public:
    /**
     * @brief Default constructor.
     */
    Ultimate_Board();
    virtual ~Ultimate_Board() {}

    /**
     * @brief Updates board with player's move.
     * @param move Pointer to Move object (containing coords and symbol).
     * @return true if move is valid and applied.
     */
    virtual bool update_board(Move<char>* move) override;

    /**
     * @brief Checks if player has won the main 3x3 board.
     * @param player Pointer to player.
     * @return true if player owns 3 consecutive sub-boards.
     */
    virtual bool is_win(Player<char>* player) override;

    /**
     * @brief Checks if player has lost.
     * @param player Pointer to player.
     * @return Always false for standard rules.
     */
    virtual bool is_lose(Player<char>* player) override;

    /**
     * @brief Checks if game is a draw.
     * @param player Pointer to player.
     * @return true if no winner and no moves left.
     */
    virtual bool is_draw(Player<char>* player) override;

    /**
     * @brief Checks if game is over.
     * @param player Pointer to player.
     * @return true if win or draw.
     */
    virtual bool game_is_over(Player<char>* player) override;

    /**
     * @brief Validates if a move is within bounds of a sub-board.
     */
    bool valid_subcell(int main_idx, int sub_idx) const;

    /**
     * @brief Gets all allowed moves based on current state and forced board.
     */
    vector<pair<int,int>> get_allowed_moves() const;

    /**
     * @brief Gets empty cells in a specific sub-board.
     */
    vector<pair<int,int>> get_all_empty_in_main(int main_idx) const;

    // ... (helper functions for AI simulation omitted for brevity but should be documented similarly)
    bool make_temp_move(int main_idx, int sub_idx, char symbol);
    void undo_temp_move(int main_idx, int sub_idx, char prev_claim);

    vector<char> get_main_claims() const { return main_claims; }
    int get_forced_main() const { return forced_main; }

    static const char blank_symbol;

private:
    vector<char> main_claims; ///< Status of the 9 main sub-boards.
    int forced_main;          ///< Index of sub-board next player MUST play in.

    char check_subboard_winner(int main_idx) const;
    bool subboard_full(int main_idx) const;
    char check_three_in_row_on_main(char who) const;

    /**
     * @brief Converts main/sub indices to global 9x9 grid coordinates.
     */
    inline void unpack_indices(int main_idx, int sub_idx, int &gr, int &gc) const {
        int mr = main_idx / 3, mc = main_idx % 3;
        int sr = sub_idx / 3, sc = sub_idx % 3;
        gr = mr * 3 + sr;
        gc = mc * 3 + sc;
    }
};

/**
 * @class Ultimate_AIPlayer
 * @brief AI Player for Ultimate Tic-Tac-Toe.
 */
class Ultimate_AIPlayer : public Player<char> {
public:
    Ultimate_AIPlayer(const string& name, char symbol);
    virtual ~Ultimate_AIPlayer() {}

    /**
     * @brief Selects a move for the AI.
     * @param board Pointer to the Ultimate_Board.
     * @return Pointer to selected Move.
     */
    Move<char>* get_simple_move(Ultimate_Board* board);

private:
    pair<int,int> pick_random(const vector<pair<int,int>>& v) const;
};

/**
 * @class Ultimate_UI
 * @brief UI for Ultimate Tic-Tac-Toe.
 */
class Ultimate_UI : public UI<char> {
    Ultimate_Board* board_ptr;
public:
    Ultimate_UI(Ultimate_Board* board);
    virtual ~Ultimate_UI() {}

    virtual Player<char>** setup_players() override;
    virtual Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    virtual Move<char>* get_move(Player<char>* player) override;

    /**
     * @brief Displays the board state.
     */
    virtual void display_board_matrix(const vector<vector<char>>& matrix) const override;

    void print_main_board_only() const;
    void print_full_board() const;
};

#endif
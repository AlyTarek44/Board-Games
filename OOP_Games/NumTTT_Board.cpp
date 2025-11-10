#include "NumTTT_Board.h"
#include <iostream>
#include <vector>
#include <numeric>   // For std::accumulate
#include <algorithm> // For std::find

using namespace std;

//================================
// Board Class Implementation
//================================

// Constructor
NumTTT_Board::NumTTT_Board() : Board(3, 3) {
    // Initialize the board with the blank symbol (0)
    for (auto &row : board) {
        for (auto &cell : row) {
            cell = blank_symbol;
        }
    }
    // Initialize the number lists
    odd_nums = {1, 3, 5, 7, 9};
    even_nums = {2, 4, 6, 8};
}

/**
 * @brief Updates the board with the given move.
 * This is the core validation logic.
 */
bool NumTTT_Board::update_board(Move<int>* move) {
    int x = move->get_x();
    int y = move->get_y();
    int number = move->get_symbol(); // This is the number (1-9) to play

    // 1. Check bounds
    if (x < 0 || x >= rows || y < 0 || y >= columns) {
        if (x != -1) { // Don't print for dummy moves
            cout << "Invalid move: Position (" << x << "," << y << ") is out of bounds.\n";
        }
        return false;
    }

    // 2. Check if cell is empty
    if (board[x][y] != blank_symbol) {
       cout << "Invalid move: Cell (" << x << "," << y << ") is already taken.\n";
        return false;
    }

    // 3. Determine whose turn it is and validate the number
    bool is_odd_turn = (n_moves % 2 == 0);

    if (is_odd_turn) {
        // Player 1 (Odd) turn
        auto it = find(odd_nums.begin(), odd_nums.end(), number);
        if (it != odd_nums.end()) {
            odd_nums.erase(it); // Valid number, remove it from list
        } else {
            cout << "Invalid move: " << number << " is not an available odd number.\n";
            return false;
        }
    } else {
        // Player 2 (Even) turn
        auto it = find(even_nums.begin(), even_nums.end(), number);
        if (it != even_nums.end()) {
            even_nums.erase(it); // Valid number, remove it from list
        } else {
            cout << "Invalid move: " << number << " is not an available even number.\n";
            return false;
        }
    }

    // 4. All checks passed.
    board[x][y] = number;
    n_moves++;
    return true;
}

/**
 * @brief Checks if the current player has won by summing lines to 15.
 */
bool NumTTT_Board::is_win(Player<int>* player) {
    return check_all_lines();
}

/**
 * @brief Helper function to check all 8 lines.
 * @return true if any line sums to 15, false otherwise.
 */
bool NumTTT_Board::check_all_lines() {
    int sum;

    // Check rows
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] != 0 && board[i][1] != 0 && board[i][2] != 0) {
            sum = board[i][0] + board[i][1] + board[i][2];
            if (sum == 15) return true;
        }
    }

    // Check columns
    for (int j = 0; j < 3; ++j) {
        if (board[0][j] != 0 && board[1][j] != 0 && board[2][j] != 0) {
            sum = board[0][j] + board[1][j] + board[2][j];
            if (sum == 15) return true;
        }
    }

    // Check main diagonal
    if (board[0][0] != 0 && board[1][1] != 0 && board[2][2] != 0) {
        sum = board[0][0] + board[1][1] + board[2][2];
        if (sum == 15) return true;
    }

    // Check anti-diagonal
    if (board[0][2] != 0 && board[1][1] != 0 && board[2][0] != 0) {
        sum = board[0][2] + board[1][1] + board[2][0];
        if (sum == 15) return true;
    }

    return false; // No winning line found
}

/**
 * @brief Checks if the game is a draw.
 */
bool NumTTT_Board::is_draw(Player<int>* player) {
    // Called after is_win() fails, so just check if board is full
    return (n_moves == 9);
}

/**
 * @brief Checks if the game is over (win or draw).
 */
bool NumTTT_Board::game_is_over(Player<int>* player) {
    return is_win(player) || is_draw(player);
}
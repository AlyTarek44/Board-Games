/**
 * @file NUMTTT_classes.cpp
 * @brief Implementation of Numerical Tic-Tac-Toe.
 */

#include "NUMTTT_classes.h"
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

// -----------------------------------------------------------------------------
// NumTTT_Board Implementation
// -----------------------------------------------------------------------------

NumTTT_Board::NumTTT_Board() : Board(3, 3) {
    // Initialize board with 0 (blank)
    for (auto &row : board) {
        for (auto &cell : row) {
            cell = blank_symbol;
        }
    }
    // Initialize number pools
    odd_nums = {1, 3, 5, 7, 9};
    even_nums = {2, 4, 6, 8};
}

bool NumTTT_Board::update_board(Move<int>* move) {
    int x = move->get_x();
    int y = move->get_y();
    int number = move->get_symbol(); // The number played

    // 1. Check Bounds
    if (x < 0 || x >= rows || y < 0 || y >= columns) {
        if (x != -1) cout << "Invalid move: Position out of bounds.\n";
        return false;
    }

    // 2. Check Empty
    if (board[x][y] != blank_symbol) {
       cout << "Invalid move: Cell occupied.\n";
        return false;
    }

    // 3. Validate Number Ownership
    bool is_odd_turn = (n_moves % 2 == 0); // Player 1 (Odd) always starts

    if (is_odd_turn) {
        auto it = find(odd_nums.begin(), odd_nums.end(), number);
        if (it != odd_nums.end()) {
            odd_nums.erase(it); // Consume number
        } else {
            cout << "Invalid move: " << number << " is not available/valid for Odd player.\n";
            return false;
        }
    } else {
        auto it = find(even_nums.begin(), even_nums.end(), number);
        if (it != even_nums.end()) {
            even_nums.erase(it); // Consume number
        } else {
            cout << "Invalid move: " << number << " is not available/valid for Even player.\n";
            return false;
        }
    }

    // 4. Apply
    board[x][y] = number;
    n_moves++;
    return true;
}

bool NumTTT_Board::check_all_lines() {
    int sum;
    // Rows
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] && board[i][1] && board[i][2]) {
            sum = board[i][0] + board[i][1] + board[i][2];
            if (sum == 15) return true;
        }
    }
    // Columns
    for (int j = 0; j < 3; ++j) {
        if (board[0][j] && board[1][j] && board[2][j]) {
            sum = board[0][j] + board[1][j] + board[2][j];
            if (sum == 15) return true;
        }
    }
    // Diagonals
    if (board[0][0] && board[1][1] && board[2][2]) {
        sum = board[0][0] + board[1][1] + board[2][2];
        if (sum == 15) return true;
    }
    if (board[0][2] && board[1][1] && board[2][0]) {
        sum = board[0][2] + board[1][1] + board[2][0];
        if (sum == 15) return true;
    }

    return false;
}

bool NumTTT_Board::is_win(Player<int>* player) {
    return check_all_lines();
}

bool NumTTT_Board::is_draw(Player<int>* player) {
    return (n_moves == 9);
}

bool NumTTT_Board::game_is_over(Player<int>* player) {
    return is_win(player) || is_draw(player);
}

// -----------------------------------------------------------------------------
// NumTTT_UI Implementation
// -----------------------------------------------------------------------------

NumTTT_UI::NumTTT_UI(NumTTT_Board *b) : UI<int>("Welcome to Numerical Tic-Tac-Toe!", 3), board_ptr(b) {}

Player<int> **NumTTT_UI::setup_players() {
    Player<int> **players = new Player<int> *[2];
    vector<string> type_options = { "Human", "Computer" };

    cout << "\n--- Player Setup ---\n";
    // Symbol 1 for Odd, 2 for Even (internal IDs)
    string name1 = get_player_name("Player 1 (Odd)");
    PlayerType type1 = get_player_type_choice("Player 1", type_options);
    players[0] = create_player(name1, 1, type1);

    string name2 = get_player_name("Player 2 (Even)");
    PlayerType type2 = get_player_type_choice("Player 2", type_options);
    players[1] = create_player(name2, 2, type2);

    return players;
}

Player<int> *NumTTT_UI::create_player(string &name, int symbol, PlayerType type) {
    string type_str = (type == PlayerType::HUMAN) ? "Human" : "Computer";
    cout << "Created player: " << name << " (" << (symbol == 1 ? "Odd" : "Even") << ") [" << type_str << "]\n";
    return new Player<int>(name, symbol, type);
}

Move<int> *NumTTT_UI::get_move(Player<int> *player) {
    int x, y, number;
    bool is_odd_player = (player->get_symbol() == 1);

    cout << "---------------------------------\n" << player->get_name() << "'s turn.\n";

    if (player->get_type() == PlayerType::HUMAN) {
        if (is_odd_player) {
            cout << "Available Odd Numbers: "; print_available(board_ptr->get_odd_nums());
        } else {
            cout << "Available Even Numbers: "; print_available(board_ptr->get_even_nums());
        }

        while (true) {
            cout << "Enter number to play: ";
            if (cin >> number) break;
            cout << "Invalid int.\n"; cin.clear(); cin.ignore(10000, '\n');
        }
        while (true) {
            cout << "Enter position (row col): ";
            if (cin >> x >> y) break;
            cout << "Invalid ints.\n"; cin.clear(); cin.ignore(10000, '\n');
        }

    } else {
        // Computer Logic: Random valid move
        const vector<int>& available_list = is_odd_player ? board_ptr->get_odd_nums() : board_ptr->get_even_nums();
        number = available_list[rand() % available_list.size()];

        int r = board_ptr->get_rows();
        int c = board_ptr->get_columns();
        do {
            x = rand() % r;
            y = rand() % c;
        } while (board_ptr->get_board_matrix()[x][y] != 0);

        cout << "Computer chose number " << number << " at (" << x << ", " << y << ")\n";
    }
    return new Move<int>(x, y, number);
}

void NumTTT_UI::print_available(const vector<int> &nums) {
    cout << "{ ";
    for (size_t i = 0; i < nums.size(); ++i) cout << nums[i] << (i == nums.size() - 1 ? " " : ", ");
    cout << "}\n";
}
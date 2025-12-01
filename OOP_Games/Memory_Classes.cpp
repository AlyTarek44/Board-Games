#include <iostream>
#include <iomanip>
#include <cctype>
#include <thread>
#include <chrono>
#include <cstdlib>
#include "Memory_Classes.h"

using namespace std;

void Memory_UI::clear_screen() {
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#else
    system("clear");
#endif
}

Memory_Board::Memory_Board() : Board<char>(3, 3) {
    real_board = vector<vector<char>>(3, vector<char>(3, blank_symbol));
}

bool Memory_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    if (x < 0 || x >= 3 || y < 0 || y >= 3 || real_board[x][y] != blank_symbol) {
        return false;
    }

    n_moves++;
    real_board[x][y] = toupper(mark);
    return true;
}

vector<vector<char>> Memory_Board::get_board_matrix() const {
    // Always return hidden board to main display
    vector<vector<char>> hidden_board = vector<vector<char>>(3, vector<char>(3, blank_symbol));

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (real_board[i][j] != blank_symbol) {
                hidden_board[i][j] = hidden_symbol;
            }
        }
    }
    return hidden_board;
}

vector<vector<char>> Memory_Board::get_move_display_board(int move_x, int move_y, char symbol) const {
    // Show board with the current move visible
    vector<vector<char>> move_board = vector<vector<char>>(3, vector<char>(3, blank_symbol));

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (real_board[i][j] != blank_symbol) {
                if (i == move_x && j == move_y) {
                    // Show the current move
                    move_board[i][j] = symbol;
                } else {
                    // Hide all other moves
                    move_board[i][j] = hidden_symbol;
                }
            }
        }
    }
    return move_board;
}

bool Memory_Board::is_win(Player<char>* player) {
    const char sym = player->get_symbol();

    auto all_equal = [&](char a, char b, char c) {
        return a == b && b == c && a != blank_symbol;
    };

    for (int i = 0; i < 3; ++i) {
        if ((all_equal(real_board[i][0], real_board[i][1], real_board[i][2]) && real_board[i][0] == sym) ||
            (all_equal(real_board[0][i], real_board[1][i], real_board[2][i]) && real_board[0][i] == sym))
            return true;
    }

    if ((all_equal(real_board[0][0], real_board[1][1], real_board[2][2]) && real_board[1][1] == sym) ||
        (all_equal(real_board[0][2], real_board[1][1], real_board[2][0]) && real_board[1][1] == sym))
        return true;

    return false;
}

bool Memory_Board::is_draw(Player<char>* player) {
    return (n_moves == 9 && !is_win(player));
}

bool Memory_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

Memory_UI::Memory_UI(Memory_Board* board) : UI<char>("Memory Tic-Tac-Toe - Test Your Memory!", 3), memory_board(board) {}

Player<char>* Memory_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";
    return new Player<char>(name, symbol, type);
}

void Memory_UI::display_board_matrix(const vector<vector<char>>& matrix) const {
    UI<char>::display_board_matrix(matrix);
}

Move<char>* Memory_UI::get_move(Player<char>* player) {
    int x, y;

    // Show hidden board before getting move
    cout << "\nCurrent board (all positions hidden):\n";
    display_board_matrix(memory_board->get_board_matrix());

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\n" << player->get_name() << " (" << player->get_symbol()
             << "), enter your move x and y (0 to 2): ";
        cin >> x >> y;
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        do {
            x = rand() % 3;
            y = rand() % 3;
        } while (memory_board->get_board_matrix()[x][y] != '.');
        cout << "\nComputer " << player->get_name() << " chose position (" << x << ", " << y << ")\n";
    }
    return new Move<char>(x, y, player->get_symbol());
}

void Memory_UI::run_game(Player<char>* players[2]) {
    display_board_matrix(memory_board->get_board_matrix());
    Player<char>* currentPlayer = players[0];

    while (true) {
        for (int i : {0, 1}) {
            currentPlayer = players[i];

            Move<char>* move = get_move(currentPlayer);

            while (!memory_board->update_board(move)) {

                cout << "Invalid move! Try again.\n";
                delete move;
                move = get_move(currentPlayer);
            }

            cout << "\nYour move was placed at (" << move->get_x() << ", " << move->get_y() << "):\n";
            display_board_matrix(memory_board->get_move_display_board(move->get_x(), move->get_y(), currentPlayer->get_symbol()));

            cout << "Remember your move...\n\n";
            this_thread::sleep_for(chrono::milliseconds(1100));

            clear_screen();

            delete move;

            if (memory_board->is_win(currentPlayer)) {
                display_message(currentPlayer->get_name() + " wins!");
                return;
            }
            if (memory_board->is_draw(currentPlayer)) {
                display_message("Draw!");
                return;
            }
        }
    }
}
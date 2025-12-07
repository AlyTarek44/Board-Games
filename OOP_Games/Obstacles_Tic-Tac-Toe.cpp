/**
 * @file Obstacles_Tic-Tac-Toe.cpp
 * @brief Implementation of Obstacles Tic-Tac-Toe.
 */

#include "Obstacles_Tic-Tac-Toe.h"
#include <iostream>
#include <iomanip>
#include <cctype>

obstacles_TTT_board::obstacles_TTT_board() : Board(6, 6) {
     for (auto& row : board) {
          for (auto& cell : row) {
               cell = blank_symbol;
          }
     }
}

vector<pair<int, int>> obstacles_TTT_board::get_moves() {
     vector<pair<int, int>> moves;
     for (int i = 0; i < rows; i++) {
          for (int j = 0; j < columns; j++) {
               if (board[i][j] == blank_symbol) {
                    moves.push_back(make_pair(i, j));
               }
          }
     }
     return moves;
}

bool obstacles_TTT_board::update_board(Move<char> *move) {
     int x = move->get_x();
     int y = move->get_y();
     char symbol = move->get_symbol();

     // Validate bounds and emptiness
     if ( x < 0 || x >= rows || y < 0 || y >= columns || board[x][y] != '.' ) {
          return false;
     }

     board[x][y] = symbol;
     n_moves++;

     // Place obstacles randomly every 2 moves
     if (n_moves % 2 == 0) {
         for (int i = 0; i < 2; i++) { // Place up to 2 obstacles? (Logic in snippet implies 2 iterations)
              auto moves = get_moves();
              if (!moves.empty()) {
                   int r = rand() % moves.size();
                   int k = moves[r].first;
                   int l = moves[r].second;
                   board[k][l] = obstacle_symbol;
              }
         }
     }
     return true;
}

bool obstacles_TTT_board::check_win() {
     auto all_equal = [&](char a, char b, char c, char d) {
          return a == b && b == c && c == d && a != blank_symbol && a != obstacle_symbol;
     };

     // Check rows (scan for 4 in a row)
     for (int i = 0; i < rows; i++) {
          for (int j = 0; j < columns; j++) {
               if (i + 3 >= rows) break; // Optimization: cant fit 4 vertically starting here? Wait, this loop is rows...
               // Note: Logic in provided snippet seemed to mix indices slightly, standardizing 4-check:

               // Logic from original snippet:
               // Row check needs columns index j to go up to col-3
               // The provided snippet: checks board[i+k][j] (vertical) inside row loops?
               // Let's implement standard 4-way check correctly.

               // 1. Horizontal
               if (j + 3 < columns && all_equal(board[i][j], board[i][j+1], board[i][j+2], board[i][j+3])) return true;

               // 2. Vertical
               if (i + 3 < rows && all_equal(board[i][j], board[i+1][j], board[i+2][j], board[i+3][j])) return true;

               // 3. Diagonal (Down-Right)
               if (i + 3 < rows && j + 3 < columns &&
                   all_equal(board[i][j], board[i+1][j+1], board[i+2][j+2], board[i+3][j+3])) return true;

               // 4. Anti-Diagonal (Down-Left)
               if (i + 3 < rows && j - 3 >= 0 &&
                   all_equal(board[i][j], board[i+1][j-1], board[i+2][j-2], board[i+3][j-3])) return true;
          }
     }
     return false;
}

bool obstacles_TTT_board::is_win(Player<char> *) {
     return check_win();
}

bool obstacles_TTT_board::is_draw(Player<char> *) {
     // Draw if no moves left
     return get_moves().empty();
}

bool obstacles_TTT_board::is_lose(Player<char> *) { return false; }

bool obstacles_TTT_board::game_is_over(Player<char> *) {
     return check_win() || get_moves().empty();
}

// --- UI ---

obstacles_TTT_UI::obstacles_TTT_UI(obstacles_TTT_board *b) : UI<char>("Welcome to obstacles Tic-Tac-Toe",3), board(b){}

Player<char> *obstacles_TTT_UI::createplayer(string& name, char symbol, PlayerType type) {
     cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
       << " player: " << name << " (" << symbol << ")\n";
     return new Player<char>(name, symbol, type);
}

Move<char> *obstacles_TTT_UI::get_move(Player<char> *player) {
     int x, y;
     if (player->get_type() == PlayerType::HUMAN) {
          cout << "Enter row: "; cin >> x;
          cout << "Enter column: "; cin >> y;
          return new Move<char>(x, y, player->get_symbol());
     } else {
          auto move = board->get_moves();
          if (move.empty()) return nullptr;
          auto rand_move = rand() % move.size();
          return new Move<char>(move[rand_move].first, move[rand_move].second, player->get_symbol());
     }
}
//
// Created by Ali Tarek on 01/12/2025.
//

#include "Obstacles_Tic-Tac-Toe.h"
#include <iostream>
#include <iomanip>
#include <cctype>  // for toupper()

obstacles_TTT_board::obstacles_TTT_board() :Board(6, 6) {
     for (auto& row : board) {
          for (auto& cell : row) {
               cell =blank_symbol;
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
     if ( x < 0 || x >= rows || y < 0 || y >= columns || board[x][y] != '.' ) {
          return false;
     }
     board[x][y] = symbol;
     n_moves++;



      // put obstacles every two moves
     if (n_moves % 2 == 0) {

         for (int i = 0; i < 2; i++) {
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

     //check rows
     for (int i = 0; i < rows; i++) {
          for (int j = 0; j < columns; j++) {
               if (i + 3 >= rows){break;}

               if (all_equal(board[i][j],board[i+1][j],board[i+2][j] , board[i+3][j] ) ) {
                    return true;
               }
          }
     }

     //check columns
     for (int i = 0; i < rows; i++) {
          for (int j = 0; j < columns; j++) {
               if (j+3 >= columns){break;}
               if (all_equal(board[i][j],board[i][j+1],board[i][j+2],board[i][j+3] ) ) {
                    return true;
               }
          }
     }

     //check diagonal
     for (int i = 0; i < rows; i++) {
          for (int j = 0; j < columns; j++) {
               if ( i + 3 >= rows || j + 3 >= columns){break;}
               if (all_equal(board[i][j],board[i+1][j+1],board[i+2][j+2],board[i+3][j+3] ) ) {
                    return true;
               }
          }
     }

     //check anti-diagonal

     for (int i = 0; i < rows; i++) {
          for (int j = columns-1 ; j >= 0; j--) {
                  if (i + 3 >= rows || j - 3 < 0){break;}
               if (all_equal(board[i][j],board[i+1][j-1],board[i+2][j-2],board[i+3][j-3] ) ) {
                    return true;
               }
          }
     }


      return false;

}

bool obstacles_TTT_board::is_win(Player<char> *) {
     if (check_win()) {
          return true;
     }
     return false;
}

bool obstacles_TTT_board::is_draw(Player<char> *) {
     if (get_moves().empty()) {
          return true;
     }
     return false;
}

bool obstacles_TTT_board::is_lose(Player<char> *) {
     return false;
}

bool obstacles_TTT_board::game_is_over(Player<char> *) {
     if (check_win() || get_moves().empty()) {
          return true;
     }
     return false;
}



// UI
obstacles_TTT_UI::obstacles_TTT_UI(obstacles_TTT_board *b) : UI<char>("Welcome to obstacles Tic-Tac-Toe",3),board(b){}

Player<char> *obstacles_TTT_UI::createplayer(string& name, char symbol, PlayerType type) {

     cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
       << " player: " << name << " (" << symbol << ")\n";

     return new Player<char>(name, symbol, type);

}

Move<char> *obstacles_TTT_UI::get_move(Player<char> *player) {
     int x, y;
     if (player->get_type() == PlayerType::HUMAN) {
          cout << "Enter row: ";
          cin >> x;

          cout << "Enter column: ";
          cin >> y;

          return new Move<char>(x, y, player->get_symbol());

     }
     else {

          auto move =board->get_moves();
         if (move.empty()) {
              return nullptr;
         }
          auto rand_move = rand() % move.size();
          int x = move[rand_move].first;
          int y = move[rand_move].second;
          return new Move<char>(x,y,player->get_symbol());
     }
}

























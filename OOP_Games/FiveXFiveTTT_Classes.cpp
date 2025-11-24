//
// Created by Ali Tarek on 23/11/2025.
////--------------------------------------- IMPLEMENTATION

#include <iostream>
#include <iomanip>
#include <cctype>  // for toupper()
#include "FiveXFiveTTT_Classes.h"


FxFTTT_board :: FxFTTT_board() : Board(5, 5) {
  for (auto& row : board){
    for (auto& cell : row){
      cell = blank_symbol;
    }
  }
}
bool FxFTTT_board :: update_board(Move<char>* move){
  int x=move->get_x();
  int y=move->get_y();
  char mark = move->get_symbol();

  if (!(x<0 || x>rows || y<0 || y>columns) && (board[x][y]==blank_symbol || mark== 0)){
    if (mark == 0) { // Undo move
      n_moves--;
      board[x][y] = blank_symbol;
      }
    else {         // Apply move
      n_moves++;
      board[x][y] = toupper(mark);
     }
    return true;
    }
  return false;
}

  int FxFTTT_board :: check_all_lines(){

    int sum_x=0, sum_o=0;


    auto all_equal = [&](char a, char b, char c) {
    return a == b && b == c && a != blank_symbol;

    };

    //check rows
  for(int i=0;i<rows;i++){

    for(int j=0;j+2<columns;j++){



      if(all_equal(board[i][j], board[i][j+1], board[i][j+2] )){

        if ( board[i][j]== 'X'){sum_x++;}

        else if ( board[i][j] == 'O' ){sum_o++;}
      }
    }
}
  //check columns
  for(int j=0;j<columns;j++){

    for(int i=0;i+2<rows;i++){



      if(all_equal(board[i][j], board[i+1][j], board[i+2][j] )){
        if ( board[i][j] == 'X'){sum_x++;}
        else if ( board[i][j] == 'O' ){sum_o++;}
      }
     }
    }

  //check main diagonal
 for (int i=0;i+2<rows;i++){
   for (int j=0;j+2<columns;j++){
     if(all_equal(board[i][j], board[i+1][j+1], board[i+2][j+2] )){
       if ( board[i][j] == 'X'){sum_x++;}
       else if ( board[i][j] == 'O' ){sum_o++;}
     }
   }
 }
 //check anti diagonal
 for (int j=columns-1;j-2>=0;j--){
   for (int i=0;i+2<rows;i++){
     if(all_equal(board[i][j], board[i+1][j-1], board[i+2][j-2] )){
       if ( board[i][j] == 'X'){sum_x++;}
       else if ( board[i][j] == 'O' ){sum_o++;}
     }
   }
 }
if (sum_x==sum_o) {return 0;}
if (sum_x>sum_o) {return 1;}
if (sum_x<sum_o) {return -1;}
}
bool FxFTTT_board::is_draw(Player<char>* Player) {
  if (n_moves < 24)
    return false;


  int result = check_all_lines();
  return (result == 0);
}
bool FxFTTT_board::is_win(Player<char>* player) {
  if (n_moves < 24)
    return false;

  int result = check_all_lines();

  if (player->get_symbol() == 'X')
    return result == 1;

  if (player->get_symbol() == 'O')
    return result == -1;

  return false;
}

bool FxFTTT_board :: is_lose(Player<char>*player) {
  if (n_moves < 24){return false;}


  int result = check_all_lines();

  if (player->get_symbol() == 'X')
    return result == -1;

  if (player->get_symbol() == 'O')
    return result == 1;

  return false;
}

FxFTTT_ui ::FxFTTT_ui() : UI<char>("Welcome to 5x5 tic tac toe by Ali",3) {}

  Player<char>* FxFTTT_ui::create_player(string& name, char symbol, PlayerType type ) {
  cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
       << " player: " << name << " (" << symbol << ")\n";

  return new Player<char>(name, symbol, type);
}
Move<char>* FxFTTT_ui::get_move(Player<char>* player) {
  int x, y;

  if (player->get_type() == PlayerType::HUMAN) {
    cout << "\nPlease enter your move x and y (0 to 4): ";
    cin >> x >> y;
  }
  else if (player->get_type() == PlayerType::COMPUTER) {
    x = rand() % player->get_board_ptr()->get_rows();
    y = rand() % player->get_board_ptr()->get_columns();
  }
  return new Move<char>(x, y, player->get_symbol());
}
bool FxFTTT_board::game_is_over(Player<char>* player) {
  return (n_moves == 24);
}
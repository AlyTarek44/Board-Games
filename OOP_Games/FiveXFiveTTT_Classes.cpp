/**
 * @file FiveXFiveTTT_Classes.cpp
 * @brief Implementation of 5x5 Tic-Tac-Toe logic.
 */

#include "FiveXFiveTTT_Classes.h"
#include <iostream>
#include <iomanip>
#include <cctype>

FxFTTT_board :: FxFTTT_board() : Board(5, 5) {
  for (auto& row : board){
    for (auto& cell : row){
      cell = blank_symbol;
    }
  }
}

bool FxFTTT_board::update_board(Move<char>* move) {
  int x = move->get_x();
  int y = move->get_y();
  char mark = move->get_symbol();

  if (x >= 0 && x < rows && y >= 0 && y < columns && board[x][y] == blank_symbol) {
    n_moves++;
    board[x][y] = toupper(mark);
    return true;
  }
  return false;
}

/**
 * @brief Iterates through Rows, Cols, and Diagonals to count 3-in-a-row patterns.
 * @return 1 if X wins count, -1 if O wins count, 0 if draw.
 */
int FxFTTT_board :: check_all_lines(){
    int sum_x=0, sum_o=0;

    auto all_equal = [&](char a, char b, char c) {
        return a == b && b == c && a != blank_symbol;
    };

    // Check rows
    for(int i=0;i<rows;i++){
        for(int j=0;j+2<columns;j++){
            if(all_equal(board[i][j], board[i][j+1], board[i][j+2] )){
                if ( board[i][j]== 'X'){sum_x++;}
                else if ( board[i][j] == 'O' ){sum_o++;}
            }
        }
    }
    // Check columns
    for(int j=0;j<columns;j++){
        for(int i=0;i+2<rows;i++){
            if(all_equal(board[i][j], board[i+1][j], board[i+2][j] )){
                if ( board[i][j] == 'X'){sum_x++;}
                else if ( board[i][j] == 'O' ){sum_o++;}
            }
        }
    }

    // Check main diagonal
    for (int i=0;i+2<rows;i++){
        for (int j=0;j+2<columns;j++){
            if(all_equal(board[i][j], board[i+1][j+1], board[i+2][j+2] )){
                if ( board[i][j] == 'X'){sum_x++;}
                else if ( board[i][j] == 'O' ){sum_o++;}
            }
        }
    }
    // Check anti diagonal
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
    return 0; // Default return
}

bool FxFTTT_board::is_draw(Player<char>* Player) {
  if (n_moves < 24) return false;
  int result = check_all_lines();
  return (result == 0);
}

bool FxFTTT_board::is_win(Player<char>* player) {
  if (n_moves < 24) return false;

  int result = check_all_lines();
  if (player->get_symbol() == 'X') return result == 1;
  if (player->get_symbol() == 'O') return result == -1;

  return false;
}

bool FxFTTT_board :: is_lose(Player<char>*player) {
  if (n_moves < 24){return false;}

  int result = check_all_lines();
  if (player->get_symbol() == 'X') return result == -1;
  if (player->get_symbol() == 'O') return result == 1;

  return false;
}

// --- UI Implementations ---

FxFTTT_ui ::FxFTTT_ui() : UI<char>("Welcome to 5x5 tic tac toe by Ali",3) {}

Player<char>* FxFTTT_ui::create_player(string& name, char symbol, PlayerType type ) {
  cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
       << " player: " << name << " (" << symbol << ")\n";
  return new Player<char>(name, symbol, type);
}

Move<char>* FxFTTT_ui::get_move(Player<char>* player) {
    int x, y;
    if (player->get_type() == PlayerType::HUMAN) {
        string s;
        cout << "\nPlease enter your move x and y (0 to 4): ";
        cin >> s;
        while (s.size() != 2 || !isdigit(s[0]) || !isdigit(s[1])) {
            cout << "Invalid input! Enter exactly two digits (ex: 02, 40): ";
            cin >> s;
        }
        x = s[0] - '0';
        y = s[1] - '0';
    }
    else {
        // AI Logic integration
        FxFTTT_board* board = dynamic_cast<FxFTTT_board*>(player->get_board_ptr());
        if (!board) {
            x = rand() % player->get_board_ptr()->get_rows();
            y = rand() % player->get_board_ptr()->get_columns();
        }
        else {
            std::pair<int,int> best_move = board->find_best_move();
            x = best_move.first;
            y = best_move.second;
            cout << "\nAI chooses move: " << x << y << endl;
        }
    }
    return new Move<char>(x, y, player->get_symbol());
}

bool FxFTTT_board::game_is_over(Player<char>* player) {
    if (n_moves == 24) return true;
    return false;
}

// --- AI Functions Implementation ---

std::vector<std::pair<int, int> > FxFTTT_board::get_available_moves() {
     vector<std::pair<int, int> > moves;
  for (int i=0 ; i<rows ;i++) {
         for (int j=0 ; j<columns ; j++) {
           if (board[i][j] == blank_symbol) {
                moves.push_back(std::make_pair(i,j));
           }
         }
       }
  return moves;
}

void FxFTTT_board::simulate_move( int row, int col, char symbol) {
  board[row][col] = symbol;
  n_moves++;
}

void FxFTTT_board::undo_move(int row, int col) {
  board[row][col] = blank_symbol;
  n_moves--;
}

void FxFTTT_board::set_ai_symbols(char ai, char opp) {
    ai_symbol = ai;
    opp_symbol = opp;
}

int FxFTTT_board::evaluate_board() {
    int ai_score = 0, opp_score = 0;
    auto all_equal = [&](char a, char b, char c) {
        return a == b && b == c && a != blank_symbol;
    };


    // Check rows
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j + 2 < columns; j++) {
            char c = board[i][j];
            if (all_equal(c, board[i][j+1], board[i][j+2])) {
                if (c == ai_symbol) ai_score++;
                else if (c == opp_symbol) opp_score++;
            }
        }
    }
    // Check columns
    for (int j = 0; j < columns; j++) {
        for (int i = 0; i + 2 < rows; i++) {
            char c = board[i][j];
            if (all_equal(c, board[i+1][j], board[i+2][j])) {
                if (c == ai_symbol) ai_score++;
                else if (c == opp_symbol) opp_score++;
            }
        }
    }
    // Check main diagonals
    for (int i = 0; i + 2 < rows; i++) {
        for (int j = 0; j + 2 < columns; j++) {
            char c = board[i][j];
            if (all_equal(c, board[i+1][j+1], board[i+2][j+2])) {
                if (c == ai_symbol) ai_score++;
                else if (c == opp_symbol) opp_score++;
            }
        }
    }
    // Check anti-diagonals
    for (int i = 0; i + 2 < rows; i++) {
        for (int j = 2; j < columns; j++) {
            char c = board[i][j];
            if (all_equal(c, board[i+1][j-1], board[i+2][j-2])) {
                if (c == ai_symbol) ai_score++;
                else if (c == opp_symbol) opp_score++;
            }
        }
    }

    return ai_score - opp_score;
}

int FxFTTT_board::minimax(int depth, bool is_max, int alpha, int beta) {
    auto moves = get_available_moves();

    // Stop if no moves left or depth limit reached
    if (depth == 0 || moves.empty()) {
        return evaluate_board();
    }

    if (is_max) {
        int best = -100000;
        for (auto& move : moves) {
            simulate_move(move.first, move.second, ai_symbol);
            int val = minimax(depth - 1, false, alpha, beta);
            undo_move(move.first, move.second);
            best = std::max(best, val);
            alpha = std::max(alpha, best);
            if (beta <= alpha) break;
        }
        return best;
    } else {
        int best = 100000;
        for (auto& move : moves) {
            simulate_move(move.first, move.second, opp_symbol);
            int val = minimax(depth - 1, true, alpha, beta);
            undo_move(move.first, move.second);
            best = std::min(best, val);
            beta = std::min(beta, best);
            if (beta <= alpha) break;
        }
        return best;
    }
}

std::pair<int,int> FxFTTT_board::find_best_move() {
    int best_val = -100000;
    std::pair<int,int> best_move = {-1,-1};
    int alpha = -100000;
    int beta = 100000;
    for (auto& move : get_available_moves()) {
        simulate_move(move.first, move.second, ai_symbol);
        int move_val = minimax(4, false, alpha, beta);
        undo_move(move.first, move.second);

        if (move_val > best_val) {
            best_val = move_val;
            best_move = move;
            alpha = std::max(alpha, best_val);
        }
    }
    return best_move;
}
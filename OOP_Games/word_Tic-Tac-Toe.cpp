//
// Created by Ali Tarek on 26/11/2025.
//

#include "word_Tic-Tac-Toe.h"
#include <iostream>
#include<fstream>
#include <cctype>

#include "Misere_Classes.h"
#include "XO_Classes.h"

using namespace std;






word_xo_board::word_xo_board() : Board(3, 3) {
    for (auto& row : board) {
        for (auto& cell : row) {
            cell = blank_symbol;
        }
    }
    load_dictionary();
}


bool word_xo_board::update_board(Move<char> *move) {
    int x = move->get_x();
    int y = move->get_y();
    char symbol = move->get_symbol();

    if (x < 0 || x > rows-1 || y < 0 || y > columns-1) {
        return false;
    }

    if (board[x][y] != blank_symbol) {
        return false;
    }

    board[x][y] = symbol;

    return true;
}


void word_xo_board::load_dictionary() {
    ifstream file("dic.txt") ;
        string word;
    while (file >> word) {
        dictionary.insert(word);
    }
    file.close();
}


bool word_xo_board::is_valid_word() {

    auto not_blank=[&](char a,char b , char c) {
        return a!=blank_symbol && b!=blank_symbol && c!=blank_symbol;
    };


    char ww[5];




    //checking rows
    for (int i = 0 ; i < rows ; i++ ) {
        for (int j = 0 ; j + 2 < columns ; j++ ) {
            if (not_blank(board[i][j], board[i][j+1], board[i][j + 2]) ) {
                ww[0] = board[i][j];
                ww[1] = board[i][j+1];
                ww[2] = board[i][j+2];
                ww[3] = '\0';
                if (dictionary.count(ww)>0) {
                    return true;
                }

            }

        }
    }


    // checking columns

    for (int j = 0 ; j < columns ; j++) {
        for (int i = 0 ; i + 2 < rows ; i++ ) {
            if (not_blank(board[i][j], board[i+1][j], board[i+2][j]) ) {
                ww[0] = board[i][j];
                ww[1] = board[i+1][j];
                ww[2] = board[i+2][j];
                ww[3] = '\0';
                if (dictionary.count(ww)>0) {
                    return true;
                }
            }
        }
    }
//check diagonal
    if (not_blank(board[0][0], board[1][1], board[2][2]) ) {
        ww[0] = board[0][0];
        ww[1] = board[1][1];
        ww[2] = board[2][2];
        ww[3] = '\0';
        if (dictionary.count(ww)>0) {
            return true;
        }

    }

//check anti_diagonal
    if (not_blank(board[0][2], board[1][1], board[2][0]) ) {
        ww[0] = board[0][2];
        ww[1] = board[1][1];
        ww[2] = board[2][0];
        ww[3] = '\0';
        if (dictionary.count(ww)>0) {
            return true;
        }
    }
return false;
}


bool word_xo_board::is_win(Player<char> *player) {
    if (is_valid_word()) {
        return true;
    }
    return false;
}


bool word_xo_board::is_lose(Player<char> *player) {
    return false;
}


bool word_xo_board::is_draw(Player<char> *player) {

    for (auto& row : board) {
        for (auto& cell : row) {
            if (cell == blank_symbol) {
                return false;
            }
        }
    }
    if (!(is_valid_word())) {
    return true;
    }
    return false;
}


bool word_xo_board::game_is_over(Player<char> *player) {
    if (is_win(player)) {
        return true;
    }
    if (is_draw(player)) {
        return true;
    }
    if (is_lose(player)) {
        return true;
    }
    return false;
}


void word_xo_board::simulate_move(int row, int col, char letter) {
    board[row][col] = letter;
    n_moves++;
}


void word_xo_board::undo_move(int row, int col, char) {
    board[row][col] = blank_symbol;
    n_moves--;
}



Move<char>* word_xo_board::find_best_move() {
    vector<Move<char>*> safe_moves;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            if (board[i][j] == blank_symbol) {
                for (char letter = 'A'; letter <= 'Z'; ++letter) {

                    simulate_move(i, j, letter);
                    if (is_valid_word()) {
                        undo_move(i, j, letter);
                        return new Move<char>(i, j, letter);
                    }


                    bool human_can_win = false;
                    for (int hi = 0; hi < rows; ++hi) {
                        for (int hj = 0; hj < columns; ++hj) {
                            if (board[hi][hj] == blank_symbol) {
                                for (char hletter = 'A'; hletter <= 'Z'; ++hletter) {
                                    simulate_move(hi, hj, hletter);
                                    if (is_valid_word()) human_can_win = true;
                                    undo_move(hi, hj, hletter);
                                }
                            }
                        }
                    }

                    undo_move(i, j, letter);

                    if (!human_can_win) {
                        safe_moves.push_back(new Move<char>(i, j, letter));
                    }
                }
            }
        }
    }


    if (!safe_moves.empty()) return safe_moves[0];


    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < columns; ++j)
            if (board[i][j] == blank_symbol)
                return new Move<char>(i, j, 'A');

    return nullptr;
}



word_xo_ui::word_xo_ui(word_xo_board*b) : UI<char>("welcome to word tic tac toe" , 3) , board(b) {}



Player<char>* word_xo_ui::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";
      return new Player<char>(name, symbol, type);
}



Move<char>* word_xo_ui::get_move(Player<char>* player) {
    if (player->get_type() != PlayerType::HUMAN) {

        int x, y;
        char letter;
        Move<char>* best_move =board->find_best_move();
        x = best_move->get_x();
        y = best_move->get_y();
        letter = best_move->get_symbol();
        delete best_move;

        cout << player->get_name() << " (COMPUTER) chooses letter " << letter
                << " at (" << x << "," << y << ")\n";

        return new Move<char>(x, y, letter);
    }

        int x, y;
        char letter;

        cout << player->get_name() << " enter letter: ";
        cin >> letter;
        letter = toupper(letter);

        cout << "Enter row: ";
        cin >> x;

        cout << "Enter column: ";
        cin >> y;

        return new Move<char>(x, y, letter);

}



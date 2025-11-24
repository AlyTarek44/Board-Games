#ifndef SUS_CLASSES_H
#define SUS_CLASSES_H

#include "BoardGame_Classes.h"
#include <vector>
#include <string>

using namespace std;

class SUS_Board : public Board<char> {
private:
    char blank_symbol = '.';
    int player1_score = 0;
    int player2_score = 0;
    char player1_letter, player2_letter;

    int check_and_score_sequences(int last_x, int last_y, char symbol);
    vector<vector<pair<int, int>>> get_all_lines();

public:
    SUS_Board();

    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

    void set_player_letters(char p1_letter, char p2_letter);
    int get_player1_score() const { return player1_score; }
    int get_player2_score() const { return player2_score; }
    char get_player1_letter() const { return player1_letter; }
    char get_player2_letter() const { return player2_letter; }

    vector<pair<int, int>> get_valid_moves();

    int evaluate_move(int x, int y, char symbol, int depth = 2);
};

class SUS_UI : public UI<char> {
private:
    SUS_Board* board_ptr;
    char player1_letter, player2_letter;

    void choose_letters();

public:
    SUS_UI(SUS_Board* board);
    ~SUS_UI() {}

    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* player) override;
    Player<char>** setup_players() override;

    void display_scores() const;
};

#endif
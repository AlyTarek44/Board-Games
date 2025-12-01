#ifndef DIAMONDTTT_CLASSES_H
#define DIAMONDTTT_CLASSES_H

#include "BoardGame_Classes.h"
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class Diamond_Board : public Board<char> {
public:
    Diamond_Board();
    virtual ~Diamond_Board() {}

    virtual bool update_board(Move<char>* move) override;
    virtual bool is_win(Player<char>* player) override;
    virtual bool is_lose(Player<char>* player) override;
    virtual bool is_draw(Player<char>* player) override;
    virtual bool game_is_over(Player<char>* player) override;

    bool valid_cell(int r, int c) const;
    vector<pair<int,int>> get_valid_positions() const;
    vector<pair<int,int>> get_empty_positions() const;

    bool make_temp_move(int r, int c, char symbol);
    void undo_temp_move(int r, int c);

    bool placement_creates_win(int r, int c, char symbol) const;

    static const char blank_symbol;

private:
    const int dr[4] = {0, 1, 1, 1};
    const int dc[4] = {1, 0, 1, -1};

    bool exists_contiguous_line_including(int r, int c, int len, int dir, char symbol) const;

    vector<pair<int,int>> valid_positions;
};

class Diamond_AIPlayer : public Player<char> {
public:
    Diamond_AIPlayer(const string& name, char symbol);
    virtual ~Diamond_AIPlayer() {}

    Move<char>* get_smart_move(Diamond_Board* board, char opponent_symbol);

private:

    int backtrack_minimax(Diamond_Board* board, char current_symbol, int alpha, int beta);

    string board_key(Diamond_Board* board) const;

    unordered_map<string,int> memo;

    pair<int,int> tiebreak_choose(const vector<pair<int,int>>& choices) const;
};

class Diamond_UI : public UI<char> {
    Diamond_Board* board_ptr;
public:
    Diamond_UI(Diamond_Board* board);
    virtual ~Diamond_UI() {}

    virtual Player<char>** setup_players() override;
    virtual Player<char>* create_player(string& name, char symbol, PlayerType type) override;

    virtual Move<char>* get_move(Player<char>* player) override;

    virtual void display_board_matrix(const vector<vector<char>>& matrix) const override;
};

#endif

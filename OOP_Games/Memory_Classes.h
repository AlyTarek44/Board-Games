#ifndef MEMORY_CLASSES_H
#define MEMORY_CLASSES_H

#include "BoardGame_Classes.h"
using namespace std;

class Memory_Board : public Board<char> {
private:
    char blank_symbol = '.';
    char hidden_symbol = '?';
    vector<vector<char>> real_board;

public:
    Memory_Board();

    bool update_board(Move<char>* move);

    bool is_win(Player<char>* player);

    bool is_lose(Player<char>*) { return false; };

    bool is_draw(Player<char>* player);

    bool game_is_over(Player<char>* player);

    vector<vector<char>> get_board_matrix() const;


    vector<vector<char>> get_move_display_board(int move_x, int move_y, char symbol) const;
};

class Memory_UI : public UI<char> {
private:
    Memory_Board* memory_board;
    void clear_screen();

public:
    Memory_UI(Memory_Board* board);

    ~Memory_UI() {};

    Player<char>* create_player(string& name, char symbol, PlayerType type);

    virtual Move<char>* get_move(Player<char>* player);


    virtual void display_board_matrix(const vector<vector<char>>& matrix) const;


    void run_game(Player<char>* players[2]);
};

#endif // MEMORY_CLASSES_H
#ifndef ULTIMATE_CLASSES_H
#define ULTIMATE_CLASSES_H

#include "BoardGame_Classes.h"
#include <string>
#include <vector>

using namespace std;



class Ultimate_Board : public Board<char> {
public:
    Ultimate_Board();
    virtual ~Ultimate_Board() {}

    virtual bool update_board(Move<char>* move) override;
    virtual bool is_win(Player<char>* player) override;
    virtual bool is_lose(Player<char>* player) override;
    virtual bool is_draw(Player<char>* player) override;
    virtual bool game_is_over(Player<char>* player) override;

    bool valid_subcell(int main_idx, int sub_idx) const;
    vector<pair<int,int>> get_allowed_moves() const;
    vector<pair<int,int>> get_all_empty_in_main(int main_idx) const;

    bool make_temp_move(int main_idx, int sub_idx, char symbol);
    void undo_temp_move(int main_idx, int sub_idx, char prev_claim);

    vector<char> get_main_claims() const { return main_claims; }
    int get_forced_main() const { return forced_main; }

    static const char blank_symbol;

private:
    vector<char> main_claims;
    int forced_main;


    char check_subboard_winner(int main_idx) const;
    bool subboard_full(int main_idx) const;
    char check_three_in_row_on_main(char who) const;

    inline void unpack_indices(int main_idx, int sub_idx, int &gr, int &gc) const {
        int mr = main_idx / 3, mc = main_idx % 3;
        int sr = sub_idx / 3, sc = sub_idx % 3;
        gr = mr * 3 + sr;
        gc = mc * 3 + sc;
    }
};

class Ultimate_AIPlayer : public Player<char> {
public:
    Ultimate_AIPlayer(const string& name, char symbol);
    virtual ~Ultimate_AIPlayer() {}

    Move<char>* get_simple_move(Ultimate_Board* board);

private:
    pair<int,int> pick_random(const vector<pair<int,int>>& v) const;
};

class Ultimate_UI : public UI<char> {
    Ultimate_Board* board_ptr;
public:
    Ultimate_UI(Ultimate_Board* board);
    virtual ~Ultimate_UI() {}

    virtual Player<char>** setup_players() override;
    virtual Player<char>* create_player(string& name, char symbol, PlayerType type) override;

    virtual Move<char>* get_move(Player<char>* player) override;

    void print_main_board_only() const;
    void print_full_board()const;
};

#endif

#define FOURINROW_CLASSES_H

#include "BoardGame_Classes.h"
#include <vector>
#include <string>

using namespace std;

class FourInRow_Board : public Board<char> {
private:
    char blank_symbol = '.';
    char player1_symbol = 'X';
    char player2_symbol = 'O';

    // Helper functions
    bool check_win_condition(int row, int col, char symbol);
    bool check_direction(int row, int col, int delta_row, int delta_col, char symbol);
    int get_next_empty_row(int col);
    int evaluate_position(char ai_symbol, char human_symbol);

public:
    // Constructor
    FourInRow_Board();

    // Override virtual functions from the base Board class
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

    // Additional methods for Four-in-a-row game
    bool is_valid_move(int col);
    vector<int> get_valid_moves();
    void make_temp_move(int col, char symbol);
    void undo_temp_move(int col);
    int evaluate_board(char ai_symbol, char human_symbol);
    bool would_win(int col, char symbol);
};


class FourInRow_AIPlayer : public Player<char> {
private:
    int backtracking_decision(FourInRow_Board* board, char ai_symbol, char human_symbol);

public:
    FourInRow_AIPlayer(string name, char symbol) : Player<char>(name, symbol, PlayerType::AI) {}
    Move<char>* get_smart_move(FourInRow_Board* board, char human_symbol);
};

class FourInRow_UI : public UI<char> {
private:
    FourInRow_Board* board_ptr;  // Pointer to access board-specific methods

public:
    FourInRow_UI(FourInRow_Board* board);

    ~FourInRow_UI() {}

    // Override virtual functions from the base UI class
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* player) override;

    // Override setup_players for Four-in-a-row game
    Player<char>** setup_players() override;

    // Get board pointer
    FourInRow_Board* get_board_ptr() const { return board_ptr; }

};

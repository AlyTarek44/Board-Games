#include "SUS_Classes.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;


SUS_Board::SUS_Board() : Board(3, 3) {
    for (auto& row : board) {
        for (auto& cell : row) {
            cell = blank_symbol;
        }
    }
}

void SUS_Board::set_player_letters(char p1_letter, char p2_letter) {
    player1_letter = p1_letter;
    player2_letter = p2_letter;
}

vector<vector<pair<int, int>>> SUS_Board::get_all_lines() {
    vector<vector<pair<int, int>>> lines;

    // Rows
    lines.push_back({{0, 0}, {0, 1}, {0, 2}});
    lines.push_back({{1, 0}, {1, 1}, {1, 2}});
    lines.push_back({{2, 0}, {2, 1}, {2, 2}});

    // Columns
    lines.push_back({{0, 0}, {1, 0}, {2, 0}});
    lines.push_back({{0, 1}, {1, 1}, {2, 1}});
    lines.push_back({{0, 2}, {1, 2}, {2, 2}});

    // Diagonals
    lines.push_back({{0, 0}, {1, 1}, {2, 2}});
    lines.push_back({{0, 2}, {1, 1}, {2, 0}});

    return lines;
}

int SUS_Board::check_and_score_sequences(int last_x, int last_y, char symbol) {
    int points_earned = 0;
    auto all_lines = get_all_lines();

    for (auto& line : all_lines) {
        int x1 = line[0].first, y1 = line[0].second;
        int x2 = line[1].first, y2 = line[1].second;
        int x3 = line[2].first, y3 = line[2].second;

        bool contains_last_move = (x1 == last_x && y1 == last_y) ||
                                 (x2 == last_x && y2 == last_y) ||
                                 (x3 == last_x && y3 == last_y);

        if (contains_last_move) {
            char a = board[x1][y1];
            char b = board[x2][y2];
            char c = board[x3][y3];

            if (a != blank_symbol && b != blank_symbol && c != blank_symbol) {
                if (a == 'S' && b == 'U' && c == 'S') {
                    points_earned++;
                }
            }
        }
    }

    return points_earned;
}

bool SUS_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char symbol = move->get_symbol();

    if (x < 0 || x >= rows || y < 0 || y >= columns) {
        cout << "Invalid move: Position (" << x << "," << y << ") is out of bounds.\n";
        return false;
    }

    if (board[x][y] != blank_symbol) {
        cout << "Invalid move: Cell (" << x << "," << y << ") is already taken.\n";
        return false;
    }

    board[x][y] = symbol;
    n_moves++;

    int points = check_and_score_sequences(x, y, symbol);
    if (points > 0) {
        if (symbol == player1_letter) {
            player1_score += points;
            cout << "Player 1 scored " << points << " point(s) for completing S-U-S sequence(s)!\n";
        } else {
            player2_score += points;
            cout << "Player 2 scored " << points << " point(s) for completing S-U-S sequence(s)!\n";
        }
    }

    return true;
}

bool SUS_Board::is_win(Player<char>* player) {
    if (n_moves == 9) {
        if (player->get_symbol() == player1_letter) {
            return player1_score > player2_score;
        } else {
            return player2_score > player1_score;
        }
    }
    return false;
}

bool SUS_Board::is_lose(Player<char>* player) {
    return false;
}

bool SUS_Board::is_draw(Player<char>* player) {
    if (n_moves == 9) {
        return player1_score == player2_score;
    }
    return false;
}

bool SUS_Board::game_is_over(Player<char>* player) {
    return n_moves == 9;
}

vector<pair<int, int>> SUS_Board::get_valid_moves() {
    vector<pair<int, int>> valid_moves;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (board[i][j] == blank_symbol) {
                valid_moves.push_back({i, j});
            }
        }
    }
    return valid_moves;
}

int SUS_Board::evaluate_move(int x, int y, char symbol, int depth) {
    char original_value = board[x][y];
    board[x][y] = symbol;

    int immediate_ai_score = check_and_score_sequences(x, y, symbol);

    char opponent_symbol = (symbol == player1_letter) ? player2_letter : player1_letter;
    int opponent_scoring_risk = 0;

    if (depth > 0) {
        auto valid_moves = get_valid_moves();

        for (auto& move : valid_moves) {
            int opp_x = move.first;
            int opp_y = move.second;

            char opp_original = board[opp_x][opp_y];
            board[opp_x][opp_y] = opponent_symbol;

            int opponent_score_potential = check_and_score_sequences(opp_x, opp_y, opponent_symbol);

            board[opp_x][opp_y] = opp_original;

            if (opponent_score_potential > 0) {
                opponent_scoring_risk = opponent_score_potential;
                break;
            }
        }
    }

    board[x][y] = original_value;

    if (immediate_ai_score > 0) {
        return 1000;
    } else if (opponent_scoring_risk > 0) {
        return 100;
    } else {
        return 500;
    }
}


SUS_UI::SUS_UI(SUS_Board* board) :
    UI<char>("Welcome to SUS Game! Form S-U-S sequences to score points.", 3),
    board_ptr(board) {
    srand(time(0));
}

void SUS_UI::choose_letters() {
    cout << "\n--- SUS Game Letter Selection ---\n";

    while (true) {
        cout << "Player 1, choose your letter (S or U): ";
        char choice;
        cin >> choice;
        choice = toupper(choice);

        if (choice == 'S' || choice == 'U') {
            player1_letter = choice;
            player2_letter = (choice == 'S') ? 'U' : 'S';
            break;
        } else {
            cout << "Invalid choice. Please enter 'S' or 'U'.\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }

    cout << "Player 1 will play with: " << player1_letter << "\n";
    cout << "Player 2 will play with: " << player2_letter << "\n";
    cout << "----------------------------------------\n";

    board_ptr->set_player_letters(player1_letter, player2_letter);
}

Player<char>** SUS_UI::setup_players() {
    Player<char>** players = new Player<char>*[2];
    vector<string> type_options = { "Human", "Computer" };

    choose_letters();

    cout << "\n--- SUS Game Player Setup ---\n";

    string name1 = get_player_name("Player 1 (" + string(1, player1_letter) + ")");
    PlayerType type1 = get_player_type_choice("Player 1 (" + string(1, player1_letter) + ")", type_options);
    players[0] = create_player(name1, player1_letter, type1);

    string name2 = get_player_name("Player 2 (" + string(1, player2_letter) + ")");
    PlayerType type2 = get_player_type_choice("Player 2 (" + string(1, player2_letter) + ")", type_options);
    players[1] = create_player(name2, player2_letter, type2);

    cout << "----------------------------------------\n";
    return players;
}

Player<char>* SUS_UI::create_player(string& name, char symbol, PlayerType type) {
    string type_str = (type == PlayerType::HUMAN) ? "Human" : "Computer";
    cout << "Created " << type_str << " player: " << name
         << " (Letter: " << symbol << ")\n";
    return new Player<char>(name, symbol, type);
}

void SUS_UI::display_scores() const {
    cout << "\n--- Current Scores ---\n";
    cout << "Player 1 (" << board_ptr->get_player1_letter() << "): "
         << board_ptr->get_player1_score() << " points\n";
    cout << "Player 2 (" << board_ptr->get_player2_letter() << "): "
         << board_ptr->get_player2_score() << " points\n";
    cout << "----------------------\n";
}

Move<char>* SUS_UI::get_move(Player<char>* player) {
    if (board_ptr->game_is_over(player)) {
        return nullptr;
    }

    int x, y;

    if (player->get_type() == PlayerType::HUMAN) {
        display_scores();

        cout << "\n---------------------------------\n";
        cout << player->get_name() << "'s turn (Letter: " << player->get_symbol() << ")\n";

        while (true) {
            cout << "Enter position (row col, 0-2): ";
            if (cin >> x >> y) {
                if (x >= 0 && x < 3 && y >= 0 && y < 3) {
                    if (board_ptr->get_board_matrix()[x][y] == '.') {
                        break;
                    } else {
                        cout << "Cell (" << x << ", " << y << ") is already occupied! Choose another.\n";
                    }
                } else {
                    cout << "Invalid position! Please enter numbers between 0-2.\n";
                }
            } else {
                cout << "Invalid input! Please enter two numbers.\n";
                cin.clear();
                cin.ignore(10000, '\n');
            }
        }
        cout << "---------------------------------\n";

        return new Move<char>(x, y, player->get_symbol());

    } else {

        auto valid_moves = board_ptr->get_valid_moves();

        if (valid_moves.empty()) {
            return new Move<char>(-1, -1, player->get_symbol());
        }

        int best_score = -1;
        pair<int, int> best_move = valid_moves[0];

        for (auto& move : valid_moves) {
            int x = move.first;
            int y = move.second;

            int score = board_ptr->evaluate_move(x, y, player->get_symbol(), 2);

            if (score > best_score) {
                best_score = score;
                best_move = move;
            }
        }

        x = best_move.first;
        y = best_move.second;

        cout << "Computer " << player->get_name() << " placed '" << player->get_symbol()
             << "' at (" << x << ", " << y << ")\n";

        return new Move<char>(x, y, player->get_symbol());
    }
}
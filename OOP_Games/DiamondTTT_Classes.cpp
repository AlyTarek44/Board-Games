#include "DiamondTTT_Classes.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <limits>

using namespace std;

const char Diamond_Board::blank_symbol = ' ';

// Diamond_Board implementation

Diamond_Board::Diamond_Board() : Board<char>(5, 5) {
    // initialize all cells to blank
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < columns; ++j)
            board[i][j] = blank_symbol;

    valid_positions.clear();
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < columns; ++c) {
            if (valid_cell(r, c)) valid_positions.emplace_back(r, c);
        }
    }
    n_moves = 0;
}

bool Diamond_Board::valid_cell(int r, int c) const {
    if (r < 0 || r >= rows || c < 0 || c >= columns) return false;
    return (abs(r - 2) + abs(c - 2) <= 2);
}

vector<pair<int,int>> Diamond_Board::get_valid_positions() const {
    return valid_positions;
}

vector<pair<int,int>> Diamond_Board::get_empty_positions() const {
    vector<pair<int,int>> empties;
    for (auto p : valid_positions) {
        if (board[p.first][p.second] == blank_symbol) empties.push_back(p);
    }
    return empties;
}

bool Diamond_Board::update_board(Move<char>* move) {
    int r = move->get_x();
    int c = move->get_y();
    char s = move->get_symbol();

    if (!valid_cell(r, c)) {
        cout << "Invalid move: (" << r << "," << c << ") is not a diamond cell.\n";
        return false;
    }
    if (board[r][c] != blank_symbol) {
        cout << "Invalid move: Cell occupied.\n";
        return false;
    }
    board[r][c] = s;
    ++n_moves;
    return true;
}

bool Diamond_Board::make_temp_move(int r, int c, char symbol) {
    if (!valid_cell(r, c)) return false;
    if (board[r][c] != blank_symbol) return false;
    board[r][c] = symbol;
    ++n_moves;
    return true;
}

void Diamond_Board::undo_temp_move(int r, int c) {
    if (!valid_cell(r, c)) return;
    if (board[r][c] != blank_symbol) {
        board[r][c] = blank_symbol;
        --n_moves;
    }
}

bool Diamond_Board::exists_contiguous_line_including(int r, int c, int len, int dir, char symbol) const {
    int rdr = dr[dir], dcr = dc[dir];

    for (int t = 0; t < len; ++t) {
        int sr = r - t * rdr;
        int sc = c - t * dcr;

        bool ok = true;
        for (int k = 0; k < len; ++k) {
            int rr = sr + k * rdr;
            int cc = sc + k * dcr;
            if (rr < 0 || rr >= rows || cc < 0 || cc >= columns) { ok = false; break; }
            if (!valid_cell(rr, cc)) { ok = false; break; }
            if (board[rr][cc] != symbol) { ok = false; break; }
        }
        if (ok) return true;
    }
    return false;
}

// After placing symbol at (r,c), check whether that move produced simultaneous 3 and 4 in different directions.
bool Diamond_Board::placement_creates_win(int r, int c, char symbol) const {
    bool has3[4] = {false, false, false, false};
    bool has4[4] = {false, false, false, false};

    for (int dir = 0; dir < 4; ++dir) {
        has3[dir] = exists_contiguous_line_including(r, c, 3, dir, symbol);
        has4[dir] = exists_contiguous_line_including(r, c, 4, dir, symbol);
    }

    // Need one dir3 and one dir4 with dir3 != dir4
    for (int d3 = 0; d3 < 4; ++d3) if (has3[d3]) {
        for (int d4 = 0; d4 < 4; ++d4) if (has4[d4] && d4 != d3) {
            return true;
        }
    }
    return false;
}

bool Diamond_Board::is_win(Player<char>* player) {
    if (!player) return false;
    char sym = player->get_symbol();
    for (auto p : valid_positions) {
        int r = p.first, c = p.second;
        if (board[r][c] == sym) {
            if (placement_creates_win(r, c, sym)) return true;
        }
    }
    return false;
}

bool Diamond_Board::is_lose(Player<char>* /*player*/) {
    return false;
}

bool Diamond_Board::is_draw(Player<char>* /*player*/) {
    // Draw if no empty valid cells and nobody has a valid win
    for (auto p : valid_positions) if (board[p.first][p.second] == blank_symbol) return false;
    // check both symbols
    Player<char> tmpX("tmpX",'X',PlayerType::HUMAN);
    Player<char> tmpO("tmpO",'O',PlayerType::HUMAN);
    if (!is_win(&tmpX) && !is_win(&tmpO)) return true;
    return false;
}

bool Diamond_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

// AI Player implementation (full backtracking)

Diamond_AIPlayer::Diamond_AIPlayer(const string& name, char symbol)
    : Player<char>(name, symbol, PlayerType::AI) {}

static string make_key_from_positions(const vector<pair<int,int>>& valid_positions, const vector<vector<char>>& board_mat) {
    // serialize only valid positions in given order
    string key;
    key.reserve(valid_positions.size());
    for (auto &p : valid_positions) {
        key.push_back(board_mat[p.first][p.second]);
    }
    return key;
}

string Diamond_AIPlayer::board_key(Diamond_Board* board) const {
    auto valid_pos = board->get_valid_positions();
    auto mat = board->get_board_matrix();
    return make_key_from_positions(valid_pos, mat);
}

Move<char>* Diamond_AIPlayer::get_smart_move(Diamond_Board* board, char opponent_symbol) {
    cout << "AI " << get_name() << " is thinking (optimized full backtracking)...\n";

    memo.clear();

    vector<pair<int,int>> empties = board->get_empty_positions();
    if (empties.empty()) {
        if (board->valid_cell(2,2) && board->get_board_matrix()[2][2] == Diamond_Board::blank_symbol)
            return new Move<char>(2,2,get_symbol());
        auto v = board->get_valid_positions();
        if (!v.empty()) return new Move<char>(v.front().first, v.front().second, get_symbol());
        return new Move<char>(0,0,get_symbol());
    }

    for (auto mv : empties) {
        int r = mv.first, c = mv.second;
        board->make_temp_move(r, c, get_symbol());
        if (board->placement_creates_win(r, c, get_symbol())) {
            board->undo_temp_move(r, c);
            cout << "AI found immediate winning move at (" << r << "," << c << ")\n";
            return new Move<char>(r, c, get_symbol());
        }
        board->undo_temp_move(r, c);
    }

    int best_score = -2;
    vector<pair<int,int>> best_moves;

    for (auto mv : empties) {
        int r = mv.first, c = mv.second;
        board->make_temp_move(r, c, get_symbol());

        if (board->placement_creates_win(r, c, get_symbol())) {
            board->undo_temp_move(r, c);
            best_moves.clear();
            best_moves.push_back(mv);
            best_score = 1;
            break;
        }

        int score = backtrack_minimax(board, opponent_symbol, -2, 2);

        board->undo_temp_move(r, c);

        if (score > best_score) {
            best_score = score;
            best_moves.clear();
            best_moves.push_back(mv);
        } else if (score == best_score) {
            best_moves.push_back(mv);
        }
    }

    if (best_moves.empty()) {
        if (board->valid_cell(2,2) && board->get_board_matrix()[2][2] == Diamond_Board::blank_symbol)
            return new Move<char>(2,2,get_symbol());
        auto v = board->get_empty_positions();
        if (!v.empty()) {
            return new Move<char>(v.front().first, v.front().second, get_symbol());
        }
        auto vp = board->get_valid_positions();
        return new Move<char>(vp.front().first, vp.front().second, get_symbol());
    }

    pair<int,int> chosen = tiebreak_choose(best_moves);
    cout << "AI " << get_name() << " selected (" << chosen.first << "," << chosen.second << ") with score " << best_score << "\n";
    return new Move<char>(chosen.first, chosen.second, get_symbol());
}

int Diamond_AIPlayer::backtrack_minimax(Diamond_Board* board, char current_symbol, int alpha, int beta) {
    string key = board_key(board) + current_symbol;
    auto it = memo.find(key);
    if (it != memo.end()) return it->second;

    // Terminal condition: if no empties => draw
    auto empties = board->get_empty_positions();
    if (empties.empty()) {
        memo[key] = 0;
        return 0;
    }

    bool is_ai_turn = (current_symbol == this->get_symbol());
    int best = is_ai_turn ? -2 : 2;

    vector<pair<int,int>> order = empties;
    stable_sort(order.begin(), order.end(), [&](const pair<int,int>& a, const pair<int,int>& b) {
        board->make_temp_move(a.first, a.second, current_symbol);
        bool a_win = board->placement_creates_win(a.first, a.second, current_symbol);
        board->undo_temp_move(a.first, a.second);

        board->make_temp_move(b.first, b.second, current_symbol);
        bool b_win = board->placement_creates_win(b.first, b.second, current_symbol);
        board->undo_temp_move(b.first, b.second);

        if (a_win != b_win) return a_win; // true first
        return false;
    });

    for (auto mv : order) {
        int r = mv.first, c = mv.second;
        board->make_temp_move(r, c, current_symbol);

        int outcome;
        if (board->placement_creates_win(r, c, current_symbol)) {
            // mover wins immediately
            outcome = (current_symbol == this->get_symbol()) ? 1 : -1;
            board->undo_temp_move(r, c);

            if (is_ai_turn) {
                best = max(best, outcome);
                alpha = max(alpha, best);
            } else {
                best = min(best, outcome);
                beta = min(beta, best);
            }

            memo[key] = best;
            if (alpha >= beta) {
                return best;
            }
            continue;
        }

        char next = (current_symbol == 'X') ? 'O' : 'X';
        int score = backtrack_minimax(board, next, alpha, beta);

        board->undo_temp_move(r, c);

        if (is_ai_turn) {
            if (score > best) best = score;
            alpha = max(alpha, best);
        } else {
            if (score < best) best = score;
            beta = min(beta, best);
        }

        if (alpha >= beta) break;
    }

    if (best == -2 || best == 2) best = 0; // safety fallback
    memo[key] = best;
    return best;
}

pair<int,int> Diamond_AIPlayer::tiebreak_choose(const vector<pair<int,int>>& choices) const {
    // prefer center (2,2), then the middle-of-edges of diamond (1,2),(2,1),(2,3),(3,2),
    pair<int,int> center = {2,2};
    for (auto p : choices) if (p == center) return p;

    vector<pair<int,int>> nexts = { {1,2}, {2,1}, {2,3}, {3,2} };
    for (auto np : nexts) {
        for (auto p : choices) if (p == np) return p;
    }

    return choices.front();
}

// Diamond_UI implementation

Diamond_UI::Diamond_UI(Diamond_Board* board) : UI<char>("Welcome to Diamond Tic-Tac-Toe (make a 3-line AND a 4-line simultaneously in different directions to win).", 1),
                                                board_ptr(board) {
    srand((unsigned)time(nullptr));
}

Player<char>** Diamond_UI::setup_players() {
    Player<char>** players = new Player<char>*[2];
    vector<string> type_options = { "Human", "Computer" };

    cout << "\n--- Diamond Tic-Tac-Toe Player Setup ---\n";

    string nameX = get_player_name("Player X");
    PlayerType typeX = get_player_type_choice("Player X", type_options);
    if (typeX == PlayerType::COMPUTER) players[0] = new Diamond_AIPlayer(nameX, 'X');
    else players[0] = create_player(nameX, 'X', typeX);

    string nameO = get_player_name("Player O");
    PlayerType typeO = get_player_type_choice("Player O", type_options);
    if (typeO == PlayerType::COMPUTER) players[1] = new Diamond_AIPlayer(nameO, 'O');
    else players[1] = create_player(nameO, 'O', typeO);

    cout << "----------------------------------------\n";
    return players;
}

Player<char>* Diamond_UI::create_player(string& name, char symbol, PlayerType type) {
    string type_str = (type == PlayerType::HUMAN) ? "Human" : "Computer";
    cout << "Created " << type_str << " player: " << name << " (Symbol: " << symbol << ")\n";
    return new Player<char>(name, symbol, type);
}

void print_diamond_board(const Diamond_Board* board) {
    const auto& mat = board->get_board_matrix();

    cout << "\n       COLUMNS\n";
    cout << "       0   1   2   3   4\n";
    cout << "    -----------------------\n";

    for (int r = 0; r < 5; r++) {
        cout << "R " << r << " | ";
        for (int c = 0; c < 5; c++) {

            if (!board->valid_cell(r,c)) {
                // invalid diamond area — print space
                cout << "    ";
            } else {
                // valid diamond cell — print dot or symbol
                char v = mat[r][c];
                if (v == ' ')
                    cout << " .  ";
                else
                    cout << " " << v << "  ";
            }
        }
        cout << "|\n";
    }
    cout << "    -----------------------\n\n";
}


Move<char>* Diamond_UI::get_move(Player<char>* player) {
    char sym = player->get_symbol();

    print_diamond_board(board_ptr);

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\n---------------------------------\n";
        cout << player->get_name() << "'s turn (Symbol: " << sym << ")\n";
        int r, c;

        while (true) {
            cout << "Enter move as: row col  (0-4 0-4). Only diamond cells are valid.\n";
            cout << "Move: ";

            if (!(cin >> r >> c)) {
                cout << "Invalid input. Try again.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            if (r < 0 || r > 4 || c < 0 || c > 4) {
                cout << "Out of range. Use 0..4 for both row and column.\n";
                continue;
            }
            if (!board_ptr->valid_cell(r,c)) {
                cout << "Not a valid diamond cell. Choose another.\n";
                continue;
            }
            if (board_ptr->get_board_matrix()[r][c] != Diamond_Board::blank_symbol) {
                cout << "Cell occupied. Choose another.\n";
                continue;
            }
            break;
        }

        cout << "---------------------------------\n";
        return new Move<char>(r, c, sym);

    } else {
        Diamond_AIPlayer* ai = dynamic_cast<Diamond_AIPlayer*>(player);
        char opp_sym = (sym == 'X') ? 'O' : 'X';
        return ai->get_smart_move(board_ptr, opp_sym);
    }
}




#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <unordered_map>
#include <tuple>

namespace rng = std::ranges;

// row, col, has number been seen
using SeenPosition = std::tuple<int, int, bool>;
// map: number -> row, col, has number been seen
using Board = std::unordered_map<int, SeenPosition>;

[[nodiscard]]
std::vector<int> read_numbers(const std::string &filename) {
    std::ifstream in{filename};
    // this should make use of RVO
    std::string number;
    std::vector<int> numbers{};
    while(std::getline(in, number, ',')) {
        numbers.push_back(std::stoi(number));
    }
    return numbers;
}

[[nodiscard]]
std::vector<Board> read_boards(const std::string &filename) {
    std::ifstream in{filename};

    std::vector<int> board_numbers{std::istream_iterator<int>{in},std::istream_iterator<int>{}};
    int num_boards = board_numbers.size() / 25;
    std::vector<Board> boards(num_boards);
    std::cout << "num boards = " << num_boards << "\n";
    for (int i = 0; i < board_numbers.size(); ++i) {
        int board_num = int(i / 25);
        int row = int(i / 5) - (5*board_num);
        int col = i % 5;
        boards[board_num][board_numbers[i]] = {row, col, false};
    }
    return boards;
}

int main() {
    std::vector<int> numbers = read_numbers("numbers");
    std::vector<Board> boards = read_boards("input");
    auto row_or_col_complete = [](const Board &board) {
        std::vector<int> row_count(5);
        std::vector<int> col_count(5);
        for(const auto& pair: board) {
            auto [row, col, seen] = pair.second;
            row_count[row] += seen;
            col_count[col] += seen;
        }
        return (rng::any_of(row_count, [](auto val){ return val == 5;})
        || rng::any_of(col_count, [](auto val){ return val == 5;}));
    };

    auto sum_unseen_numbers = [](const Board &board) {
        int sum = 0;
        for(const auto& pair: board) {
            auto [row, col, seen] = pair.second;
            if (!seen) {
                sum += pair.first;
            }
        }
        return sum;
    };

    std::vector<int> winning_scores{};
    std::unordered_map<int, bool> seen_boards;
    // TODO: clean this up with ranges
    for(int n: numbers) {
        int board_num = 0;
        for (auto &board: boards) {
            if (board.contains(n)) {
                std::get<2>(board[n]) = true;
            }
            if (row_or_col_complete(board)) {
                int score = n * sum_unseen_numbers(board);
                // score == 0 : board is already complete
                if ((score > 0) && (!seen_boards.contains(board_num))) {
                    winning_scores.push_back(n * sum_unseen_numbers(board));
                    seen_boards[board_num] = true;
                }
            }
            ++board_num;
        }
    }
    //part 1 - 23177
    std::cout << "first winning score = " << winning_scores[0] << "\n";
    //part 2
    std::cout << "last winning score = " << winning_scores.back() << "\n";
}

#include <algorithm>
#include <numeric>
#include <fstream>
#include <iterator>
#include <iostream>
#include <vector>
#include <unordered_map>

[[nodiscard]]
std::vector<std::string> read_file(const std::string &filename) {
    std::ifstream in{filename};
    return {std::istream_iterator<std::string>{in}, std::istream_iterator<std::string>{}};
}

[[nodiscard]]
int error_score(const std::string& line) {
    static std::unordered_map<char, char> tokens{{'(', ')'}, {'[', ']'}, {'{', '}'}, {'<', '>'}};
    static std::unordered_map<char, int> token_scores{{')', 3}, {']', 57}, {'}', 1197}, {'>', 25137}};
    std::vector<char> expected_closing;
    for(const char c: line) {
        if (tokens.contains(c)) {
            // chunk opened
            expected_closing.insert(expected_closing.begin(), tokens[c]);
        } else if (expected_closing[0] == c) {
            // chunk closed
            expected_closing.erase(expected_closing.begin());
        } else {
            //closed with wrong character
            return token_scores[c];
        }
    }
    return 0;
}


int main() {
    auto lines = read_file("input");
    // part 1 - syntax error score
    std::vector<int> scores(lines.size());
    std::transform(lines.begin(), lines.end(), scores.begin(), [](auto line) { return error_score(line);});
    // score = 265527
    std::cout << "syntax error score = " << std::accumulate(scores.begin(), scores.end(), 0) << "\n";
    return 0;
}

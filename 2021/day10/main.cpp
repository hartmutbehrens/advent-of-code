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
auto completion_score(const std::vector<char> tokens) {
    static std::unordered_map<char, long long> token_value{{')', 1LL}, {']', 2LL}, {'}', 3LL}, {'>', 4LL}};
    return std::accumulate(tokens.begin(), tokens.end(), 0LL, [](long long total, char t) {
        return total = (total * 5 + token_value[t]);
    });
}

[[nodiscard]]
auto line_score(const std::string& line, bool score_corrupt = true) {
    static std::unordered_map<char, char> tokens{{'(', ')'}, {'[', ']'}, {'{', '}'}, {'<', '>'}};
    static std::unordered_map<char, int> token_value{{')', 3}, {']', 57}, {'}', 1197}, {'>', 25137}};
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
            return score_corrupt ? token_value[c] : 0LL;
        }
    }
    return !score_corrupt ? completion_score(expected_closing) : 0LL;
}


int main() {
    auto lines = read_file("input");
    // part 1 - syntax error score
    std::vector<long long> corrupt_scores(lines.size());
    std::transform(lines.begin(), lines.end(), corrupt_scores.begin(), [](auto line) { return line_score(line);});
    // score = 265527
    std::cout << "syntax error score = " << std::accumulate(corrupt_scores.begin(), corrupt_scores.end(), 0) << "\n";
    // part 2 - incomplete scores
    std::vector<long> incomplete_scores(lines.size());
    std::transform(lines.begin(), lines.end(), incomplete_scores.begin(), [](auto line) { return line_score(line, false);});
    std::sort(incomplete_scores.rbegin(), incomplete_scores.rend());
    incomplete_scores.erase(std::remove_if(
                                    incomplete_scores.begin(),
                                    incomplete_scores.end(),
                                    [](auto v) {return v == 0;}),
                            incomplete_scores.end());
    std::cout << "incomplete score = " << incomplete_scores[incomplete_scores.size()/2] << "\n";
    return 0;
}

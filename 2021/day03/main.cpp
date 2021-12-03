#include <algorithm>
#include <numeric>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <ranges>

namespace rng = std::ranges;

[[nodiscard]]
std::vector<std::string> read_file(const std::string &filename) {
    std::ifstream in{filename};
    // this should make use of RVO
    return {std::istream_iterator<std::string>{in},std::istream_iterator<std::string>{}};
}

int main() {
    std::vector<std::string> diagnostics = read_file("input");
    std::array<int, 12> one_count{};
    auto count_ones = [&one_count](const std::string& line) {
        int i = 0;
        for (const char c: line) {
            one_count[i] += c - '0';
            ++i;
        }
    };

    auto to_string = [](auto view) {
        std::string r;
        for(auto i: view) {
            r += std::to_string(i);
        }
        return r;
    };
    rng::for_each(diagnostics, count_ones);
    auto gamma = one_count | rng::views::transform([diagnostics](int n) { return n > 0.5 * diagnostics.size(); });
    auto epsilon = one_count | rng::views::transform([diagnostics](int n) { return n < 0.5 * diagnostics.size(); });
    std::cout << "part 1 = " << std::stoi(to_string(gamma), 0, 2) * std::stoi(to_string(epsilon), 0, 2) << "\n";
}

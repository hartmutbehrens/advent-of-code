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
std::vector<int> read_file(const std::string &filename) {
    std::ifstream in{filename};
    // this should make use of RVO
    return {std::istream_iterator<int>{in},std::istream_iterator<int>{}};
}

int main() {
    std::vector<int> numbers = read_file("input");
    int current_depth = numbers[0];
    auto did_increase = [&current_depth](int new_depth) {
        int delta = new_depth - current_depth;
        current_depth = new_depth;
        return delta > 0;
    };
    // part 1
   auto increases = numbers | rng::views::transform(did_increase);
   std::cout << "increases = " << std::accumulate(increases.begin(), increases.end(), 0);
}

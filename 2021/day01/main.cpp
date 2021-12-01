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
    auto depth_increased = [&current_depth](int new_depth) -> bool {
        int delta = new_depth - current_depth;
        current_depth = new_depth;
        return delta > 0;
    };
    // part 1 - 1195
   auto part1_increases = numbers | rng::views::transform(depth_increased);
   std::cout << "increases (part1) = " << std::accumulate(part1_increases.begin(), part1_increases.end(), 0) << "\n";
   // part 2
   int index = 0;
   auto sliding_depth_increased = [&index, &numbers](int depth) {
       int delta = 0;
       if (index > 2) {
           auto prev_window = rng::views::counted(numbers.begin() + index - 3, 3);
           auto window = rng::views::counted(numbers.begin() + index - 2, 3);
           auto prev_depth = std::accumulate(prev_window.begin(), prev_window.end(), 0);
           auto depth = std::accumulate(window.begin(), window.end(), 0);
           delta = depth - prev_depth;
       }
       index++;
       return delta > 0;
   };
    auto part2_increases = numbers | rng::views::transform(sliding_depth_increased);
    std::cout << "increases (part2) = " << std::accumulate(part2_increases.begin(), part2_increases.end(), 0);
}

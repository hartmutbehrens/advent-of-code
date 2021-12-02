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
    // old solution for part 1 - superseded by sliding window algo
//    int current_depth = numbers[0];
//    auto depth_increased = [&current_depth](int new_depth) -> bool {
//        int delta = new_depth - current_depth;
//        current_depth = new_depth;
//        return delta > 0;
//    };
//    auto part1_increases = numbers | rng::views::transform(depth_increased);
//    std::cout << "increases (part1) = " << std::accumulate(part1_increases.begin(), part1_increases.end(), 0) << "\n";
    int window_size = 1;
    auto sliding_depth_increased = [&numbers, &window_size](int index) -> bool {
        int delta = 0;
        int prev_start = index - window_size;
        if (index > (window_size - 1)) {
            // previous over-engineered solution
            // auto prev_window = rng::views::counted(numbers.begin() + prev_start, window_size);
            // auto window = rng::views::counted(numbers.begin() + prev_start + 1, window_size);
            // auto prev_depth = std::accumulate(prev_window.begin(), prev_window.end(), 0);
            // auto cur_depth = std::accumulate(window.begin(), window.end(), 0);
            // delta = cur_depth - prev_depth
            
            // someone pointed out that for the sliding window where you compare A+B+C < B+C+D
            // you can simply reduce this to A < D. Same reasoning for B+C+D < C+D+E, etc.
            delta = numbers[index] - numbers[prev_start];
        }
        return delta > 0;
    };
    // part 1 - 1195
    auto indices = rng::views::iota(0, int(numbers.size()));
    auto part1_increases = indices | rng::views::transform(sliding_depth_increased);
    std::cout << "increases (part1) = " << std::accumulate(part1_increases.begin(), part1_increases.end(), 0) << "\n";
    // part 2 - 1235
    window_size = 3;
    auto part2_increases = indices | rng::views::transform(sliding_depth_increased);
    std::cout << "increases (part2) = " << std::accumulate(part2_increases.begin(), part2_increases.end(), 0);
}

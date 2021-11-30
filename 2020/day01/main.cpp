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
    // lambda to check if a number exists within the provided input
    auto has_complement_number = [numbers](int n) {
        return rng::find(numbers, 2020 - n) != rng::end(numbers);
    };
    // part 1 - find two numbers that sum to 2020
    auto two_numbers = numbers | rng::views::filter(has_complement_number);
    std::cout << std::accumulate(two_numbers.begin(), two_numbers.end(), 1, std::multiplies<int>()) << "\n";
    // iterate over the input numbers and check whether the sum of x + the iterated number
    // are present in the original input
    auto is_sum_a_complement = [numbers, &has_complement_number](int x) {
        return rng::any_of(numbers, [x, &has_complement_number](int y){ return has_complement_number(x+y);});
    };
    // part 2 - find three numbers that sum to 2020
    auto three_numbers = numbers | rng::views::filter(is_sum_a_complement);
    std::cout << std::accumulate(three_numbers.begin(), three_numbers.end(), 1, std::multiplies<int>());
}

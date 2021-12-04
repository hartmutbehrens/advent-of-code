#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <bitset>
#include <functional>

namespace rng = std::ranges;

[[nodiscard]]
std::vector<std::string> read_file(const std::string &filename) {
    std::ifstream in{filename};
    // this should make use of RVO
    return {std::istream_iterator<std::string>{in},std::istream_iterator<std::string>{}};
}

int main() {
    std::vector<std::string> diagnostics = read_file("input");
    std::vector<std::bitset<12>> bitsets(diagnostics.size());
    std::transform(diagnostics.begin(), diagnostics.end(),
                   bitsets.begin(), [](const std::string& s) { return std::bitset<12>{s};});
    std::vector<int> sum_at_each_pos(12);
    auto count_ones = [&sum_at_each_pos](const auto& bitset){
        for(int i=0; i<12; ++i) {
            sum_at_each_pos[i] += bitset[i];
        }
    };
    rng::for_each(bitsets, count_ones);
    std::bitset<12> gamma{};
    for (int i=0; i<12; ++i) {
        gamma[i] = sum_at_each_pos[i] > 0.5 * diagnostics.size();
    }
    // epsilon is just inverse of gamma
    std::bitset<12> epsilon = ~gamma;
    // part 1 = 2035764
    std::cout << "part 1 = " << gamma.to_ulong() * epsilon.to_ulong() << "\n";

    auto get_rating = [](std::vector<std::bitset<12>> bitsets, auto compare) {
        int bit_pos = 11; // start at MSB
        while (bitsets.size() > 1) {
            int sum_at_pos = 0;
            for (auto b: bitsets) {
                sum_at_pos += b[bit_pos];
            }
            bool bit_to_match = sum_at_pos >= 0.5 * bitsets.size();
            std::erase_if(bitsets, [bit_pos, bit_to_match, compare](auto b) { return compare(b[bit_pos], bit_to_match); });
            --bit_pos;
        }
        return bitsets[0];
    };
    std::bitset<12> oxygen_rating = get_rating(bitsets, std::not_equal_to<>());
    std::bitset<12> scrubber_rating = get_rating(bitsets, std::equal_to<>());
    // part 2 - 2817661
    std::cout << "part 2 = " << oxygen_rating.to_ulong() * scrubber_rating.to_ulong() << "\n";
}

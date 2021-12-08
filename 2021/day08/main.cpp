#include <array>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Entry {
    std::array<std::string, 10> patterns;
    std::array<std::string,4> output;

    friend std::istream& operator>>(std::istream& is, Entry& line) {
        char discard;
        std::string word;
        for (int i=0; i<10; ++i) {
            is >> line.patterns[i];
        }
        is >> discard;
        for (int i=0; i<4; ++i) {
            is >> line.output[i];
        }
        return is;
    }
};


[[nodiscard]]
std::vector<Entry> read_input(const std::string &filename) {
    std::ifstream in{filename};
    // this should make use of RVO
    return {std::istream_iterator<Entry>{in}, std::istream_iterator<Entry>{}};
}

int main() {
    auto entries = read_input("input");
    int num_of_1 = 0;
    int num_of_4 = 0;
    int num_of_7 = 0;
    int num_of_8 = 0;
    for(const auto& entry: entries) {
        num_of_1 += std::accumulate(entry.output.begin(), entry.output.end(), 0, [](int sum, const std::string& value) { return sum += value.length() == 2; });
        num_of_4 += std::accumulate(entry.output.begin(), entry.output.end(), 0, [](int sum, const std::string& value) { return sum += value.length() == 4; });
        num_of_7 += std::accumulate(entry.output.begin(), entry.output.end(), 0, [](int sum, const std::string& value) { return sum += value.length() == 3; });
        num_of_8 += std::accumulate(entry.output.begin(), entry.output.end(), 0, [](int sum, const std::string& value) { return sum += value.length() == 7; });
    }
    std::cout << "part 1 = " << num_of_1 + num_of_4 + num_of_7 + num_of_8 << "\n";
    return 0;
}

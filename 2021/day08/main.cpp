#include <array>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

[[nodiscard]]
constexpr bool contains_all(const std::string& str, const std::string& find) {
    bool contains = true;
    for (auto s: find) {
        contains = contains && str.find(s) != std::string::npos;
    }
    return contains;
}


struct Entry {
    std::array<std::string, 10> patterns{""};
    std::array<std::string, 4> outputs{""};
    std::array<std::string, 10> numbers{""};

    friend std::istream& operator>>(std::istream& is, Entry& line) {
        char discard;
        std::string word;
        std::vector<std::string> zero_six_nine{};
        std::vector<std::string> two_three_five{};
        for (int i=0; i<10; ++i) {
            is >> line.patterns[i];
            std::sort(line.patterns[i].begin(), line.patterns[i].end());
            switch (line.patterns[i].length()) {
                case 2: line.numbers[1] = line.patterns[i]; break;
                case 3: line.numbers[7] = line.patterns[i]; break;
                case 4: line.numbers[4] = line.patterns[i]; break;
                case 5: two_three_five.push_back(line.patterns[i]); break;
                case 6: zero_six_nine.push_back(line.patterns[i]); break;
                case 7: line.numbers[8] = line.patterns[i]; break;
            };
        }
        is >> discard;
        // 3 is the only number in 2,3,5 that has all segments common with 1
        line.numbers[3] = *std::find_if(two_three_five.begin(), two_three_five.end(), [&line](const std::string& s) { return contains_all(s, line.numbers[1]); });
        std::erase(two_three_five, line.numbers[3]);
        // 9 has all segments common with three
        line.numbers[9] = *std::find_if(zero_six_nine.begin(), zero_six_nine.end(), [&line](const std::string& s) { return contains_all(s, line.numbers[3]); });
        std::erase(zero_six_nine, line.numbers[9]);
        // between zero and six, zero has all segments in common with one
        line.numbers[0] = *std::find_if(zero_six_nine.begin(), zero_six_nine.end(), [&line](const std::string& s) { return contains_all(s, line.numbers[1]); });
        std::erase(zero_six_nine, line.numbers[0]);
        // only six remains
        line.numbers[6] = zero_six_nine[0];
        // five has all numbers in common with 9
        line.numbers[5] = *std::find_if(two_three_five.begin(), two_three_five.end(), [&line](const std::string& s) { return contains_all(line.numbers[9], s); });
        std::erase(two_three_five, line.numbers[5]);
        line.numbers[2] = two_three_five[0];
        // get the output
        for (int i=0; i<4; ++i) {
            is >> line.outputs[i];
            std::sort(line.outputs[i].begin(), line.outputs[i].end());
        }
        return is;
    }
};

class sum_if_length {
public:
    explicit sum_if_length(int length) : m_length(length)
    { }
    int operator()(int sum, const Entry& entry) const {
        return sum + std::accumulate(entry.outputs.begin(), entry.outputs.end(), 0,
                                     [&](int a, const std::string& value) { return a += value.length() == m_length; });
    }
private:
    int m_length;
};


[[nodiscard]]
std::vector<Entry> read_input(const std::string &filename) {
    std::ifstream in{filename};
    // this should make use of RVO
    return {std::istream_iterator<Entry>{in}, std::istream_iterator<Entry>{}};
}

int main() {
    auto entries = read_input("input");
    int num_of_1 = std::accumulate(entries.begin(), entries.end(), 0, sum_if_length(2));
    int num_of_4 = std::accumulate(entries.begin(), entries.end(), 0, sum_if_length(4));
    int num_of_7 = std::accumulate(entries.begin(), entries.end(), 0, sum_if_length(3));
    int num_of_8 = std::accumulate(entries.begin(), entries.end(), 0, sum_if_length(7));;
    std::cout << "part 1 = " << num_of_1 + num_of_4 + num_of_7 + num_of_8 << "\n";;
    int sum = 0;
    for(const Entry& entry: entries) {
        std::string num;
        for(auto output: entry.outputs) {
            int x = std::distance(entry.numbers.begin(), std::find(entry.numbers.begin(), entry.numbers.end(), output));
            num += std::to_string(x);
        }
        sum += std::stoi(num);
    }
    std::cout << "part 2 = " << sum << "\n";;
    return 0;
}

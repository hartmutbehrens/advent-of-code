#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <unordered_map>

namespace rng = std::ranges;

using Vents = std::unordered_map<std::string, int>;

struct Point {
    int x;
    int y;
};

struct Line {
    Point from;
    Point to;
    bool diagonal;

    friend std::istream& operator>>(std::istream& is, Line& line) {
        char discard;
        is >> line.from.x >> discard >> line.from.y >> discard >> discard >> line.to.x >> discard >> line.to.y;
        line.diagonal = !((line.from.x == line.to.x) or (line.from.y == line.to.y));
        return is;
    }
};

[[nodiscard]]
std::vector<Line> read_input(const std::string &filename) {
    std::ifstream in{filename};
    // this should make use of RVO
    return {std::istream_iterator<Line>{in},std::istream_iterator<Line>{}};
}

void iota(auto& v, int n, int inc) {
    std::ranges::generate(v, [&n, &inc] () mutable { return n += inc; });
}

int main() {
    std::vector<Line> lines = read_input("input");
    auto plot_vents = [](Vents& vents_map, const Line &line, bool exclude_diagonal = true) {
        // only consider horizontal or vertical lines for now
        if (exclude_diagonal && line.diagonal) {
            return;
        }
        int x_delta = line.to.x - line.from.x;
        int x_inc = x_delta == 0 ? 0 : x_delta > 0 ? 1 : -1;
        int y_delta = line.to.y - line.from.y;
        int y_inc = y_delta == 0 ? 0 :  y_delta > 0 ? 1 : -1;
        // this works only because we have straight lines and diagonal
        int num_steps = abs(x_delta) == 0 ? abs(y_delta) : abs(x_delta);
        std::vector<int> x_numbers(num_steps+1);
        std::vector<int> y_numbers(num_steps+1);
        iota(x_numbers, line.from.x - x_inc, x_inc);
        iota(y_numbers, line.from.y - y_inc, y_inc);
        for(int i = 0; i<num_steps +1; ++i) {
            std::string key = std::to_string(x_numbers[i]) + "," +  std::to_string(y_numbers[i]);
            vents_map[key] += 1;
        }
    };
    auto overlap_count = [](const Vents& vents_map) {
        int sum = 0;
        for(const auto &pair: vents_map) {
            if (pair.second > 1) {
                ++sum;
            }
        }
        return sum;
    };
    Vents simple_vents_map{};
    for(const auto& line: lines) {
        plot_vents(simple_vents_map, line);
    }
    // part 1 - 5280
    std::cout << "part 1 overlap = " << overlap_count(simple_vents_map) << "\n";
    // part 2 -
    Vents proper_vents_map{};
    for(const auto& line: lines) {
        plot_vents(proper_vents_map, line, false);
    }
    std::cout << "part 2 overlap = " << overlap_count(proper_vents_map) << "\n";
    return 0;
}

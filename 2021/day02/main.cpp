#include <algorithm>
#include <numeric>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <ranges>

namespace rng = std::ranges;

struct Navigation {
    std::string direction;
    int amount;

    friend std::istream& operator>>(std::istream& is, Navigation& obj) {
        is >> obj.direction >> obj.amount;
        return is;
    }
};

struct Heading {
    int horizontal = 0;
    int depth = 0;
};

[[nodiscard]]
std::vector<Navigation> read_file(const std::string &filename) {
    std::ifstream in{filename};
    // this should make use of RVO
    return {std::istream_iterator<Navigation>{in},std::istream_iterator<Navigation>{}};
}

int main() {
    std::vector<Navigation> directions = read_file("input");
    Heading heading;
    auto adjust_heading = [&heading](Navigation n) {
        if (n.direction == "forward") {
            heading.horizontal += n.amount;
        } else if (n.direction == "down") {
            heading.depth += n.amount;
        } else if (n.direction == "up") {
            heading.depth -= n.amount;
        }
    };
    rng::for_each(directions, adjust_heading);
    std::cout << "result (part 1) = " << heading.horizontal * heading.depth << "\n";
}

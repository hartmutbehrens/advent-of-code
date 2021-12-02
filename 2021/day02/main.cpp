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
    int aim = 0;
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
    auto navigate = [&heading](const Navigation& n) {
        if (n.direction == "forward") {
            heading.horizontal += n.amount;
        } else if (n.direction == "down") {
            heading.depth += n.amount;
        } else if (n.direction == "up") {
            heading.depth -= n.amount;
        }
    };
    // part 1 - 1989014
    rng::for_each(directions, navigate);
    std::cout << "result (part 1) = " << heading.horizontal * heading.depth << "\n";
    // part 2
    heading = {.horizontal = 0, .depth = 0};
    auto navigate_with_aim = [&heading](const Navigation& n) {
        if (n.direction == "down") {
            heading.aim += n.amount;
        } else if (n.direction == "up") {
            heading.aim -= n.amount;
        } else if (n.direction == "forward") {
            heading.horizontal += n.amount;
            heading.depth += heading.aim * n.amount;
        }
    };
    rng::for_each(directions, navigate_with_aim);
    std::cout << "result (part 2) = " << heading.horizontal * heading.depth << "\n";
}

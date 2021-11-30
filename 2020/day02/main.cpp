#include <algorithm>
#include <numeric>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <ranges>

namespace rng = std::ranges;

struct PasswordAndPolicy {
    int min;
    int max;
    char letter;
    std::string password;

    friend std::istream& operator>>(std::istream& is, PasswordAndPolicy& obj) {
        char ignore;
        is >> obj.min >> ignore >> obj.max >> obj.letter >> ignore >> obj.password;
        return is;
    }
};

[[nodiscard]]
std::vector<PasswordAndPolicy> read_file(const std::string &filename) {
    std::ifstream in{filename};
    // this should make use of RVO
    return {std::istream_iterator<PasswordAndPolicy>{in},
            std::istream_iterator<PasswordAndPolicy>{}};;
}

int main() {
    auto records = read_file("input");
    auto is_valid = [](PasswordAndPolicy r) {
        auto count = rng::count_if(r.password, [&r](char c) {return c == r.letter;});
        return (count >= r.min) and (count <= r.max);
    };
    // part 1 - number of valid passwords
    auto valid = records | rng::views::transform(is_valid);
    std::cout << std::accumulate(valid.begin(), valid.end(), 0) << "\n";
}

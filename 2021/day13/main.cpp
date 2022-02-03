#include <cctype>
#include <fstream>
#include <iterator>
#include <iostream>
#include <vector>
#include <regex>
#include <unordered_map>
#include <set>

using Coord = std::pair<int, int>;
using Fold = std::pair<char, int>;

struct Paper {
    std::set<Coord> dots;
    std::vector<Fold> folds;
};

[[nodiscard]]
std::vector<std::string> split(const std::string& s, const std::string& delimiter) {
    std::regex reg("[" + delimiter + "]");
    std::sregex_token_iterator iter(s.begin(), s.end(), reg, -1);
    std::sregex_token_iterator end;
    return std::vector<std::string>{iter, end};
}

[[nodiscard]]
auto read_file(const std::string &filename) {
    Paper p;
    std::ifstream in{filename};
    for(auto it = std::istream_iterator<std::string>{in};  it != std::istream_iterator<std::string>{}; ++it) {
        if (it->find(",") != std::string::npos) {
            auto parts = split(*it, ",");
            p.dots.emplace(std::stoi(parts[0]), std::stoi(parts[1]));
        } else if (it->find("=") != std::string::npos) {
            auto parts = split(*it, "=");
            p.folds.emplace_back(parts[0].back(), std::stoi(parts[1]));
        } else {
            continue;
        }
    }
    return p;
}

void fold_x(int x, std::set<Coord>& dots) {
    std::set<Coord> folded;
    for (auto dot: dots) {
        if (dot.first > x) {
            folded.emplace(x - (dot.first - x), dot.second);
        } else {
            folded.emplace(dot);
        }
    }
    dots.swap(folded);
}

void fold_y(int y, std::set<Coord>& dots) {
    std::set<Coord> folded;
    for(auto dot: dots) {
        if (dot.second > y) {
            folded.emplace(dot.first, y - (dot.second - y));
        } else {
            folded.emplace(dot);
        }
    }
    dots.swap(folded);
}

void fold(const Fold& fold, std::set<Coord>& dots) {
    if (fold.first == 'x') {
        fold_x(fold.second, dots);
    } else {
        fold_y(fold.second, dots);
    }
}



int main() {
    auto paper = read_file("input");
    fold(paper.folds[0], paper.dots);
    std::cout << "number dots = " << paper.dots.size() << "\n";
    return 0;
}

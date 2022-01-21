#include <cctype>
#include <fstream>
#include <iterator>
#include <iostream>
#include <vector>
#include <regex>
#include <unordered_map>
#include <set>

[[nodiscard]]
std::vector<std::string> split(const std::string& s, const std::string& delimiter) {
    std::regex reg("[" + delimiter + "]");
    std::sregex_token_iterator iter(s.begin(), s.end(), reg, -1);
    std::sregex_token_iterator end;
    return std::vector<std::string>{iter, end};
}

[[nodiscard]]
auto read_file(const std::string &filename) {
    std::unordered_map<std::string, std::vector<std::string>> caves{};
    std::ifstream in{filename};
    for(auto it = std::istream_iterator<std::string>{in};  it != std::istream_iterator<std::string>{}; ++it) {
        auto edge = split(*it, "-");
        caves[edge[0]].push_back(edge[1]);
        caves[edge[1]].push_back(edge[0]);
    }
    return caves;
}

bool is_small_cave(const std::string& s) {
    return std::islower(s[0]);
}

void search(const std::string& cave, auto &cave_system, auto &paths, std::set<std::string> visited = {}, std::vector<std::string> path = {}) {
    path.push_back(cave);
    if (cave == "end") {
        paths.push_back(path);
        return;
    }
    visited.insert(cave);
    for (const auto &next: cave_system[cave]) {
        if (is_small_cave(next)) {
            if (!visited.contains(next)) {
                search(next, cave_system, paths, visited, path);
            }
        } else {
            search(next, cave_system, paths, visited, path);
        }
    }
}


int main() {
    auto cave_system = read_file("input");
    std::vector<std::vector<std::string>> paths;
    search("start", cave_system, paths);
    std::cout << "number paths = " << paths.size() << "\n";
    return 0;
}

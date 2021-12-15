#include <array>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

[[nodiscard]]
std::vector<int> read_file(const std::string &filename) {
    std::ifstream in{filename};
    std::vector<int> heights{};
    for(auto it = std::istream_iterator<char>{in};  it != std::istream_iterator<char>{}; ++it) {
        // subtract the char value of '0' and promote to int
        heights.push_back(*it - '0');
    }
    return heights;
}


int main() {
    auto heights = read_file("input");
    auto is_lowest = [&heights](int pos, int grid_length = 100) {
        int col = pos % grid_length;
        int row = int(pos / grid_length);
        bool last_row = pos + grid_length > heights.size();
        int up = row > 0 ? heights[grid_length * (row - 1) + col] : 10;
        int down = last_row ? 10: heights[grid_length * (row + 1) + col];
        int left = col > 0 ? heights[pos - 1] : 10;
        int right = col < grid_length - 1 ? heights[pos + 1] : 10;
        return (heights[pos] < up) && (heights[pos] < down) && (heights[pos] < left) && (heights[pos] < right);
    };
    std::vector<int> lowest;
    for(int i=0; i < heights.size(); ++i) {
        if (is_lowest(i)) {
            lowest.push_back(i);
        }
    }
    // part 1 = 456
    std::cout << "part 1 = " << std::accumulate(lowest.begin(), lowest.end(), 0, [&heights](int sum, int b) { return sum += (heights[b]+1);}) << "\n";
    return 0;
}

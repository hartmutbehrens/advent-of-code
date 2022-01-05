#include <algorithm>
#include <numeric>
#include <fstream>
#include <iterator>
#include <iostream>
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

int up(const std::vector<int> &heights, int pos, int grid_length = 100) {
    return pos >= grid_length ? heights[pos - grid_length] : 9;
}

int down(const std::vector<int> &heights, int pos, int grid_length = 100) {
    return pos + grid_length >= heights.size() ? 9: heights[pos + grid_length];
}

int left(const std::vector<int> &heights, int pos, int grid_length = 100) {
    return pos % grid_length > 0 ? heights[pos - 1] : 9;
}

int right(const std::vector<int> &heights, int pos, int grid_length = 100) {
    return pos % grid_length < grid_length - 1 ? heights[pos + 1] : 9;
}

int which_low(const std::vector<int> &lows, const std::vector<int> &heights, int pos, int grid_length = 100) {
    auto it = std::find(lows.begin(), lows.end(), pos);
    if (it != lows.end()) {
        return it - lows.begin();
    }
    if (up(heights, pos) < heights[pos]) {
        return which_low(lows, heights, pos - grid_length);
    }
    if (down(heights, pos) < heights[pos]) {
        return which_low(lows, heights, pos + grid_length);
    }
    if (left(heights, pos) < heights[pos]) {
        return which_low(lows, heights, pos - 1);
    }
    if (right(heights, pos) < heights[pos]) {
        return which_low(lows, heights, pos + 1);
    }
    return 0;
}


int main() {
    auto heights = read_file("input");
    auto is_lowest = [&heights](int p) {
        return (heights[p] < up(heights, p)) &&
                (heights[p] < down(heights, p)) &&
                (heights[p] < left(heights, p)) &&
                (heights[p] < right(heights, p));
    };
    std::vector<int> lows;
    for(int i=0; i < heights.size(); ++i) {
        if (is_lowest(i)) {
            lows.push_back(i);
        }
    }
    // part 1 = 456
    std::cout << "part 1 = " << std::accumulate(lows.begin(), lows.end(), 0, [&heights](int sum, int b) { return sum += (heights[b]+1);}) << "\n";
    // part 2 - get basin sizes
    std::vector<int> low_count(lows.size());
    for(int i=0; i < heights.size(); ++i) {
        if (heights[i] != 9 ) {
            low_count[which_low(lows, heights, i)]++;
        }
    }
    std::sort(low_count.rbegin(), low_count.rend());
    // part 2 - 1047744
    std::cout << "part 2 = " << low_count[0] * low_count[1] * low_count[2] << "\n";
    return 0;
}

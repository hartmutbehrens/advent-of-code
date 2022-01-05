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

int up(const std::vector<int> &heights, int pos, auto cmp, int grid_length = 100) {
    int value = pos - grid_length > 0 ? heights[pos - grid_length] : 9;
    return cmp(value, heights[pos]);
}

int down(const std::vector<int> &heights, int pos, auto cmp, int grid_length = 100) {
    int value = pos + grid_length < heights.size() ? heights[pos + grid_length] : 9;
    return cmp(value, heights[pos]);
}

int left(const std::vector<int> &heights, int pos, auto cmp, int grid_length = 100) {
    int value = pos % grid_length > 0 ? heights[pos - 1] : 9;
    return cmp(value, heights[pos]);
}

int right(const std::vector<int> &heights, int pos, auto cmp, int grid_length = 100) {
    int value = pos % grid_length < grid_length ? heights[pos + 1] : 9;
    return cmp(value, heights[pos]);
}

int which_low(const std::vector<int> &lows, const std::vector<int> &heights, int pos, int grid_length = 100) {
    auto it = std::find(lows.begin(), lows.end(), pos);
    if (it != lows.end()) {
        return it - lows.begin();
    }
    if (up(heights, pos, std::less<>())) {
        return which_low(lows, heights, pos - grid_length);
    }
    if (down(heights, pos, std::less<>())) {
        return which_low(lows, heights, pos + grid_length);
    }
    if (left(heights, pos, std::less<>())) {
        return which_low(lows, heights, pos - 1);
    }
    if (right(heights, pos, std::less<>())) {
        return which_low(lows, heights, pos + 1);
    }
    return 0;
}


int main() {
    auto heights = read_file("input");
    std::vector<int> positions(heights.size());
    std::iota(positions.begin(), positions.end(), 0);
    std::vector<int> lows;
    std::copy_if(positions.begin(), positions.end(),
                  std::back_inserter(lows),
                  [&heights](int p) { return
                  up(heights, p, std::greater<>()) &&
                  down(heights, p, std::greater<>()) &&
                  left(heights, p, std::greater<>()) &&
                  right(heights, p, std::greater<>());});
    // part 1 = 456
    std::cout << "part 1 = " << std::accumulate(lows.begin(), lows.end(), 0, [&heights](int sum, int b) { return sum += (heights[b]+1);}) << "\n";
    // part 2 - get basin sizes
    std::vector<int> low_count(lows.size());
    std::for_each(positions.begin(), positions.end(),
                  [&heights, &lows, &low_count](int p) {
        if (heights[p] != 9) {
            low_count[which_low(lows, heights, p)]++;
        }
    });
    std::sort(low_count.rbegin(), low_count.rend());
    // part 2 - 1047744
    std::cout << "part 2 = " << low_count[0] * low_count[1] * low_count[2] << "\n";
    return 0;
}

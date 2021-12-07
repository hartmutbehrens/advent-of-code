#include <algorithm>
#include <numeric>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


[[nodiscard]]
auto read_input(const std::string &filename) {
    std::ifstream in{filename};
    std::vector<int> numbers;
    std::string number_as_string;
    while (std::getline(in, number_as_string, ',')) {
        numbers.push_back(std::stoi(number_as_string));
    }
    return numbers;
}

int main() {
    auto positions = read_input("input");
    int num = int(positions.size());
    std::sort(positions.begin(), positions.end());
    auto median = (positions[num/2] + positions[num/2 - 1]) / 2;
    auto spend_fuel = [&median](int sum, int pos) { return sum += abs(pos-median); };
    std::cout << "fuel spent = " << std::accumulate(positions.begin(), positions.end(), 0, spend_fuel) << "\n";
    auto mean = std::accumulate(positions.begin(), positions.end(), 0) / num;
    auto spend_fuel_crab_way = [&mean](int sum, int pos) {
        for(int i=1;i<=abs(pos-mean); ++i) {
            sum+=i;
        }
        return sum;
    };
    std::cout << "crab way fuel spent = " << std::accumulate(positions.begin(), positions.end(), 0, spend_fuel_crab_way) << "\n";
    return 0;
}

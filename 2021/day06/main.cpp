#include <algorithm>
#include <numeric>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


[[nodiscard]]
std::vector<int> read_input(const std::string &filename) {
    std::ifstream in{filename};
    // vector containing number of fish per day
    // index of vector == day
    std::vector<int> num_fish_per_age(9);
    std::string number_as_string;
    while (std::getline(in, number_as_string, ',')) {
        int age = std::stoi(number_as_string);
        num_fish_per_age[age]++;
    }
    return num_fish_per_age;
}

int main() {
    std::vector<int> num_fish_per_age = read_input("input");

    auto evolve_population = [&num_fish_per_age]() {
        int fish_age_0 = num_fish_per_age[0];
        // while each other number decreases by 1 if it was present at the start of the day.
        std::rotate(num_fish_per_age.begin(), num_fish_per_age.begin()+1, num_fish_per_age.end());
//        for (std::vector<int>::iterator it=num_fish_per_age.begin(); it!=num_fish_per_age.end(); ++it)
//            std::cout << ' ' << *it;
//        std::cout << "\n";
        // Each day, a 0 becomes a 6 ,
        num_fish_per_age[6] += fish_age_0;
        // and adds a new 8 to the end of the list
        num_fish_per_age[8] = fish_age_0;
    };
//    for (std::vector<int>::iterator it=num_fish_per_age.begin(); it!=num_fish_per_age.end(); ++it)
//        std::cout << ' ' << *it;
//    std::cout << "\n";
    for (int n=0; n<80; n++) {
        evolve_population();
    }
    // 950082 - too high
    std::cout << "num fish after 80 days = " << std::accumulate(num_fish_per_age.begin(), num_fish_per_age.end(), 0) << "\n";
    return 0;
}

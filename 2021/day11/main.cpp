#include <algorithm>
#include <numeric>
#include <fstream>
#include <iterator>
#include <iostream>
#include <vector>

[[nodiscard]]
std::vector<int> read_file(const std::string &filename) {
    std::ifstream in{filename};
    std::vector<int> numbers{};
    std::for_each(std::istream_iterator<char>{in},
                  std::istream_iterator<char>{},
                  [&numbers](char c) { numbers.push_back(c - '0');});
    return numbers;
}

void print_energies(const std::vector<int> & energies, int length = 10) {
    int i = 0;
    for(const auto e: energies) {
        std::cout << e << " ";
        if (i == length - 1) {
            std::cout << "\n";
            i = 0;
        } else {
            ++i;
        }
    }
    std::cout << "\n";
}


// operator overload to sum two pairs
template <typename T,typename U>
std::pair<T,U> operator+(const std::pair<T,U> & l,const std::pair<T,U> & r) {
    return {l.first+r.first,l.second+r.second};
}

std::vector<int> adjacent_positions(int pos, int length = 10) {
    std::vector<int> adjacent{};
    // all possible directions
    // up, down, left, right, UL, UR, LL, LR
    static std::vector<std::pair<int,int>> directions{{0,1},{0,-1},{-1,0},{1,0},{-1,1},{1,1},{-1,-1},{1,-1}};
    // convert pos to x,y
    std::pair<int, int> p = std::make_pair(pos % length, int(pos / length));
    std::for_each(directions.begin(),
                  directions.end(),
                  [&p, length, &adjacent](auto d) {
                      auto t = p + d;
                      // add to adjacency list if valid position
                      if ((t.first >=0) && (t.second >=0) && (t.first < length) && (t.second < length)) {
                          return adjacent.push_back(t.second * length + t.first);
                      }
                  });
    return adjacent;
}

std::vector<int> flashing_positions(const std::vector<int> &energies) {
    std::vector<int> positions(energies.size());
    std::iota(positions.begin(), positions.end(), 0);
    std::vector<int> flashing{};
    std::for_each(positions.begin(),
                  positions.end(),
                  [&energies, &flashing](int p){ if (energies[p] > 9) { flashing.push_back(p);}});
    return flashing;
}

int simulate_step(std::vector<int> &energies) {
    // energy level of each octopus increases by one
    std::transform(energies.begin(), energies.end(), energies.begin(),
                   [](int e){ return ++e;});
    // octopus with energy level greater than 9 flashes
    auto flashing = flashing_positions(energies);
    int flashes = 0;
    do {
        flashes += int(flashing.size());
        std::for_each(flashing.begin(),
                      flashing.end(),
                      [&energies](int p) {
            // reset this flashing position
            energies[p] = 0;
            // increment adjacent positions
            for (int pos : adjacent_positions(p)) {
                // octopus can flash at most once per step
                if (energies[pos] > 0) {
                    ++energies[pos];
                }
            }
        });
        flashing = flashing_positions(energies);
    } while (!flashing.empty());
    return flashes;
}



int main() {
    auto energies = read_file("input");
    int flashes = 0;
    print_energies(energies);
    for (int x=0; x<100; ++x) {
        flashes += simulate_step(energies);
    }
    print_energies(energies);
    // 1617 flashes
    std::cout << "number flashes = " << flashes << "\n";
    return 0;
}

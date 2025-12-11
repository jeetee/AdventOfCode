#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdint>

#include <algorithm>
#include <execution>
#include <utility>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::string_literals::operator""s;
using std::vector;

static void logic(string fileName);

int main()
{
    cout << "Advent of Code 2025: 05\n";
    logic("05-example.txt"s);
    cout << endl;
    logic("05-input.txt"s);
    return 0;
}

static void logic(string fileName)
{
    cout << "Processing " << fileName << " file " << endl;
    std::ifstream input(fileName);

    vector<std::pair<uint64_t,uint64_t>> valid_ranges;

    for (string line; std::getline(input, line) && !line.empty();) {
        std::stringstream ss(line);
        uint64_t min, max;
        ss >> min;
        ss.ignore(); // Skip '-'
        ss >> max;
        valid_ranges.emplace_back(min, max);
    }

    int valid_count = 0;
    for (uint64_t num; input >> num;) {
        cout << '.' << std::flush; // Progress report
        if (std::any_of(std::execution::par, std::begin(valid_ranges), std::end(valid_ranges), [&num](auto& minmax){
                return (minmax.first <= num) && (num <= minmax.second);
            })){
            ++valid_count;
        }
    }

    cout << "\nNumber of valid IDs = " << valid_count << endl;
}

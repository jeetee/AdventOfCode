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

    // Reduce ranges by merging overlaps (for part 2)
    std::sort(std::execution::par, std::begin(valid_ranges), std::end(valid_ranges));
    // As long as the start of the next interval is before my end, we can merge
    // as it will always be equal to or above my own start due to the initial sorting
    auto current_it = std::begin(valid_ranges);
    auto next_it = valid_ranges.empty() ? std::end(valid_ranges) : std::next(current_it);
    while (next_it != std::end(valid_ranges)) {
        if (next_it->first <= current_it->second) {
            current_it->second = std::max(current_it->second, next_it->second);
        }
        else { // Next does not overlap current
            // Move it into the new (next) current position so the resulting vector will have no holes
            *(++current_it) = std::move(*next_it);
        }
        ++next_it;
    }
    valid_ranges.erase(std::next(current_it), std::end(valid_ranges));

    int valid_count = 0;
    for (uint64_t num; input >> num;) {
        cout << '.' << std::flush; // Progress report
        if (std::any_of(std::execution::par, std::begin(valid_ranges), std::end(valid_ranges), [&num](auto& minmax){
                return (minmax.first <= num) && (num <= minmax.second);
            })){
            ++valid_count;
        }
    }

    const uint64_t count_of_fresh_IDs = std::transform_reduce(std::execution::par, std::begin(valid_ranges), std::end(valid_ranges),
                                                              0ul, std::plus{},
                                                              [](std::pair<uint64_t, uint64_t> range){
        return range.second - range.first + 1;
    });

    cout << "\nNumber of valid IDs = " << valid_count
         << "\nNumber of fresh IDs = " << count_of_fresh_IDs << endl;
}

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdint>

#include <numeric>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::string_literals::operator""s;
using std::vector;

static void logic(string fileName);

int main()
{
    cout << "Advent of Code 2025: 02\n";
    logic("02-example.txt"s);
    cout << endl;
    logic("02-input.txt"s);
    return 0;
}

static void logic(string fileName)
{
    cout << "Processing " << fileName << " file " << endl;
    std::ifstream input(fileName);

    uint64_t low_bound = 0;
    vector<uint64_t> invalid_ids;

    while (input >> low_bound) {
        cout << "." << std::flush; // Progress report
        input.ignore(1); // skip the '-'
        uint64_t high_bound;
        input >> high_bound;
        input.ignore(1); // skip ','

        for (uint64_t id = low_bound; id <= high_bound; ++id) {
            std::stringstream ss;
            ss << id;
            const auto sv = ss.view();
            auto num_chars = sv.length();
            // Can only be invalid if amount of characters is even
            if (num_chars % 2) {
                continue;
            }

            num_chars /= 2;
            if (sv.substr(0, num_chars).compare(sv.substr(num_chars)) == 0) {
                cout << ' ' << id << ' ';
                invalid_ids.emplace_back(id);
            }
        }
    }

    cout << "\nSum of invalid IDs = " << std::accumulate(invalid_ids.cbegin(), invalid_ids.cend(), 0ul) << endl;
}

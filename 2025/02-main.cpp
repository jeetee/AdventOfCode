#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdint>

#include <numeric>
#include <regex>
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
    vector<uint64_t> invalid_ids_part_one, invalid_ids_part_two;

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
            const auto num_chars = sv.length() / 2;
            bool is_valid = true;

            for (auto regex_len = 1; (regex_len <= num_chars) && is_valid; ++regex_len) {
                if ((sv.length() % regex_len) != 0) {
                    // Can't be invalid if pattern doesn't fit exactly into total number
                    continue;
                }
                std::stringstream regexbuild;
                regexbuild << "^(" << sv.substr(0, regex_len) << ")+$";
                std::regex pattern(regexbuild.str());
                if (std::regex_match(ss.str(), pattern)) {
                    cout << ' ' << id << ' ';
                    invalid_ids_part_two.emplace_back(id);
                    is_valid = false;
                }
            }

            if (sv.substr(0, num_chars).compare(sv.substr(num_chars)) == 0) {
                //cout << ' ' << id << ' ';
                invalid_ids_part_one.emplace_back(id);
            }
        }
    }

    cout << "\nSum of invalid IDs 1 = " << std::accumulate(invalid_ids_part_one.cbegin(), invalid_ids_part_one.cend(), 0ul)
         << "\nSum of invalid IDs 2 = " << std::accumulate(invalid_ids_part_two.cbegin(), invalid_ids_part_two.cend(), 0ul) << endl;
}

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
    cout << "Advent of Code 2025: 07\n";
    logic("07-example.txt"s);
    cout << endl;
    logic("07-input.txt"s);
    return 0;
}

static void logic(string fileName)
{
    cout << "Processing " << fileName << " file " << endl;
    std::ifstream input(fileName);

    vector<uint64_t> timeline_counts; // Track beam timelines for each column at a point in time
    string line_1;
    { // Parsing start line
        std::getline(input, line_1);
        std::replace(std::execution::par, std::begin(line_1), std::end(line_1), 'S', '|');
        input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        timeline_counts.resize(line_1.size(), 0);
        timeline_counts[line_1.find('|')] = 1;
    }

    uint split_count = 0;
    for (string line; std::getline(input, line);) {
        vector<uint64_t> next_timeline(timeline_counts.size(), 0);
        for (auto i = line_1.size(); i --> 0;) {
            if (line_1[i] == '|') {
                if (line[i] == '^') { // Split the beam
                    line[i-1] = line[i+1] = '|';
                    line[i] = '.';
                    ++split_count;
                    next_timeline[i-1] += timeline_counts[i];
                    next_timeline[i+1] += timeline_counts[i];
                }
                else { // Let it flow
                    line[i] = '|';
                    next_timeline[i] += timeline_counts[i];
                }
            }
            else if (line[i] == '^') { // Clear out splitters that had nothing to do
                line[i] = '.';
            }
        }
        line_1 = std::move(line);
        timeline_counts = std::move(next_timeline);
        // Next line is empty ......
        input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    cout << "\nBeam split count = " << split_count
         << "\nBeam split timelines: " << std::reduce(std::execution::par, std::begin(timeline_counts), std::end(timeline_counts), 0ul) << endl;
}

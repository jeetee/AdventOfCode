#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdint>

#include <iterator>
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
    cout << "Advent of Code 2025: 04\n";
    logic("04-example.txt"s);
    cout << endl;
    logic("04-input.txt"s);
    return 0;
}

static vector<string> remove_reachable_rolls(vector<string>& floorplan, int& amount_removed)
{
    vector<string> copy = floorplan;

    const size_t height = floorplan.size();
    const size_t width = floorplan.front().size();

    int reachable_rolls_count = 0;
    for (auto h = height; h --> 0;) {
        for (auto w = width; w --> 0;) {
            if (floorplan[h][w] != '@') {
                continue;
            }

            int surrounding_rolls_count = 0;
            // Row above me
            if ((h > 0) && (w > 0) && (floorplan[h-1][w-1] == '@')) {
                ++surrounding_rolls_count;
            }
            if ((h > 0) && (floorplan[h-1][w] == '@')) {
                ++surrounding_rolls_count;
            }
            if ((h > 0) && (w < width-1) && (floorplan[h-1][w+1] == '@')) {
                ++surrounding_rolls_count;
            }
            // Same row, neighbors
            if ((w > 0) && (floorplan[h][w-1] == '@')) {
                ++surrounding_rolls_count;
            }
            if ((w < width-1) && (floorplan[h][w+1] == '@')) {
                ++surrounding_rolls_count;
            }
            // Row below me
            if ((h < height-1) && (w > 0) && (floorplan[h+1][w-1] == '@')) {
                ++surrounding_rolls_count;
            }
            if ((h < height-1) && (floorplan[h+1][w] == '@')) {
                ++surrounding_rolls_count;
            }
            if ((h < height-1) && (w < width-1) && (floorplan[h+1][w+1] == '@')) {
                ++surrounding_rolls_count;
            }

            if (surrounding_rolls_count < 4) {
                ++reachable_rolls_count;
                copy[h][w] = 'x'; // Remove indicator
            }
        }
    }

    amount_removed = reachable_rolls_count;
    return copy;
}

static void logic(string fileName)
{
    cout << "Processing " << fileName << " file " << endl;
    std::ifstream input(fileName);

    vector<string> floorplan;

    for (string line; std::getline(input, line);) {
        floorplan.emplace_back(line);
    }

    // PART 1
    int reachable_rolls_count;
    auto resulting_floorplan = remove_reachable_rolls(floorplan, reachable_rolls_count);
    cout << "\nReachable rolls = " << reachable_rolls_count << endl;

    // PART 2
    int total_removed_rolls_count = 0;
    while (reachable_rolls_count != 0) {
        total_removed_rolls_count += reachable_rolls_count;
        resulting_floorplan = remove_reachable_rolls(resulting_floorplan, reachable_rolls_count);
    }
    std::copy(resulting_floorplan.cbegin(), resulting_floorplan.cend(), std::ostream_iterator<string>(cout, "\n"));

    cout << "Total removed rolls = " << total_removed_rolls_count << endl;
}

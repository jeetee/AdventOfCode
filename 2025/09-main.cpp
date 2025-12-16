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
    cout << "Advent of Code 2025: 09\n";
    logic("09-example.txt"s);
    cout << endl;
    logic("09-input.txt"s);
    return 0;
}

typedef struct point_s {
    int x, y;
} point_t;

static void logic(string fileName)
{
    cout << "Processing " << fileName << " file " << endl;
    std::ifstream input(fileName);

    vector<point_t> points;
    int x, y;
    while (input >> x) {
        input.ignore(); // Skip ','
        input >> y;
        points.emplace_back(x, y);
    }

    cout << "Finding largest rectangular area between two points " << endl;
    uint64_t max_area = 0;
    for (auto p1 = std::begin(points); p1 != std::prev(std::end(points)); ++p1) {
        for (auto p2 = std::next(p1); p2 != std::end(points); ++p2) {
            uint64_t area = (std::abs(p1->x - p2->x) + 1) * (uint64_t)(std::abs(p1->y - p2->y) + 1);
            if (area > max_area) {
                max_area = area;
            }
        }
        cout << '.' << std::flush; // Progress report
    }

    cout << "\nMax area = " << max_area << endl;
}

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
    int maxx = 0, maxy = 0;
    int x, y;
    while (input >> x) {
        input.ignore(); // Skip ','
        input >> y;
        points.emplace_back(x, y);
        if (x > maxx) { maxx = x; }
        if (y > maxy) { maxy = y; }
    }
    cout << "Max X = " << maxx << " Max Y = " << maxy << endl;

    // For each y, register circumference points as ranges in x
    vector<vector<std::pair<int, int>>> floorplan(maxy + 1);
    auto register_circumference = [&floorplan](const point_t& p1, const point_t& p2){
        // Process this circumference segment
        if (p1.y == p2.y) {
            // Same height, segment is entire x-range to register
            floorplan[p1.y].emplace_back(std::min(p1.x, p2.x), std::max(p1.x, p2.x));
        }
        else { // p1->x == p2->x -> same column
            for (int y = std::min(p1.y, p2.y) + 1; y < std::max(p1.y, p2.y); ++y) {
                floorplan[y].emplace_back(p1.x, p1.x);
            }
        }
    };
    for (auto p1 = std::cbegin(points), p2 = std::next(std::cbegin(points)); p2 != std::cend(points); ++p1, ++p2) {
        register_circumference(*p1, *p2);
    }
    // Same loop logic for last, closing, segment
    register_circumference(*std::crbegin(points), *std::cbegin(points));

    std::for_each(std::execution::par, std::begin(floorplan), std::end(floorplan), [](vector<std::pair<int, int>>& ranges){
        std::sort(std::execution::par, std::begin(ranges), std::end(ranges));
        bool merge = true;
        auto current_it = std::begin(ranges);
        auto next_it = ranges.empty() ? std::end(ranges) : std::next(current_it);
        while (next_it != std::end(ranges)) {
            if (merge) {
                current_it->second = next_it->second;
                if (next_it->first == next_it->second) {
                    // Merged a range of 1 -> this was a vertical line; toggle filling
                    merge = false;
                }
            }
            else {
                // Got a next range to start from -> make it consecutive
                *(++current_it) = std::move(*next_it);
                merge = true;
            }
            ++next_it;
        }
        ranges.erase(std::next(current_it), std::end(ranges));
    });

    cout << "Finding largest rectangular area between two points " << endl;
    uint64_t max_area = 0, max_contained_area = 0;
    for (auto p1 = std::begin(points); p1 != std::prev(std::end(points)); ++p1) {
        for (auto p2 = std::next(p1); p2 != std::end(points); ++p2) {
            uint64_t area = (std::abs(p1->x - p2->x) + 1) * (uint64_t)(std::abs(p1->y - p2->y) + 1);
            if (area > max_area) {
                max_area = area;
            }

            // Check whether this rectangle-combo is inside our polygon
            const point_t top_left { .x = std::min(p1->x, p2->x), .y = std::min(p1->y, p2->y) };
            const point_t bottom_right { .x = std::max(p1->x, p2->x), .y = std::max(p1->y, p2->y) };
            const bool contained = std::all_of(
                std::execution::par,
                std::next(std::cbegin(floorplan), top_left.y),
                std::next(std::cbegin(floorplan), bottom_right.y + 1), // +1 for inclusive check on height
                [&top_left, &bottom_right](const vector<std::pair<int, int>>& row) {
                    // Does width fit within a range of this row?
                    const auto left = top_left.x;
                    const auto right = bottom_right.x;
                    return std::any_of(std::execution::par, std::cbegin(row), std::cend(row), [&left, &right](const std::pair<int, int>& range){
                        return (left >= range.first) && (right <= range.second);
                    });
                });
            if (contained) {
                if (area > max_contained_area) {
                    max_contained_area = area;
                }
            }
        }
        cout << '.' << std::flush; // Progress report
    }

    cout << "\nMax area = " << max_area
         << "\nMax contained area = " << max_contained_area << endl;
}

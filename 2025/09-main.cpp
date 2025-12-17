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

/// \brief Check whether two line segments intersect
/// \details See https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection#Given_two_points_on_each_line_segment for formula
/// \param p1 first point of Segment 1
/// \param p2 second point of Segment 1
/// \param p3 first point of Segment 2
/// \param p4 second point of Segment 2
bool segments_intersect(const point_t& p1, const point_t& p2, const point_t& p3, const point_t& p4)
{
    const int64_t denominator = (p1.x - p2.x) * (int64_t)(p3.y - p4.y)
                              - ((p1.y - p2.y) * (int64_t)(p3.x - p4.x));
    if (denominator == 0) {
        // Parallel or coinciding lines, considered not crossing
        return false;
    }
    const int64_t nominator_t = (p1.x - p3.x) * (int64_t)(p3.y - p4.y)
                                - ((p1.y - p3.y) * (int64_t)(p3.x - p4.x));
    const int64_t nominator_u = -(p1.x - p2.x) * (int64_t)(p1.y - p3.y)
                                + ((p1.y - p2.y) * (int64_t)(p1.x - p3.x));

    return (
        ( // 0 <= t <= 1
            ((denominator > 0) && (nominator_t >= 0) && (nominator_t <= denominator))
         || ((denominator < 0) && (nominator_t <= 0) && (nominator_t >= denominator))
        )
        &&
        ( // 0 <= u <= 1
            ((denominator > 0) && (nominator_u >= 0) && (nominator_u <= denominator))
         || ((denominator < 0) && (nominator_u <= 0) && (nominator_u >= denominator))
        )
    );
}

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

    vector<std::pair<point_t const*const, point_t const*const>> segments;
    segments.reserve(points.size());
    for (auto p1 = std::cbegin(points), p2 = std::next(std::cbegin(points)); p2 != std::cend(points); ++p1, ++p2) {
        segments.emplace_back(&*p1, &*p2);
    }
    segments.emplace_back(&*std::crbegin(points), &*std::cbegin(points)); // Add wrapping segment

    cout << "Finding largest rectangular area between two points " << endl;
    uint64_t max_area = 0, max_contained_area = 0;
    for (auto p1 = std::begin(points); p1 != std::prev(std::end(points)); ++p1) {
        for (auto p2 = std::next(p1); p2 != std::end(points); ++p2) {
            uint64_t area = (std::abs(p1->x - p2->x) + 1) * (uint64_t)(std::abs(p1->y - p2->y) + 1);
            if (area > max_area) {
                max_area = area;
            }
            // Check whether this rectangle-combo is inside our polygon
            // If we check whether it crosses any of the polygon boundaries we know it to be (partially) outside
            // We need all corner points to be able to check
            const point_t p12 { .x = p1->x, .y = p2->y }, p21 { .x = p2->x, .y = p1->y };
            // Start with the wrapping segment
            bool crossing = segments_intersect(*p1, p12, *std::crbegin(points), *std::cbegin(points))
                | segments_intersect(p12, *p2, *std::crbegin(points), *std::cbegin(points))
                | segments_intersect(*p2, p21, *std::crbegin(points), *std::cbegin(points))
                | segments_intersect(p21, *p1, *std::crbegin(points), *std::cbegin(points));

            if (!crossing) { // Check the other segments
                crossing = std::any_of(std::execution::par, std::cbegin(segments), std::prev(std::cend(segments)), [&p1, &p2, &p12, &p21](auto segment){
                    return segments_intersect(*p1, p12, *segment.first, *segment.second)
                         | segments_intersect(p12, *p2, *segment.first, *segment.second)
                         | segments_intersect(*p2, p21, *segment.first, *segment.second)
                         | segments_intersect(p21, *p1, *segment.first, *segment.second);
                });
            }

            if (!crossing) {
                // We are considered inside, even if it's just a single shared edge overlap
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

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdint>

#include <algorithm>
#include <execution>
#include <set>
#include <utility>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::string_literals::operator""s;
using std::vector;

static void logic(string fileName, int connection_count);

int main()
{
    cout << "Advent of Code 2025: 08\n";
    logic("08-example.txt"s, 10);
    cout << endl;
    logic("08-input.txt"s, 1000);
    return 0;
}

typedef struct point_s {
    int x, y, z;
    bool operator==(const point_s& other) const {
        return (x == other.x) && (y == other.y) && (z == other.z);
    }

    int circuitIdx = -1;
} point_t;

typedef struct pointpair_s {
    const uint64_t distance;
    point_t * const p1;
    point_t * const p2;
    bool operator<(const pointpair_s& other) const {
        return distance < other.distance;
    }
} pointpair_t;

static void logic(string fileName, int connection_count)
{
    cout << "Processing " << fileName << " file " << endl;
    std::ifstream input(fileName);

    vector<point_t> points;
    int x, y, z;
    while (input >> x) {
        input.ignore(); // Skip ','
        input >> y;
        input.ignore(); // Skip ','
        input >> z;
        points.emplace_back(x, y, z);
    }

    cout << "Building distance graph " << endl;
    std::multiset<pointpair_t> distance_between_points;
    for (auto p1 = std::begin(points); p1 != std::prev(std::end(points)); ++p1) {
        for (auto p2 = std::next(p1); p2 != std::end(points); ++p2) {
            // As we only need to compare distances, there's no need to square them all
            uint64_t distance = (p1->x - p2->x) * (p1->x - p2->x);
            distance += (p1->y - p2->y) * (p1->y - p2->y);
            distance += (p1->z - p2->z) * (p1->z - p2->z);
            distance_between_points.emplace(distance, &*p1, &*p2);
        }
        cout << '.' << std::flush; // Progress report
    }

    cout << "Building circuits " << endl;
    vector<vector<point_t *>> circuits;
    for (auto closest = std::begin(distance_between_points); closest != std::next(std::begin(distance_between_points), connection_count); ++closest) {
        if (closest->p1->circuitIdx == -1) {
            // P1 is not part of a circuit
            if (closest->p2->circuitIdx != -1) {
                // join P2
                closest->p1->circuitIdx = closest->p2->circuitIdx;
                circuits[closest->p2->circuitIdx].push_back(closest->p1);
            }
            else {
                // Create a new circuit
                closest->p1->circuitIdx = closest->p2->circuitIdx = circuits.size();
                circuits.push_back(vector<point_t *>{
                    closest->p1, closest->p2
                });
            }
        }
        else {
            if (closest->p2->circuitIdx == -1) {
                // P2 is not part of a circuit, join P1
                closest->p2->circuitIdx = closest->p1->circuitIdx;
                circuits[closest->p1->circuitIdx].push_back(closest->p2);
            }
            else if (closest->p2->circuitIdx != closest->p1->circuitIdx) {
                // P2 is part of a different circuit -> merge them
                const auto old_circuit = closest->p2->circuitIdx;
                const auto new_circuit = closest->p1->circuitIdx;
                std::for_each(std::begin(circuits[old_circuit]), std::end(circuits[old_circuit]), [&new_circuit](point_t * point) {
                    point->circuitIdx = new_circuit;
                });
                circuits[new_circuit].insert(std::end(circuits[new_circuit]),
                                             std::cbegin(circuits[old_circuit]), std::cend(circuits[old_circuit]));
                circuits[old_circuit].clear();
            }
        }
    }

    // Sort by circuit size (note, this breaks circuitIdx references inside the point_s data
    std::sort(std::execution::par, std::begin(circuits), std::end(circuits), [](vector<point_t *>& a, vector<point_t *>& b){
        return a.size() > b.size();
    });

    const auto top_circuit_sizes_mult = circuits[0].size() * circuits[1].size() * circuits[2].size();

    cout << "\nThree largest circuit sizes multiplied = " << top_circuit_sizes_mult << endl;
}

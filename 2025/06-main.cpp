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
    cout << "Advent of Code 2025: 06\n";
    logic("06-example.txt"s);
    cout << endl;
    logic("06-input.txt"s);
    return 0;
}

typedef struct math_problem_s {
    const char operation;
    vector<uint64_t> terms;
} math_problem_t;

static void logic(string fileName)
{
    cout << "Processing " << fileName << " file " << endl;
    std::ifstream input(fileName);

    vector<math_problem_t> math_problems;
    { // Scope input parsing
        vector<string> lines;
        for (string line; std::getline(input, line) && !line.empty();) {
            lines.emplace_back(line);
        }

        // Use operator reading (as it is a line using separate parsing logic) to aid in detecting number of math_problems
        std::stringstream ss(lines.back());
        for (char c; ss >> c;) {
            math_problems.push_back(math_problem_t {
                .operation = c
            });
        }

        for (auto line = std::begin(lines); line != std::prev(std::end(lines)); ++line) {
            ss.clear();
            ss.str(*line);
            for (uint64_t term, i = 0; ss >> term; ++i) {
                math_problems[i].terms.emplace_back(term);
            }
        }
    }

    const uint64_t grand_total = std::transform_reduce(std::execution::par, std::begin(math_problems), std::end(math_problems),
                                                       0ul, std::plus{},
                                                       [](const math_problem_t& math_problem) {
        return std::reduce(std::execution::par, std::begin(math_problem.terms), std::end(math_problem.terms),
                           (math_problem.operation == '+') ? 0ul : 1ul,
                           [&math_problem](uint64_t a, uint64_t b) {
                                return (math_problem.operation == '+') ? (a + b) : (a * b);
                            });
    });

    cout << "\nGrand total = " << grand_total << endl;
}

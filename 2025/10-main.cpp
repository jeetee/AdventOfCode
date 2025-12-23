#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdint>

#include <algorithm>
#include <deque>
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
    cout << "Advent of Code 2025: 10\n";
    logic("10-example.txt"s);
    cout << endl;
    logic("10-input.txt"s);
    return 0;
}

typedef struct button_s {
    uint16_t bits;
    vector<uint> joltage_additions;
} button_t;

static void logic(string fileName)
{
    cout << "Processing " << fileName << " file " << endl;
    std::ifstream input(fileName);

    uint64_t min_button_presses = 0, min_joltage_presses = 0;
    for (string line; std::getline(input, line);) {
        cout << '.' << std::flush; // Progress report
        auto ssline = std::stringstream(line);

        uint16_t desired_lamp_state = 0;
        ssline.ignore(1); // '['
        uint circuit_size = 0;
        for (char bit; (ssline >> bit) && (bit != ']'); ++circuit_size) {
            if (bit == '#') {
                desired_lamp_state |= 1 << circuit_size;
            }
        }

        vector<button_t> buttons;
        for (char brace; (ssline >> brace) && (brace == '(');) {
            button_t button {
                .bits = 0,
                .joltage_additions = vector<uint>(circuit_size, 0),
            };
            do {
                int bit;
                ssline >> bit;
                button.bits |= 1 << bit;
                button.joltage_additions[bit] = 1;
                ssline >> brace;
            } while (brace != ')');
            buttons.emplace_back(button);
        }

        // joltage input ignored for now
        vector<uint> desired_joltages; desired_joltages.reserve(circuit_size);
        for (uint joltage; ssline >> joltage;) {
            desired_joltages.emplace_back(joltage);
            ssline.ignore(1); // ',' or '}'
        }

        // Solve this line/machine
        std::deque<std::pair<uint16_t /*lampstate*/, uint /*presscount*/>> bfs_steps;
        auto step = std::pair<uint16_t, uint>(0, 0);
        bfs_steps.push_back(step);
        while (step.first != desired_lamp_state) {
            // Press the next button
            std::for_each(std::execution::par, std::cbegin(buttons), std::cend(buttons), [&step, &bfs_steps](const auto& button){
                const auto next_step_lamps = step.first ^ button.bits;
                if (next_step_lamps != 0) { // If == 0: Back at start situation, prune non-optimal search branch
                    bfs_steps.emplace_back(next_step_lamps, step.second + 1);
                }
            });
            bfs_steps.pop_front(); // Done processing this step, remove it
            step = bfs_steps.front();
        }
        // Desired lamp state reached.
        min_button_presses += step.second;

        cout << '#' << std::flush; // Progress report
        // Joltage solutions
        std::deque<std::pair<vector<uint>/*joltagestate*/, uint /*presscount*/>> bfs_jolt_steps;
        auto jstep = std::pair<vector<uint>, uint>(vector<uint>(circuit_size, 0), 0);
        bfs_jolt_steps.push_back(jstep);
        for (bool solved = false; !solved;) {
            // Press the next button
            std::for_each(std::execution::par, std::cbegin(buttons), std::cend(buttons), [&jstep, &bfs_jolt_steps, &desired_joltages](const auto& button){
                vector<uint> next_step_joltages(jstep.first.size());
                std::transform(std::execution::par, std::cbegin(jstep.first), std::cend(jstep.first),
                               std::cbegin(button.joltage_additions), std::begin(next_step_joltages), std::plus<uint>()
                               );
                if (std::equal(std::execution::par, std::cbegin(next_step_joltages), std::cend(next_step_joltages),
                               std::cbegin(desired_joltages), std::less_equal<uint>())) {
                    // We did not yet exceed any of the desired_joltages
                    bfs_jolt_steps.emplace_back(next_step_joltages, jstep.second + 1);
                }
            });
            bfs_jolt_steps.pop_front(); // Done processing this step, remove it
            jstep = bfs_jolt_steps.front();
            solved = std::equal(std::execution::par, std::cbegin(jstep.first), std::cend(jstep.first), std::cbegin(desired_joltages));
        }
        // Desired joltages reached.
        min_joltage_presses += jstep.second;
    }

    cout << "\nButton presses required = " << min_button_presses
         << "\nButton presses required for joltage = " << min_joltage_presses << endl;
}

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

static void logic(string fileName)
{
    cout << "Processing " << fileName << " file " << endl;
    std::ifstream input(fileName);

    uint64_t min_button_presses = 0;
    for (string line; std::getline(input, line);) {
        cout << '.' << std::flush; // Progress report
        auto ssline = std::stringstream(line);

        uint16_t desired_lamp_state = 0;
        ssline.ignore(1); // '['
        for (char bit, idx = 0; (ssline >> bit) && (bit != ']'); ++idx) {
            if (bit == '#') {
                desired_lamp_state |= 1 << idx;
            }
        }

        vector<uint16_t> buttons;
        for (char brace; (ssline >> brace) && (brace == '(');) {
            uint16_t button = 0;
            do {
                int bit;
                ssline >> bit;
                button |= 1 << bit;
                ssline >> brace;
            } while (brace != ')');
            buttons.emplace_back(button);
        }

        // joltage input ignored for now

        // Solve this line/machine
        std::deque<std::pair<uint16_t /*lampstate*/, uint /*presscount*/>> bfs_steps;
        auto step = std::pair<uint16_t, uint>(0, 0);
        bfs_steps.push_back(step);
        while (step.first != desired_lamp_state) {
            // Press the next button
            std::for_each(std::execution::par, std::cbegin(buttons), std::cend(buttons), [&step, &bfs_steps](const auto& button){
                const auto next_step_lamps = step.first ^ button;
                if (next_step_lamps != 0) { // If == 0: Back at start situation, prune non-optimal search branch
                    bfs_steps.emplace_back(next_step_lamps, step.second + 1);
                }
            });
            bfs_steps.pop_front(); // Done processing this step, remove it
            step = bfs_steps.front();
        }
        // Desired lamp state reached.
        min_button_presses += step.second;
    }

    cout << "\nButton presses required = " << min_button_presses << endl;
}

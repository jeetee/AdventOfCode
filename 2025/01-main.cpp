#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <algorithm>
#include <execution>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::string_literals::operator""s;
using std::vector;

static void logic(string fileName);

int main()
{
    cout << "Advent of Code 2025: 01\n";
    logic("01-example.txt"s);
    cout << endl;
    logic("01-input.txt"s);
    return 0;
}

static void logic(string fileName)
{
    cout << "Processing " << fileName << " file " << endl;
    std::ifstream input(fileName);

    int dial = 50;
    unsigned int times_at_zero = 0, times_passed_or_at_zero = 0;

    for (string line; std::getline(input, line);) {
        cout << "." << std::flush; // Progress report
        auto ssline = std::stringstream(line);
        char direction;
        int amount;
        ssline >> direction >> amount;
        // A full cycle of 100 ends up at the same spot, so we can ignore those (for part 1)
        int full_cycles = amount / 100;
        amount -= full_cycles * 100;
        times_passed_or_at_zero += full_cycles;

        //cout << direction << " " << amount << " full cycles " << full_cycles << endl;

        // Process movement
        int prev_dial = dial;
        dial += amount * ((direction == 'L')? -1 : 1);
        // Wrap
        if (dial < 0) {
            dial += 100;
            if (prev_dial != 0) { // We didn't pass zero if we started there
                ++times_passed_or_at_zero;
            }
            //cout << "negative dial overflow";
        } else if (dial >= 100) {
            dial -= 100;
            if (dial != 0) { // Prevent double counting ending at 0 as it already is counted below
                ++times_passed_or_at_zero;
            }
            //cout << "positive dial overflow";
        }

        //cout << "\n dial resulting position " << dial << endl;

        if (dial == 0) {
            ++times_at_zero;
            ++times_passed_or_at_zero;
        }
        //cout << "\ttimes at zero = " << times_at_zero << " passed or at " << times_passed_or_at_zero << endl;
    }

    cout << "\nTimes at zero = " << times_at_zero
         << "\nTimes at or passed zero = " << times_passed_or_at_zero << endl;
}

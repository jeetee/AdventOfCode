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
    unsigned int times_at_zero = 0;

    for (string line; std::getline(input, line);) {
        cout << "." << std::flush; // Progress report
        auto ssline = std::stringstream(line);
        char direction;
        int amount;
        ssline >> direction >> amount;
        // A full cycle of 100 ends up at the same spot, so we can ignore those
        amount -= (amount / 100) * 100;

        // Process movement
        dial += amount * ((direction == 'L')? -1 : 1);
        // Wrap
        if (dial < 0) {
            dial += 100;
        } else if (dial >= 100) {
            dial -= 100;
        }

        if (dial == 0) {
            ++times_at_zero;
        }
    }

    // Part 1: sum the distance between both lists
    cout << "\nTimes at zero = " << times_at_zero << endl;
}

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdint>

#include <numeric>
#include <regex>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::string_literals::operator""s;
using std::vector;

static void logic(string fileName);

int main()
{
    cout << "Advent of Code 2025: 03\n";
    logic("03-example.txt"s);
    cout << endl;
    logic("03-input.txt"s);
    return 0;
}

static void logic(string fileName)
{
    cout << "Processing " << fileName << " file " << endl;
    std::ifstream input(fileName);

    uint64_t joltage = 0;

    for (string line; std::getline(input, line);) {
        cout << "." << std::flush; // Progress report

        char digit = '9';
        size_t tens_pos;
        for (; digit > '0'; --digit) {
            tens_pos = line.find(digit, 0);
            if ((tens_pos != string::npos) && (tens_pos < line.size()-1)) {
                cout << digit;
                break;
            }
        }

        joltage += 10 * (digit - '0');

        // Locate 2nd digit
        for (digit = '9'; digit > '0'; --digit) {
            auto pos = line.find(digit, tens_pos+1);
            if ((pos != string::npos) && (pos < line.size())) {
                cout << digit;
                break;
            }
        }
        joltage += digit - '0';
    }

    cout << "\nTotal joltage = " << joltage << endl;
}

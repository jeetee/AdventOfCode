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

static uint64_t find_joltage(const string numstr, const int digits)
{
    std::stringstream joltage_chars;
    // First digit must be far enough from the end to allow room for the full number
    size_t start_pos = 0, end_pos = numstr.length() - digits;
    for (int digits_to_find = digits; digits_to_find --> 0;) {

        for (char digit = '9'; digit > '0'; --digit) {
            auto pos = numstr.find(digit, start_pos);
            if ((pos != string::npos) && (pos <= end_pos)) {
                joltage_chars << digit;
                start_pos = pos + 1; // Next digit to be found past me
                break;
            }
        }

        // Next digit can be up to one pos closer to the end of the string
        ++end_pos;
    }
    uint64_t joltage;
    joltage_chars >> joltage;
    cout << joltage;
    return joltage;
}

static void logic(string fileName)
{
    cout << "Processing " << fileName << " file " << endl;
    std::ifstream input(fileName);

    uint64_t joltage = 0;

    for (string line; std::getline(input, line);) {
        cout << "." << std::flush; // Progress report

        joltage += find_joltage(line, 2);
    }

    cout << "\nTotal joltage = " << joltage << endl;
}

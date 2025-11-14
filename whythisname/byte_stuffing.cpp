#include <iostream>
#include <string>

using namespace std;

const string FLAG = "FLAG";
const string ESC = "ESC";

// Stuffs bytes to handle FLAG and ESC sequences
string byteStuff(const string &input) {
    string out = FLAG; // starting delimiter
    size_t i = 0;
    while (i < input.size()) {
        // Check FLAG occurrence
        if (i + FLAG.size() <= input.size() && input.compare(i, FLAG.size(), FLAG) == 0) {
            out += ESC + FLAG;
            i += FLAG.size();
        }
        // Check ESC occurrence
        else if (i + ESC.size() <= input.size() && input.compare(i, ESC.size(), ESC) == 0) {
            out += ESC + ESC;
            i += ESC.size();
        } else {
            out.push_back(input[i]);
            ++i;
        }
    }
    out += FLAG; // ending delimiter
    return out;
}

// Destuffs bytes to restore original data
string byteDestuff(const string &stuffed) {
    string out;
    size_t start = FLAG.size();
    size_t end = stuffed.size() - FLAG.size();
    size_t i = start;
    while (i < end) {
        if (i + ESC.size() <= end && stuffed.compare(i, ESC.size(), ESC) == 0) {
            i += ESC.size(); // skip ESC marker
            if (i + FLAG.size() <= end && stuffed.compare(i, FLAG.size(), FLAG) == 0) {
                out += FLAG;
                i += FLAG.size();
            } else if (i + ESC.size() <= end && stuffed.compare(i, ESC.size(), ESC) == 0) {
                out += ESC;
                i += ESC.size();
            } else {
                // Unexpected sequence; copy literal char if available
                if (i < end) {
                    out.push_back(stuffed[i]);
                    ++i;
                }
            }
        } else {
            out.push_back(stuffed[i]);
            ++i;
        }
    }
    return out;
}

int main() {
    string input;
    cout << "Enter input: ";
    cin >> input;
    auto stuffed = byteStuff(input);
    cout << "After Stuffing: " << stuffed << "\n";
    auto destuffed = byteDestuff(stuffed);
    cout << "After Destuffing: " << destuffed << "\n";
    return 0;
}

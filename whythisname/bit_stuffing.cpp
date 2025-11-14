#include <iostream>
#include <string>

using namespace std;

// Stuffs bits to prevent more than five consecutive '1's
string bitStuff(const string &input) {
    string out;
    int consecutive = 0;
    for (char ch : input) {
        out.push_back(ch);
        if (ch == '1') {
            consecutive++;
            if (consecutive == 5) {
                out.push_back('0');
                consecutive = 0;
            }
        } else {
            consecutive = 0;
        }
    }
    return out;
}

// Unstuffs bits by removing '0's that follow five consecutive '1's
string bitUnstuff(const string &stuffed) {
    string out;
    int consecutive = 0;
    for (size_t i = 0; i < stuffed.size(); ++i) {
        char ch = stuffed[i];
        out.push_back(ch);
        if (ch == '1') {
            consecutive++;
            if (consecutive == 5) {
                // skip stuffed 0 if present
                if (i + 1 < stuffed.size() && stuffed[i + 1] == '0') {
                    ++i; // skip
                }
                consecutive = 0;
            }
        } else {
            consecutive = 0;
        }
    }
    return out;
}

int main() {
    string input;
    cout << "Enter bit stream: ";
    cin >> input;
    auto stuffed = bitStuff(input);
    cout << "Bit Stuffed: " << stuffed << "\n";
    auto unstuffed = bitUnstuff(stuffed);
    cout << "Bit Unstuffed: " << unstuffed << "\n";
    return 0;
}

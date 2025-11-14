#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Checks if a string contains only '0' and '1'
bool isBinary(const string &s) {
    for (char c : s) {
        if (c != '0' && c != '1') {
            return false;
        }
    }
    return true;
}

// Calculates hamming distance between two strings
int hammingDistance(const string &a, const string &b) {
    if (a.size() != b.size()) {
        return -1;
    }
    int d = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] != b[i]) {
            ++d;
        }
    }
    return d;
}

int main() {
    const int CODEWORDS = 8;
    vector<string> words(CODEWORDS);
    cout << "Enter " << CODEWORDS << " code words (each 4 or 8 bits):\n";

    for (int i = 0; i < CODEWORDS; ++i) {
        cout << "Code word " << (i + 1) << ": ";
        cin >> words[i];
        size_t len = words[i].size();

        if (i == 0) {
            if (len != 4 && len != 8) {
                cout << "Error: code word length must be 4 or 8 bits\n";
                return 1;
            }
        } else {
            if (len != words[0].size()) {
                cout << "Error: All code words must be the same length\n";
                return 1;
            }
        }

        if (!isBinary(words[i])) {
            cout << "Error: code word must contain only 0s and 1s\n";
            return 1;
        }
    }

    int minDist = (int)words[0].size() + 1;
    for (int i = 0; i < CODEWORDS - 1; ++i) {
        for (int j = i + 1; j < CODEWORDS; ++j) {
            int d = hammingDistance(words[i], words[j]);
            if (d < minDist) {
                minDist = d;
            }
        }
    }

    cout << "Minimum hamming distance among code words: " << minDist << "\n";
    return 0;
}

#include <iostream>
#include <string>

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
        return -1; // Error case
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
    string a, b;
    cout << "Enter string1: ";
    cin >> a;
    cout << "Enter string2: ";
    cin >> b;

    if (!((a.size() == 4 || a.size() == 8) && a.size() == b.size())) {
        cout << "Error: strings must be both 4 or both 8 bits long\n";
        return 1;
    }

    if (!isBinary(a) || !isBinary(b)) {
        cout << "Error: strings must be binary\n";
        return 1;
    }

    int d = hammingDistance(a, b);
    if (d >= 0) {
        cout << "Hamming distance = " << d << "\n";
    }
    return 0;
}

#include <iostream>
#include <vector>
#include <cstdint>

using namespace std;

// Binary addition using carry
static int binaryAdd(int a, int b) {
    while (b != 0) {
        int carry = (a & b) << 1;
        a = a ^ b;
        b = carry;
    }
    return a;
}

// Fold sum into 4 bits with end-around carry
static int wrapTo4Bit(int sum) {
    while (sum > 0xF) {
        int overflow = sum >> 4;
        sum = (sum & 0xF) + overflow;
    }
    return sum & 0xF;
}

// Compute checksum: sum all words, wrap to 4-bit, then 1's complement
static int computeChecksum(const vector<int>& data) {
    int sum = 0;
    for (int w : data) {
        sum = binaryAdd(sum, w);
    }
    sum = wrapTo4Bit(sum);
    return (~sum) & 0xF;
}

int main() {
    int n;
    cout << "Enter no of data elements : ";
    if (!(cin >> n) || n <= 0) {
        return 0;
    }
    vector<int> sender(n);

    cout << "Enter sender data elements (0-255) : ";
    for (int i = 0; i < n; ++i) {
        cin >> sender[i];
        sender[i] &= 0xF;
    }

    int checksum = computeChecksum(sender);
    cout << "Checksum for the sender : " << checksum << "\n\n";

    vector<int> receiver(n);
    cout << "Enter receiver data elements (0-255) : ";
    for (int i = 0; i < n; ++i) {
        cin >> receiver[i];
        receiver[i] &= 0xF;
    }

    vector<int> full = receiver;
    full.push_back(checksum);

    int recvChecksum = computeChecksum(full);
    cout << "Checksum of the receiver : " << recvChecksum << "\n";

    if (recvChecksum == 0) {
        cout << "Correct data transmitted.\n";
    } else {
        cout << "Error in data transmission.\n";
    }
    return 0;
}

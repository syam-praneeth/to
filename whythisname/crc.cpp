#include <iostream>
#include <string>

using namespace std;

// XOR operation for characters '0' and '1'
char exor(char a, char b) {
    return (a == b) ? '0' : '1';
}

// Generates CRC and appends it to the data
string generateCRC(string data, const string &key) {
    int datalen = (int)data.size();
    int keylen = (int)key.size();
    data.append(keylen - 1, '0');
    int codelen = datalen + keylen - 1;
    string rem = data.substr(0, keylen);

    for (int j = keylen; j <= codelen; j++) {
        if (rem[0] == '1') {
            for (int i = 1; i < keylen; i++) {
                rem[i - 1] = exor(rem[i], key[i]);
            }
        } else {
            for (int i = 1; i < keylen; i++) {
                rem[i - 1] = rem[i];
            }
        }
        if (j < codelen) {
            rem[keylen - 1] = data[j];
        }
    }
    return data.substr(0, datalen) + rem.substr(0, keylen - 1);
}

// Checks if the received data has a valid CRC
bool checkCRC(const string &received, const string &key) {
    int len = (int)received.size();
    int keylen = (int)key.size();
    string rem = received.substr(0, keylen);

    for (int j = keylen; j <= len; j++) {
        if (rem[0] == '1') {
            for (int i = 1; i < keylen; i++) {
                rem[i - 1] = exor(rem[i], key[i]);
            }
        } else {
            for (int i = 1; i < keylen; i++) {
                rem[i - 1] = rem[i];
            }
        }
        if (j < len) {
            rem[keylen - 1] = received[j];
        }
    }

    for (int i = 0; i < keylen - 1; i++) {
        if (rem[i] != '0') {
            return false;
        }
    }
    return true;
}

int main() {
    string data, key;
    cout << "Enter the data: ";
    cin >> data;
    cout << "Enter the key: ";
    cin >> key;

    string codeword = generateCRC(data, key);
    cout << "Dataword with CRC (codeword) = " << codeword << "\n";

    string received;
    cout << "\nEnter received data (data + CRC): ";
    cin >> received;

    if (checkCRC(received, key)) {
        cout << "No error detected in received data.\n";
    } else {
        cout << "Error detected in received data.\n";
    }
    return 0;
}

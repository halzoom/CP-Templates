#include <bits/stdc++.h>

using namespace std;
#define int long long

int charToVal(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'Z') return c - 'A' + 10;
    if (c >= 'a' && c <= 'z') return c - 'a' + 10; // allow lowercase too
    throw invalid_argument("Invalid digit");
}

char valToChar(int v) {
    if (v < 10) return '0' + v;
    return 'A' + (v - 10);
}

int toDecimal(const string &num, int a) {
    int value = 0;
    for (char c : num) {
        int digit = charToVal(c);
        value = value * a + digit;
    }
    return value;
}

string fromDecimal(int value, int b) {
    if (value == 0) return "0";
    string result;
    while (value > 0) {
        int digit = value % b;
        result.push_back(valToChar(digit));
        value /= b;
    }
    reverse(result.begin(), result.end());
    return result;
}

string convertBase(const string &num, int a, int b) {
    int decimalValue = toDecimal(num, a);
    return fromDecimal(decimalValue, b);
}

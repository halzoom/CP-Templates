#include <bits/stdc++.h>

using namespace std;
#define int long long
const int mod = 1e9 + 7;
int exp(int Base, int Power) {
    int Result = 1;
    while (Power) {
        if (Power & 1)
            Result = (Result * Base) % mod;
        Base = (Base * Base) % mod;
        Power >>= 1;
    }

    return Result;
}
int power(int n, string &s) {
    int answer = 1;
    for (auto c: s) {
        answer = exp(answer, 10);
        answer = answer * exp(n, c - '0') % mod;
    }
    return answer;
}
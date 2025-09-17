#include<bits/stdc++.h>
using namespace std;
#define int long long

/*
 * Your task is to count the number of different necklaces that consist of n
 * pearls and each pearl has m possible colors. Two necklaces are considered to
 * be different if it is not possible to rotate one of them so that they look
 * the same.
 * */
// 9
const int mod = 1e9 + 7;

int exp(int Base, int Power) {
    int Result = 1;
    while (Power) {
        if (Power & 1) Result = (Result * Base) % mod;
        Base = (Base * Base) % mod, Power >>= 1;
    }
    return Result;
}

signed main() {
    int n, k;
    cin >> n >> k;
    int res = 0;
    // count number of groups for each rotation
    for (int i = 1; i <= n; ++i) res = (res + exp(k, gcd(i, n))) % mod;
    res = res * exp(n, mod - 2) % mod;
    cout << res;
}
#include<bits/stdc++.h>
using namespace std;
#define int long long

// 8
pair<int, int> fib(int n) {
    if (n == 0) return {0, 1};
    auto p = fib(n >> 1);
    int c = p.first * ((2 * p.second % mod - p.first + 4 * mod) % mod) % mod;
    int d = (p.first * p.first % mod + p.second * p.second % mod) % mod;
    if (n & 1) return {d, (c + d) % mod};
    return {c, d};
}
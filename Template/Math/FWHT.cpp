#include<bits/stdc++.h>

using namespace std;
#define int long long

// size of arrays must be power of 2
void fwht(vector<__int128> &a, int inv, int f) {
    int sz = a.size();
    for (int len = 1; 2 * len <= sz; len <<= 1) {
        for (int i = 0; i < sz; i += 2 * len) {
            for (int j = 0; j < len; j++) {
                __int128 x = a[i + j];
                __int128 y = a[i + j + len];
                if (f == 0) {
                    if (!inv) a[i + j] = y, a[i + j + len] = x + y;
                    else a[i + j] = y - x, a[i + j + len] = x;
                } else if (f == 1) {
                    if (!inv) a[i + j + len] = x + y;
                    else a[i + j + len] = y - x;
                } else {
                    a[i + j] = x + y;
                    a[i + j + len] = x - y;
                }
            }
        }
    }
}

// T[x] = sum of(frq[y] * g[x @ y]) over all masks p , where @ means any operation
// g[x^y] = (−1)^popcount(x&y) -> for XOR convolution
// g[x|y] = (y & x == y) -> for OR convolution -> zeta transform
// g[x&y] = (y & x == x) -> for AND convolution -> zeta transform
vector<__int128> multiply(vector<__int128> a, vector<__int128> b, int f) {  // 0:AND, 1:OR, 2:XOR
    int sz = a.size();
    fwht(a, 0, f);
    fwht(b, 0, f);
    vector<__int128> c(sz);
    for (int i = 0; i < sz; ++i) { c[i] = a[i] * b[i]; }
    fwht(c, 1, f);
    if (f) {
        for (int i = 0; i < sz; ++i) { c[i] = c[i] / sz; }
    }
    return c;
}

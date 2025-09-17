#include <bits/stdc++.h>

using namespace std;
#define int long long
const int mod = 998244353, N = 1 << 22;

int add(int x, int y) {
    x += y;
    if (x >= mod) x -= mod;
    return x;
}

int sub(int x, int y) {
    x -= y;
    if (x < 0) x += mod;
    return x;
}

int mul(int x, int y) {
    return ((x * y) % mod);
}

int exp(int a, int e) {
    int r = 1;
    while (e) {
        if (e & 1) r = mul(r, a);
        a = mul(a, a);
        e >>= 1;
    }
    return r;
}

void fwht(vector<int> &a, int inv, int f) {
    int sz = a.size();
    for (int len = 1; 2 * len <= sz; len <<= 1) {
        for (int i = 0; i < sz; i += 2 * len) {
            for (int j = 0; j < len; j++) {
                int x = a[i + j];
                int y = a[i + j + len];
                if (f == 0) { // AND
                    if (!inv) {
                        a[i + j] = y;
                        a[i + j + len] = add(x, y);
                    } else {
                        a[i + j] = sub(y, x);
                        a[i + j + len] = x;
                    }
                } else if (f == 1) { // OR
                    if (!inv) {
                        a[i + j + len] = add(x, y);
                    } else {
                        a[i + j + len] = sub(y, x);
                    }
                } else { // XOR
                    a[i + j] = add(x, y);
                    a[i + j + len] = sub(x, y);
                }
            }
        }
    }
}

vector<int> multiply(vector<int> a, vector<int> b, int f) { // 0:AND, 1:OR, 2:XOR
    int sz = a.size();
    fwht(a, 0, f);
    fwht(b, 0, f);
    vector<int> c(sz);
    for (int i = 0; i < sz; ++i) {
        c[i] = mul(a[i], b[i]);
    }
    fwht(c, 1, f);
    if (f == 2) { // XOR needs division by sz
        int inv_sz = exp(sz, mod - 2);
        for (int i = 0; i < sz; ++i) {
            c[i] = mul(c[i], inv_sz);
        }
    }
    return c;
}

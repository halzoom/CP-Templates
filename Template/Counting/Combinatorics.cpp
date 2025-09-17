#include <bits/stdc++.h>

using namespace std;
#define int long long

const int mod = 1e9 + 7, N = 2e6 + 6;
struct Combinatorics {
    vector<int> fact, inv;
    // 9
    Combinatorics(int n) {
        fact.assign(n + 1, 1);
        inv.assign(n + 1, 1);
        for (int i = 1; i <= n; ++i) fact[i] = (fact[i - 1] * i) % mod;
        // fact[i + 1] = fact[i] * (i + 1);
        // invfact[i]  = invfact[i + 1] * (i + i);
        inv[n] = modInv(fact[n]);
        for (int i = n - 1; i >= 0; --i) inv[i] = (inv[i + 1] * (i + 1)) % mod;
    }
    // 8
    int modInv(int n) { return exp(n, mod - 2); }
    int exp(int base, int pow) {
        int res = 1;
        while (pow) {
            if (pow & 1) res = (res * base) % mod;
            base = (base * base) % mod, pow >>= 1;
        }
        return res;
    }
    // 4
    int nCr(int n, int r) {
        if (r < 0 or n < r) return 0;
        return fact[n] * inv[n - r] % mod * inv[r] % mod;
    }
    // 4
    int nPr(int n, int r) {
        if (r < 0 or n < r) return 0;
        return fact[n] * inv[n - r] % mod;
    }
    int starsABars(int n, int m) { return nCr(n + m - 1, m); }
    // 1, 1, 2, 5, 14, 42, 132 , 429
    int Catalan(int n) { return modInv(n + 1) * nCr(2 * n, n) % mod; }
    int invCatalan(int n) { return nCr(2 * n, n - 1); }
} comb(N);
// 5
//  nCr is odd if r is a submask of n
int nCr_Parity(int n, int r) {
    if (n < r) return 0;
    return (n & r) == r;
}
// 5
int C(int n, int k) {
    double res = 1;
    for (int i = 1; i <= k; ++i) res = res * (n - k + i) / i;
    return (int)(res + 0.01);
// 14
vector<vector<int>> Pascal(int n) {
    vector<vector<int>> C(n + 1, vector<int>(n + 1));
    C[0][0] = 1;
    for (int i = 1; i <= n; ++i) {
        C[i][0] = C[i][i] = 1;
        for (int k = 1; k < i; ++k) C[i][k] = C[i - 1][k - 1] + C[i - 1][k];
    }
    return C;
}
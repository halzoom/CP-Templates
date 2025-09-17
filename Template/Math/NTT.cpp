#include<bits/stdc++.h>
using namespace std;
#define int long long

const int mod = (119 << 23) + 1, root = 62;  // = 998244353
// For p < 2^30 there is also e.g. 5 << 25, 7 << 26, 479 << 21
// and 483 << 21 (same root). The last two are > 10^9.
int modpow(int b, int e) {
    int ans = 1;
    for (; e; b = b * b % mod, e /= 2)
        if (e & 1) ans = ans * b % mod;
    return ans;
}

// Primitive Root of the mod of form 2^a * b + 1
int generator() {
    vector<int> fact;
    int phi = mod - 1, n = phi;
    for (int i = 2; i * i <= n; ++i)
        if (n % i == 0) {
            fact.push_back(i);
            while (n % i == 0) n /= i;
        }
    if (n > 1) fact.push_back(n);
    for (int res = 2; res <= mod; ++res) {
        bool ok = true;
        for (size_t i = 0; i < fact.size() && ok; ++i) ok &= modpow(res, phi / fact[i]) != 1;
        if (ok) return res;
    }
    return -1;
}

int modpow(int b, int e, int m) {
    int ans = 1;
    for (; e; b = (int) b * b % m, e /= 2)
        if (e & 1) ans = (int) ans * b % m;
    return ans;
}

void ntt(vector<int> &a) {
    int n = (int) a.size(), L = 31 - __builtin_clz(n);
    vector<int> rt(2, 1);  // erase the static if you want to use two moduli;
    for (int k = 2, s = 2; k < n;
         k *= 2, s++) {  // erase the static if you want to use two moduli;
        rt.resize(n);
        int z[] = {1, modpow(root, mod >> s, mod)};
        for (int i = k; i < 2 * k; ++i) rt[i] = (int) rt[i / 2] * z[i & 1] % mod;
    }
    vector<int> rev(n);
    for (int i = 0; i < n; ++i) { rev[i] = (rev[i / 2] | (i & 1) << L) / 2; }
    for (int i = 0; i < n; ++i)
        if (i < rev[i]) swap(a[i], a[rev[i]]);
    for (int k = 1; k < n; k *= 2) {
        for (int i = 0; i < n; i += 2 * k) {
            for (int j = 0; j < k; ++j) {
                int z = (int) rt[j + k] * a[i + j + k] % mod, &ai = a[i + j];
                a[i + j + k] = ai - z + (z > ai ? mod : 0);
                ai += (ai + z >= mod ? z - mod : z);
            }
        }
    }
}

vector<int> multiply(const vector<int> &a, const vector<int> &b) {
    if (a.empty() || b.empty()) return {};
    int s = (int) a.size() + (int) b.size() - 1, B = 32 - __builtin_clz(s), n = 1 << B;
    int inv = modpow(n, mod - 2, mod);
    vector<int> L(a), R(b), out(n);
    L.resize(n), R.resize(n);
    ntt(L), ntt(R);
    for (int i = 0; i < n; ++i) out[-i & (n - 1)] = (int) L[i] * R[i] % mod * inv % mod;
    ntt(out);
    return {out.begin(), out.begin() + s};
}

vector<int> shift_poly(vector<int> &p, int k) {  // using NTT
    k = (k % mod + mod) % mod;
    int n = p.size();
    vector<int> fact(n, 1), inv(n, 1);
    for (int i = 1; i < n; ++i) {
        fact[i] = fact[i - 1] * i % mod;
        inv[i] = modpow(fact[i], mod - 2);
    }
    vector<int> p1(n), p2(n);
    for (int i = 0; i < n; i++) p1[i] = fact[i] * p[i] % mod;
    int curr = 1;
    for (int i = 0; i < n; i++) {
        p2[n - i - 1] = inv[i] * curr % mod;
        curr = curr * k % mod;
    }
    vector<int> res = multiply(p1, p2);
    vector<int> ans;
    for (int i = n - 1; i < res.size(); i++) ans.push_back(res[i] * inv[i - (n - 1)] % mod);
    return ans;
}

int CRT(int a, int m1, int b, int m2) {
    __int128 m = m1 * m2;
    int ans =
            a * m2 % m * modpow(m2, m1 - 2, m1) % m + m1 * b % m * modpow(m1, m2 - 2, m2) % m;
    return ans % m;
}

int mod, root, desired_mod = 1000000007;
const int mod1 = 167772161;
const int mod2 = 469762049;
const int mod3 = 754974721;
const int root1 = 3;
const int root2 = 3;
const int root3 = 11;

int CRT(int a, int b, int c, int m1, int m2, int m3) {
    __int128 M = (__int128) m1 * m2 * m3;
    int M1 = (int) m2 * m3;
    int M2 = (int) m1 * m3;
    int M3 = (int) m2 * m1;
    int M_1 = modpow(M1 % m1, m1 - 2, m1);
    int M_2 = modpow(M2 % m2, m2 - 2, m2);
    int M_3 = modpow(M3 % m3, m3 - 2, m3);
    __int128 ans = (__int128) a * M1 * M_1;
    ans += (__int128) b * M2 * M_2;
    ans += (__int128) c * M3 * M_3;
    return (ans % M) % desired_mod;
}

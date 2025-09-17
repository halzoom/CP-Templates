#include<bits/stdc++.h>
using namespace std;
#define int long long

/*
 * Given a string, your task is to determine the longest palindromic substring
 * of the string. For example, the longest palindrome in aybabtu is bab.
 * */
const int M = 2, N = 1e6 + 5;
int B[M], mods[M], pw[M][N];

struct Hashing {
    vector <array<int, M>> prefix, suffix;

    void build() {
        if (B[0]) return;
        mt19937 rng(chrono::system_clock::now().time_since_epoch().count());
        auto rnd = [&](int a, int b) { return a + rng() % (b - a + 1); };
        auto check = [&](int x) {
            for (int i = 2; i * i <= x; ++i)
                if (x % i == 0) return true;
            return false;
        };
        for (int i = 0; i < M; ++i) {
            B[i] = rnd(100, 500);
            mods[i] = rnd(1e9, 2e9);
            while (check(mods[i])) mods[i]--;
            pw[i][0] = 1;
            for (int j = 1; j < N; ++j) pw[i][j] = pw[i][j - 1] * B[i] % mods[i];
        }
    }

    Hashing(string &s, bool withSuffix = false) {
        build();
        InitPrefix(s);
        if (withSuffix) InitSuffix(s);
    }

    void InitPrefix(string &s) {
        int n = (int) s.size();
        prefix.assign(n, {});
        array<int, M> hash{};
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < M; ++j) hash[j] = (hash[j] * B[j] + s[i]) % mods[j];
            prefix[i] = hash;
        }
    }

    void InitSuffix(string &s) {
        int n = (int) s.size();
        suffix.assign(n, {});
        array<int, M> hash{};
        for (int i = n - 1; i >= 0; --i) {
            for (int j = 0; j < M; ++j) hash[j] = (hash[j] * B[j] + s[i]) % mods[j];
            suffix[i] = hash;
        }
    }

    array<int, M> get_substr_Hash(int l, int r) {
        if (l == 0) return prefix[r];
        array<int, M> res = prefix[r];
        for (int i = 0; i < M; ++i) {
            res[i] -= prefix[l - 1][i] * pw[i][r - l + 1] % mods[i];
            if (res[i] < 0) res[i] += mods[i];
        }
        return res;
    }

    array<int, M> get_reverse_Hash(int l, int r) {
        if (r + 1 == suffix.size()) return suffix[l];
        array<int, M> res = suffix[l];
        for (int i = 0; i < M; ++i) {
            res[i] -= suffix[r + 1][i] * pw[i][r - l + 1] % mods[i];
            if (res[i] < 0) res[i] += mods[i];
        }
        return res;
    }

    bool isPal(int l, int r) { return get_substr_Hash(l, r) == get_reverse_Hash(l, r); }
};

signed main() {
    string s;
    cin >> s;
    Hashing hash(s, true);
    auto can = [&](int c, int L, bool even) {
        if (c - L < 0 or c + L + even >= s.size()) return false;
        return hash.isPal(c - L, c + L + even);
    };
    int ans = 1;
    for (int center = 0; center < s.size() - 1; ++center) {
        if (s[center] == s[center + 1]) {
            int l = 0, r = s.size() + 5, mid = 0, res = 0;
            while (l <= r) {
                mid = (l + r) / 2;
                if (can(center, mid, true)) {
                    res = 2 * mid + 2;
                    l = mid + 1;
                } else {
                    r = mid - 1;
                }
            }
            ans = max(res, ans);
        }
        int l = 0, r = s.size() + 5, mid = 0, res = 0;
        while (l <= r) {
            mid = (l + r) / 2;
            if (can(center, mid, false)) {
                res = 2 * mid + 1;
                l = mid + 1;
            } else {
                r = mid - 1;
            }
        }
        ans = max(res, ans);
    }
    for (int i = 0; i + ans <= s.size(); ++i) {
        if (hash.isPal(i, i + ans - 1)) {
            cout << s.substr(i, ans);
            return 0;
        }
    }
}

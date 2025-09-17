#include<bits/stdc++.h>
using namespace std;
#define int long long

const int lg = 64, mod = 1e9 + 7, inf = 1e9 + 7;

struct Basis {
    int size = 0, n = 0;
    int basis[lg];

    Basis() {
        size = n = 0;
        for (int i = lg - 1; i >= 0; --i) basis[i] = 0;
    }

    bool insert(int x) {
        n++;
        for (int i = lg - 1; i >= 0; --i) {
            if (((x >> i) & 1) == 0) continue;
            if (not basis[i]) {
                basis[i] = x, ++size;
                return true;
            }
            x = (x ^ basis[i]);
        }
        return false;
    }

    bool merge(Basis &w) {
        bool repeat = false;
        for (int i = 0; i < lg; ++i)
            if (w.basis[i] > 0 and not insert(w.basis[i])) repeat = true;
        return repeat;
    }

    bool can(int x) { // if n > size then you can get x = 0
        for (int i = lg - 1; i >= 0; --i)
            if (basis[i] and (x & (1LL << i))) x = (x ^ basis[i]);
        return x == 0;
    }

    int count_xors(int x) { // NOTE: Add exponentiation template.
        return (can(x) ? (exp(2, n - size) + mod - 1) % mod : 0);
    }

    int kth(int k) {
        int x = 0;
        for (int i = lg - 1, c = size; i >= 0; --i) {
            if (not basis[i]) continue;
            --c;
            if (x & (1LL << i)) {
                if ((1LL << c) >= k) x = (x ^ basis[i]);
                else k = k - (1LL << c);
            } else if (k > (1LL << c)) {
                x = (x ^ basis[i]), k = k - (1LL << c);
            }
        }
        return x;
    }

    int get_max() {
        int ans = 0;
        for (int i = lg - 1; i >= 0; --i) {
            if (basis[i] && not(ans & (1LL << i))) ans = (ans ^ basis[i]);
        }
        return ans;
    }

    void AND(int x) {
        vector<int> upd;
        for (int i = lg - 1; i >= 0; --i) {
            basis[i] = (basis[i] & x);
            if (basis[i]) upd.push_back(basis[i]);
            basis[i] = 0;
        }
        for (int &val: upd) insert(val);
    }

    void OR(int x) {
        vector<int> upd;
        for (int i = lg - 1; i >= 0; --i) {
            basis[i] = (basis[i] | x);
            if (basis[i]) upd.push_back(basis[i]);
            basis[i] = 0;
        }
        for (int &val: upd) insert(val);
    }
};

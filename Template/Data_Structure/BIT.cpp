#include <bits/stdc++.h>

using namespace std;
#define int long long
const int inf = 1e18;

struct BIT {
    int n;
    vector<int> bit1, bit2;

    BIT(int size) : n(size) {
        bit1.assign(n + 2, 0);
        bit2.assign(n + 2, 0);
    }

    void add(vector<int> &bit, int idx, int val) {
        idx++;
        while (idx <= n) {
            bit[idx] += val;
            idx += idx & -idx;
        }
    }

    int get(const vector<int> &bit, int idx) {
        idx++;
        int res = 0;
        while (idx > 0) {
            res += bit[idx];
            idx -= idx & -idx;
        }
        return res;
    }

    void update(int l, int r, int val) {
        add(bit1, l, val);
        add(bit1, r + 1, -val);
        add(bit2, l, val * (l - 1));
        add(bit2, r + 1, -val * r);
    }

    int get(int i) {
        return get(bit1, i) * i - get(bit2, i);
    }

    int get(int l, int r) {
        return get(r) - get(l - 1);
    }
};


struct BIT {
    int n;
    vector<int> t;

    BIT(int n) : n(n), t(n, 0) {}

    void add(int i, int x) {
        for (; i < n; i |= i + 1) t[i] += x;
    }

    int get(int i) {
        int res = 0;
        for (; i >= 0; i = (i & (i + 1)) - 1) res += t[i];
        return res;
    }

    int get(int l, int r) { return get(r) - (l ? get(l - 1) : 0); }

    // find smallest index such that prefix sum >= k (k is 1-based)
    int kth(int k) {
        int pos = -1;
        int sum = 0;
        int LOG = 31 - __builtin_clz(n);
        for (int i = LOG; i >= 0; i--) {
            int nxt = pos + (1 << i);
            if (nxt < n and sum + t[nxt] < k) {
                sum += t[nxt];
                pos = nxt;
            }
        }
        return pos + 1;
    }
};
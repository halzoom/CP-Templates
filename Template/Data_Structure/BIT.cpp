#include <bits/stdc++.h>

using namespace std;
#define ll long long
const ll inf = 1e18;

struct BIT {
    int n;
    vector<ll> bit1, bit2;

    BIT(int size) : n(size) {
        bit1.assign(n + 2, 0);
        bit2.assign(n + 2, 0);
    }

    void add(vector<ll> &bit, int idx, ll val) {
        idx++;
        while (idx <= n) {
            bit[idx] += val;
            idx += idx & -idx;
        }
    }

    ll query_bit(const vector<ll> &bit, int idx) const {
        idx++;
        ll res = 0;
        while (idx > 0) {
            res += bit[idx];
            idx -= idx & -idx;
        }
        return res;
    }

    void add(int i, ll val) {
        update(i, i, val);
    }

    void update(int l, int r, ll val) {
        add(bit1, l, val);
        add(bit1, r + 1, -val);
        add(bit2, l, val * (l - 1));
        add(bit2, r + 1, -val * r);
    }

    ll prefix_sum(int i) const {
        return query_bit(bit1, i) * i - query_bit(bit2, i);
    }

    ll range_query(int l, int r) const {
        return prefix_sum(r) - prefix_sum(l - 1);
    }

    ll point_query(int i) const {
        return range_query(i, i);
    }
};

struct BIT {
    int n;
    vector<int> t;

    BIT(int n): n(n), t(n + 1) {}

    void add(int i, int x) {
        for(; i < n; i |= i + 1) t[i] += x;
    }

    int get(int i) {
        int res = 0;
        for(; i >= 0; i = (i & i + 1) - 1) res += t[i];
        return res;
    }

    int get(int l, int r) { return get(r) - get(l - 1); }
};
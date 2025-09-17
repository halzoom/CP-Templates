#include<bits/stdc++.h>
using namespace std;
#define int long long

const int N = 1e5 + 1, B = 4500, C = N / B + 1;

struct DistinctCounter {
    int cnt[N * 2];  // Frequency of each element
    int distinct;    // Number of distinct elements
    DistinctCounter() {
        memset(cnt, 0, sizeof cnt);
        distinct = 0;
    }

    void add(int x) {
        // If x was not present, increment distinct count
        if (cnt[x] == 0) distinct++;
        cnt[x]++;
    }

    void del(int x) {
        cnt[x]--;
        // If x is no longer present, decrement distinct count
        if (cnt[x] == 0) distinct--;
    }

    int get() {
        // Return the number of distinct elements
        return distinct;
    }
} t[C * (C + 1) / 2 + 10], ds;

int st[C], en[C], BC = 0;
int a[N], I[N];

int query(int l, int r) {
    int L = l / B, R = r / B;
    // Adjust R if the range doesn't fully cover the last block
    if (r != en[R]) R--;
    // Adjust L if the range doesn't fully cover the first block
    if (l != st[L]) L++;
    if (R < L) {
        // If the range is within a single block, handle it directly
        for (int i = l; i <= r; i++) ds.add(a[i]);
        int ans = ds.get();
        for (int i = l; i <= r; i++) ds.del(a[i]);
        return ans;
    }
    // Otherwise, use the precomputed distinct counts for the fully covered blocks
    int id = I[L * BC + R];
    // Add elements from the left partial block
    for (int i = l; i < st[L]; i++) t[id].add(a[i]);
    // Add elements from the right partial block
    for (int i = en[R] + 1; i <= r; i++) t[id].add(a[i]);
    int ans = t[id].get();
    // Remove elements from the left partial block
    for (int i = l; i < st[L]; i++) t[id].del(a[i]);
    // Remove elements from the right partial block
    for (int i = en[R] + 1; i <= r; i++) t[id].del(a[i]);
    return ans;
}

inline void upd(int id, int pos, int val) {
    t[id].del(a[pos]);  // Remove the old value
    t[id].add(val);     // Add the new value
}

map<int, int> mp;
int nxt = 0;

int get(int x) {  // Coordinate compression
    return mp.count(x) ? mp[x] : mp[x] = ++nxt;
}

signed main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, q;
    cin >> n >> q;
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        a[i] = get(a[i]);  // Compress coordinates
    }
    for (int i = 0; i < n; i++) {
        if (i % B == 0) st[i / B] = i, BC++;              // Start of each block
        if (i % B == B - 1 || i == n - 1) en[i / B] = i;  // End of each block
    }
    int nw = 0;
    for (int i = 0; i < BC; i++) {
        for (int j = i; j < BC; j++) {
            int id = nw;
            I[i * BC + j] = nw++;  // Map block range to ID
            // Precompute distinct counts for block ranges
            for (int p = st[i]; p <= en[j]; p++) t[id].add(a[p]);
        }
    }
    while (q--) {
        int ty;
        cin >> ty;
        if (ty == 2) {
            int l, r;
            cin >> l >> r;
            --l;
            --r;
            cout << n - query(l, r) << '\n';  // Handle query
        } else {
            int pos, val;
            cin >> pos >> val;
            --pos;
            val = get(val);  // Compress the new value
            for (int i = 0; i < BC; i++) {
                for (int j = i; j < BC; j++) {  // Update all relevant block ranges
                    if (st[i] <= pos && pos <= en[j]) upd(I[i * BC + j], pos, val);
                }
            }
            a[pos] = val;  // Update the array
        }
    }
    return 0;
}
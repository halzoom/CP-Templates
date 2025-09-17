#include<bits/stdc++.h>
using namespace std;
#define int long long

/*
 *You are given a graph with n vertices and m undirected edges.
 * Write a program that processes k queries of the form (li,ri):
 * the answer for the i-th query is the number of connected components
 * if we remove all the edges from the graph except edges with indices from li
 *to ri inclusive. The queries should be answered independently. In other words,
 *to answer the i-th query, you should consider a graph that has n vertices and
 *ri-li+1 edges.
 * */
struct DSU_RollBack {
    vector<int> parent, size, checkPoint;
    vector <pair<int, int>> update;
    int component;

    DSU_RollBack(int n) {
        parent.assign(n + 1, {});
        size.assign(n + 1, 1);
        component = n;
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        while (x != parent[x]) x = parent[x];
        return x;
    }

    void snapShot() { checkPoint.emplace_back(update.size()); }

    void union_sets(int a, int b) {
        a = find(a), b = find(b);
        if (a == b) return;
        if (size[a] < size[b]) swap(a, b);
        parent[b] = a;
        size[a] += size[b];
        component--;
        update.emplace_back(a, b);
    }

    void RollBack() {
        while (checkPoint.back() != update.size()) {
            auto [a, b] = update.back();
            update.pop_back();
            parent[b] = b;
            size[a] -= size[b];
            component++;
        }
        checkPoint.pop_back();
    }
};

signed main() {
    int n, m;
    cin >> n >> m;
    vector <array<int, 2>> edges(m);
    for (int i = 0; i < m; ++i) cin >> edges[i][0] >> edges[i][1];
    int B = (int) sqrt(m) + 1;
    vector <array<int, 3>> query[B];  // r , l , i;
    int k;
    cin >> k;
    vector<int> ans(k);
    for (int i = 0, l, r; i < k; ++i) {
        cin >> l >> r, --l, --r;
        if (r - l + 1 <= B) {
            DSU_RollBack ds(n);
            for (int j = l; j <= r; ++j) { ds.union_sets(edges[j][0], edges[j][1]); }
            ans[i] = ds.component;
            continue;
        }
        query[l / B].push_back({r, l, i});
    }
    for (int i = 0; i < B; ++i) {
        if (query[i].empty()) continue;
        sort(query[i].begin(), query[i].end());
        int r = (i + 1) * B - 1;
        DSU_RollBack ds(n);
        ds.union_sets(edges[r][0], edges[r][1]);
        for (auto [rq, lq, iq]: query[i]) {
            int l = (i + 1) * B - 1;
            while (rq > r) {
                ++r;
                ds.union_sets(edges[r][0], edges[r][1]);
            }
            ds.snapShot();
            while (lq < l) {
                --l;
                ds.union_sets(edges[l][0], edges[l][1]);
            }
            ans[iq] = ds.component;
            ds.RollBack();
        }
    }
    for (auto i: ans) { cout << i << '\n'; }
    return 0;
}

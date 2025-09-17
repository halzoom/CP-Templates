#include<bits/stdc++.h>
using namespace std;
#define int long long

/*
 * You are given an empty undirected graph with n vertices. You have to answer
 * the queries of three types:
 * 1. "+ u v" - add an undirected edge u-v to the graph.
 * 2. "- u v" - remove an undirected edge u-v from the graph.
 * 3. "?" - calculate the number of connected components in the graph
 * */
struct DSU {
    vector<int> parent, size, checkPoint;
    vector <pair<int, int>> update;
    int component;

    DSU(int n) {
        parent.assign(n + 1, {});
        size.assign(n + 1, 1);
        component = n;
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (x == parent[x]) return x;
        return find(parent[x]);
    }

    void union_sets(int a, int b) {
        a = find(a), b = find(b);
        if (a == b) return;
        if (size[a] < size[b]) swap(a, b);
        parent[b] = a;
        size[a] += size[b];
        component--;
        update.emplace_back(a, b);
    }

    void snapshot() { checkPoint.push_back(update.size()); }

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

const int N = 3e5 + 5;
vector <pair<int, int>> tree[N * 4];
#define mid ((l + r) / 2)
#define LF (2 * node + 1)
#define RT (2 * node + 2)

void update(int node, int l, int r, int Lx, int Rx, pair<int, int> x) {
    if (r < Lx or Rx < l) return;
    if (Lx <= l and r <= Rx) {
        tree[node].push_back(x);
        return;
    }
    update(LF, l, mid, Lx, Rx, x);
    update(RT, mid + 1, r, Lx, Rx, x);
}

DSU ds(N);

void dfs(int node, int l, int r) {
    ds.snapshot();
    for (auto [a, b]: tree[node]) { ds.union_sets(a, b); }
    if (l == r) {
        cout << ds.component << '\n';
    } else {
        dfs(LF, l, mid);
        dfs(RT, mid + 1, r);
    }
    ds.RollBack();
}

signed main() {
    int n, m;
    cin >> n >> m;
    if (m == 0) return 0;
    map<pair<int, int>, int> query;  // a , b , time
    vector <array<int, 4>> add;       // a , b , start , end
    ds = DSU(n);
    char t;
    int a, b, Q = 0;
    for (int i = 0; i < m; ++i) {
        cin >> t;
        if (t == '?') {
            Q++;
        } else if (t == '-') {
            cin >> a >> b;
            if (a > b) swap(a, b);
            int start = query[{a, b}];
            if (start < Q) add.push_back({a, b, start, Q - 1});
            query.erase({a, b});
        } else {
            cin >> a >> b;
            if (a > b) swap(a, b);
            query[{a, b}] = Q;
        }
    }
    for (auto &[x, start]: query) { update(0, 0, Q - 1, start, Q - 1, x); }
    for (auto &[u, v, start, end]: add) { update(0, 0, Q - 1, start, end, {u, v}); }
    dfs(0, 0, Q - 1);
    return 0;
}

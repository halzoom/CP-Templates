#include<bits/stdc++.h>
using namespace std;
#define int long long

/*
 * You are given an empty undirected graph on n vertices.
 * Each vertex is colored in one of two colors 0 or 1
 * such that each edge connects the vertices with different colors.
 * There are two types of queries:
 * 1. You are given two vertices x and y from different connected
 * components: add an edge (x,y) to the graph, and change the colors
 * to satisfy the condition.
 * 2. You are given two vertices x and y from one connected
 * component: answer whether they are of the same color.
 * */
struct DSU {
    vector<int> parent, size, depth;
    int component;

    DSU(int n) {
        parent.assign(n + 1, {});
        size.assign(n + 1, 1);
        depth.assign(n + 1, 0);
        component = n;
        iota(parent.begin(), parent.end(), 0);
    }

    pair<int, int> find(int x) {
        if (x == parent[x]) return {x, 0};
        auto v = find(parent[x]);
        parent[x] = v.first;   // Path compression
        depth[x] += v.second;  // update depth
        return {v.first, depth[x]};
    }

    void union_set(int a, int b) {
        int rootA = find(a).first;
        int rootB = find(b).first;
        if (rootA == rootB) return;
        if (size[rootA] < size[rootB]) swap(rootA, rootB);
        parent[rootB] = rootA;
        depth[rootB] = (depth[a] + depth[b] + 1) % 2;
        size[rootA] += size[rootB];
    }

    bool isBip(int a, int b) { return find(a).second % 2 == find(b).second % 2; }

    bool isConnected(int a, int b) { return find(a).first == find(b).first; }
};

signed main() {
    int n, m, shift = 0;
    cin >> n >> m;
    DSU dsu(n);
    for (int i = 0, t, a, b; i < m; ++i) {
        cin >> t >> a >> b;
        int x = (a + shift) % n;
        int y = (b + shift) % n;
        if (t == 0) {
            dsu.union_set(x, y);
        } else {
            bool ok = dsu.isBip(x, y);
            if (ok) {
                shift = (shift + 1) % n;
                cout << "YES\n";
            } else {
                cout << "NO\n";
            }
        }
    }
}

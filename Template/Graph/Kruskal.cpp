#include<bits/stdc++.h>
using namespace std;
#define int long long

struct DSU {
    vector<int> parent, size;
    int component;

    DSU(int n) {
        parent.assign(n + 1, {});
        size.assign(n + 1, 1);
        component = n;
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (x == parent[x]) return x;
        return parent[x] = find(parent[x]);
    }

    void union_sets(int a, int b) {
        a = find(a), b = find(b);
        if (a == b) return;
        if (size[a] < size[b]) swap(a, b);
        parent[b] = a;
        size[a] += size[b];
        component--;
    }

    bool isConnected(int a, int b) { return find(a) == find(b); }
};

int Kruskal(int n, vector <array<int, 3>> &edges) {
    sort(edges.begin(), edges.end());
    DSU ds(n);
    int sum = 0;
    for (auto &edge: edges) {
        if (!ds.isConnected(edge[1], edge[2])) {
            sum += edge[0];
            ds.union_sets(edge[1], edge[2]);
        }
    }
    return sum;
}

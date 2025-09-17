#include<bits/stdc++.h>
using namespace std;
#define int long long

struct DSU {
    vector<int> parent, size;

    DSU(int n) {
        parent.assign(n + 1, {});
        size.assign(n + 1, 1);
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (x == parent[x]) return x;
        return parent[x] = find(parent[x]);
    }

    bool union_set(int a, int b) {
        a = find(a), b = find(b);
        if (a == b) return false;
        if (size[a] < size[b]) swap(a, b);
        parent[b] = a;
        size[a] += size[b];
        return true;
    }

    bool isConnected(int a, int b) { return find(a) == find(b); }
};

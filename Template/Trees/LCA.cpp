#include<bits/stdc++.h>

using namespace std;
#define int long long

struct LCA {
    vector<vector<int>> ancestor, minE;
    vector<int> level;
    int LG;

    LCA(vector<vector<pair<int, int>>> &adj) {
        int n = (int) adj.size();
        LG = __lg(n) + 1;
        ancestor.assign(LG, vector<int>(n));
        minE.assign(LG, vector<int>(n));
        level.assign(n, {});
        build(1, 0, adj);
        for (int i = 1; i < LG; ++i) {
            for (int u = 1; u < n; ++u) {
                ancestor[i][u] = ancestor[i - 1][ancestor[i - 1][u]];
                minE[i][u] = min(minE[i - 1][u], minE[i - 1][ancestor[i - 1][u]]);
            }
        }
    }

    void build(int u, int p, vector<vector<pair<int, int>>> &adj) {
        for (auto [v, w]: adj[u]) {
            if (v == p) continue;
            level[v] = level[u] + 1;
            ancestor[0][v] = u;
            minE[0][v] = w;
            build(v, u, adj);
        }
    }

    int KthAnc(int u, int k) {
        for (int i = 0; k; ++i, k >>= 1) {
            if (k & 1) u = ancestor[i][u];
        }
        return u;
    }

    int getLCA(int u, int v) {
        if (level[u] > level[v]) swap(u, v);
        int k = level[v] - level[u];
        v = KthAnc(v, k);
        if (v == u) return v;
        for (int i = LG - 1; ~i; --i) {
            if (ancestor[i][v] != ancestor[i][u]) {
                v = ancestor[i][v];
                u = ancestor[i][u];
            }
        }
        return ancestor[0][u];
    }

    int getDist(int u, int v) {
        int lca = getLCA(u, v);
        return level[u] + level[v] - 2 * level[lca];
    }

    int query(int u, int v) {
        if (level[u] > level[v]) swap(u, v);
        int k = level[v] - level[u];
        int res = inf;
        for (int i = 0; k; ++i, k >>= 1) {
            if (k & 1) {
                res = min(res, minE[i][v]);
                v = ancestor[i][v];
            }
        }
        if (v == u) return res;
        for (int i = LG - 1; ~i; --i) {
            if (ancestor[i][v] != ancestor[i][u]) {
                res = min({res, minE[i][v], minE[i][u]});
                v = ancestor[i][v];
                u = ancestor[i][u];
            }
        }
        res = min({res, minE[0][v], minE[0][u]});
        return res;
    }
    bool is_on_path(int a, int b, int c) {
        int dab = getDist(a, b);
        int dac = getDist(a, c);
        int dcb = getDist(c, b);
        return dac + dcb == dab;
    }
};
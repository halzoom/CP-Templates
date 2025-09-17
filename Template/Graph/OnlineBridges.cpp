#include<bits/stdc++.h>
using namespace std;
#define int long long

struct OnlineBridges {
private:
    vector<int> parent, twoECC, dsu_cc, size, lastVisit;
    int bridgeCount = 0;
    int lcaIteration = 0;

    int find2ECC(int v) {
        if (v == -1) return -1;
        return twoECC[v] == v ? v : twoECC[v] = find2ECC(twoECC[v]);
    }

    int findCC(int v) {
        v = find2ECC(v);
        return dsu_cc[v] == v ? v : dsu_cc[v] = findCC(dsu_cc[v]);
    }

    void makeRoot(int v) {
        int root = v;
        int child = -1;
        while (v != -1) {
            int p = find2ECC(parent[v]);
            parent[v] = child;
            dsu_cc[v] = root;
            child = v;
            v = p;
        }
        size[root] = size[child];
    }

    void mergePath(int a, int b) {
        ++lcaIteration;
        vector<int> path_a, path_b;
        int lca = -1;
        while (lca == -1) {
            if (a != -1) {
                a = find2ECC(a);
                path_a.push_back(a);
                if (lastVisit[a] == lcaIteration) {
                    lca = a;
                    break;
                }
                lastVisit[a] = lcaIteration;
                a = parent[a];
            }
            if (b != -1) {
                b = find2ECC(b);
                path_b.push_back(b);
                if (lastVisit[b] == lcaIteration) {
                    lca = b;
                    break;
                }
                lastVisit[b] = lcaIteration;
                b = parent[b];
            }
        }
        for (int v: path_a) {
            twoECC[v] = lca;
            if (v == lca) break;
            --bridgeCount;
        }
        for (int v: path_b) {
            twoECC[v] = lca;
            if (v == lca) break;
            --bridgeCount;
        }
    }

public:
    OnlineBridges(int n) {
        parent.assign(n + 1, -1);
        twoECC.resize(n + 1);
        dsu_cc.resize(n + 1);
        size.assign(n + 1, 1);
        lastVisit.assign(n + 1, 0);
        for (int i = 1; i <= n; ++i) {
            twoECC[i] = i;
            dsu_cc[i] = i;
        }
    }

    void addEdge(int a, int b) {
        a = find2ECC(a);
        b = find2ECC(b);
        if (a == b) return;
        int ca = findCC(a);
        int cb = findCC(b);
        if (ca != cb) {
            ++bridgeCount;
            if (size[ca] > size[cb]) {
                swap(a, b);
                swap(ca, cb);
            }
            makeRoot(a);
            parent[a] = dsu_cc[a] = b;
            size[cb] += size[a];
        } else {
            mergePath(a, b);
        }
    }

    int getBridgeCount() { return bridgeCount; }

    bool sameConnectedComponent(int u, int v) { return findCC(u) == findCC(v); }

    bool inSimpleCycle(int u, int v) { return find2ECC(u) == find2ECC(v); }
};

void solve() {
    int n, q;
    cin >> n >> q;
    OnlineBridges adj(n + 1);
    while (q--) {
        int t, u, v;
        cin >> t >> u >> v;
        if (t == 1) {
            adj.addEdge(u, v);
        } else {
            cout << (adj.inSimpleCycle(u, v) ? "YES\n" : "NO\n");
        }
    }
}

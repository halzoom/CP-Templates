#include <bits/stdc++.h>

using namespace std;
#define int long long
const int N = 1e12 + 5, mod = 1e9 + 7;

struct Node {
    int lc = -1, rc = -1;
    int sum = 0, lazy = 0;
    bool isLazy = false;

    void apply(int x, int lx, int rx) {
        sum += x * ((rx - lx) % mod);
        sum %= mod;
        lazy += x;
        lazy %= mod;
        isLazy = true;
    }
};

struct DynamicSagara {
    vector<Node> tree;
    const int n;

    DynamicSagara(int n, int max_nodes = 1e7) : n(n) {
        tree.reserve(max_nodes);
        tree.push_back(Node());
    }

    int create_node() {
        tree.push_back(Node());
        return tree.size() - 1;
    }

    Node merge(const Node &a, const Node &b) {
        Node res;
        res.sum = (a.sum + b.sum) % mod;
        return res;
    }

    void propagate(int u, int lx, int rx) {
        int m = lx + (rx - lx) / 2;

        if (tree[u].lc == -1) tree[u].lc = create_node();
        if (tree[u].rc == -1) tree[u].rc = create_node();

        if (!tree[u].isLazy) return;

        tree[tree[u].lc].apply(tree[u].lazy, lx, m);
        tree[tree[u].rc].apply(tree[u].lazy, m, rx);

        tree[u].lazy = 0;
        tree[u].isLazy = false;
    }

    void update(int l, int r, int val, int u, int lx, int rx) {
        if (lx >= r || rx <= l) return;
        if (lx >= l && rx <= r) {
            tree[u].apply(val, lx, rx);
            return;
        }

        propagate(u, lx, rx);
        int m = lx + (rx - lx) / 2;

        update(l, r, val, tree[u].lc, lx, m);
        update(l, r, val, tree[u].rc, m, rx);

        tree[u].sum = merge(tree[tree[u].lc], tree[tree[u].rc]).sum;
    }

    Node query(int l, int r, int u, int lx, int rx) {
        if (lx >= r || rx <= l || u == -1) return Node();

        if (lx >= l && rx <= r) return tree[u];

        propagate(u, lx, rx);
        int m = lx + (rx - lx) / 2;

        Node L = query(l, r, tree[u].lc, lx, m);
        Node R = query(l, r, tree[u].rc, m, rx);

        return merge(L, R);
    }

    void update(int l, int r, int val) {
        update(l, r, val, 0, 0, n);
    }

    int query(int l, int r) {
        return query(l, r, 0, 0, n).sum;
    }
};

void solve() {
    int n, q;
    cin >> n >> q;
    DynamicSagara seg(N);
    auto update = [&](int l, int r, int k, int x) {
        int lf = max(l, n / (k + 1) + 1);
        int rt = min(r, n / k);
        l = max(lf, l);
        r = min(rt, r);
        if (l > r)return;
        seg.update(l, r + 1, x);
    };
    while (q--) {
        int t, l, r, k, x;
        cin >> t >> l >> r;
        if (t == 1) {
            cout << seg.query(l, r + 1) << '\n';
        } else {
            cin >> k >> x;
            update(l, r, k, x);
        }
    }
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
#ifdef HALZOOM
    freopen("Input.txt", "r", stdin);
    freopen("Output.txt", "w", stdout);
#endif

    int test = 1;
//    cin >> test;

    for (int i = 1; i <= test; ++i) {
        solve();
    }
    return 0;
}
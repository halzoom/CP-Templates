#include <bits/stdc++.h>

using namespace std;
#define int long long

struct Node {
    int left = 0, right = 0, val = 0;

    Node() {}

    Node(int v) : val(v) {}
};

struct PST {
    vector<Node> nodes;
    int tree_size;

    PST(int n) {
        tree_size = n;
        nodes.reserve(1e7);
        nodes.emplace_back();
    }

    void merge(Node &ans, const Node &lf, const Node &ri) {
        ans.val = lf.val + ri.val;
    }

    int create_node() {
        nodes.push_back({});
        return nodes.size() - 1;
    }

    int update(int idx, int val, int ni, int lx, int rx) {
        int id = create_node();
        if (lx == rx) {
            nodes[id] = Node(val + nodes[ni].val);
            return id;
        }

        int mid = (lx + rx) >> 1;
        nodes[id].left = nodes[ni].left;
        nodes[id].right = nodes[ni].right;

        if (idx <= mid)
            nodes[id].left = update(idx, val, nodes[ni].left, lx, mid);
        else
            nodes[id].right = update(idx, val, nodes[ni].right, mid + 1, rx);

        merge(nodes[id], nodes[nodes[id].left], nodes[nodes[id].right]);
        return id;
    }

    int update(int idx, int val, int version) {
        return update(idx, val, version, 0, tree_size - 1);
    }

    Node get(int l, int r, int ni, int lx, int rx) {
        if (lx > r || rx < l)
            return Node();
        if (lx >= l && rx <= r)
            return nodes[ni];

        int mid = (lx + rx) >> 1;

        Node ans = Node();
        merge(ans, get(l, r, nodes[ni].left, lx, mid), get(l, r, nodes[ni].right, mid + 1, rx));
        return ans;
    }

    int get(int l, int r, int version) {
        return get(l, r, version, 0, tree_size - 1).val;
    }

    int lf(int ni) { return nodes[ni].left; }

    int rt(int ni) { return nodes[ni].right; }

    int val(int ni) { return nodes[ni].val; }

    int getKth(int l, int r, int k, int lx, int rx) {
        if (lx == rx) return lx;

        int rem = val(lf(r)) - val(lf(l));
        int mid = (lx + rx) >> 1;

        if (rem >= k) {
            return getKth(lf(l), lf(r), k, lx, mid);
        } else {
            return getKth(rt(l), rt(r), k - rem, mid + 1, rx);
        }
    }

    int getKth(int u, int v, int k) {
        return getKth(u, v, k, 0, tree_size - 1);
    }
};

void solve() {
    int n, q;
    cin >> n >> q;
    vector<int> a(n + 1);
    vector<int> vals;
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
        vals.push_back(a[i]);
    }

    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());
    auto id = [&](int x) {
        return lower_bound(vals.begin(), vals.end(), x) - vals.begin();
    };

    PST pst(vals.size());
    vector<int> root(n + 1, 0);

    for (int i = 1; i <= n; ++i) {
        int v = id(a[i]);
        root[i] = pst.update(v, 1, root[i - 1]);
    }

    while (q--) {
        int l, r, k;
        cin >> l >> r >> k;
        int idx = pst.getKth(root[l - 1], root[r], k);
        cout << vals[idx] << '\n';
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
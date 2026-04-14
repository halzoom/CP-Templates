#include <bits/stdc++.h>

using namespace std;
#define int long long

struct Node {
    int sum = 0, isLazy = 0, base = 0, step = 0;

    Node() {};

    Node(int x) : sum(x) {};

    void add(int b, int s, int n) {
        sum += b * n + s * (n * (n - 1) / 2);
        base += b;
        step += s;
        isLazy = 1;
    }
};

struct LazySegmentTree {
#define LNodeIDX (2 * node + 1)
#define RNodeIDX (2 * node + 2)
#define mid ((l + r) / 2)
private:
    vector<Node> Tree;
    int SegSize = 1;

    Node merge(Node &a, Node &b) {
        Node res;
        res.sum = a.sum + b.sum;
        return res;
    }

    void build(int l, int r, int node, vector<int> &a) {
        if (l == r) {
            if (l < a.size()) Tree[node] = Node(a[l]);
            return;
        }
        build(l, mid, LNodeIDX, a);
        build(mid + 1, r, RNodeIDX, a);
        Tree[node] = merge(Tree[LNodeIDX], Tree[RNodeIDX]);
    }

    void propagate(int node, int l, int r) {
        if (l == r or !Tree[node].isLazy) return;

        Tree[LNodeIDX].add(Tree[node].base, Tree[node].step, mid - l + 1);
        int nxt = Tree[node].base + Tree[node].step * (mid - l + 1);
        Tree[RNodeIDX].add(nxt, Tree[node].step, r - mid);
        Tree[node].isLazy = Tree[node].base = Tree[node].step = 0;
    }


    void update(int l, int r, int node, int Lx, int Rx, int b, int s) { // b + i * s
        propagate(node, l, r);
        if (Lx > r or Rx < l) return;
        if (Lx <= l and r <= Rx)
            return Tree[node].add(b + s * (l - Lx), s, r - l + 1);

        update(l, mid, LNodeIDX, Lx, Rx, b, s);
        update(mid + 1, r, RNodeIDX, Lx, Rx, b, s);
        Tree[node] = merge(Tree[LNodeIDX], Tree[RNodeIDX]);
    }

    Node query(int l, int r, int node, int Lx, int Rx) {
        propagate(node, l, r);
        if (Lx > r or Rx < l) return {};
        if (Lx <= l and r <= Rx) return Tree[node];
        Node L = query(l, mid, LNodeIDX, Lx, Rx);
        Node R = query(mid + 1, r, RNodeIDX, Lx, Rx);
        return merge(L, R);
    }

public:
    LazySegmentTree(int n) {
        while (SegSize < n) SegSize *= 2;
        Tree.assign(2 * SegSize, {});
    }

    void build(vector<int> &a) { build(0, SegSize - 1, 0, a); }

    void update(int l, int r, int base, int step) {
        update(0, SegSize - 1, 0, l, r, base, step);
    }

    Node query(int l, int r) { return query(0, SegSize - 1, 0, l, r); }
};

void solve() {
    int n, q;
    cin >> n >> q;
    vector<int> a(n + 1);
    for (int i = 1; i <= n; ++i)
        cin >> a[i];

    LazySegmentTree seg(n + 1);
    seg.build(a);
    while (q--) {
        int t, l, r;
        cin >> t >> l >> r;
        if (t == 1)
            seg.update(l, r, 1, 1);
        else
            cout << seg.query(l, r).sum << '\n';
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
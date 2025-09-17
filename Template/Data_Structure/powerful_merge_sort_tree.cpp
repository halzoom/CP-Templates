#include<bits/stdc++.h>
using namespace std;
#define int long long

struct Node {
    int v;

    Node() {};

    Node(int x) { v = x; };
};

struct SegmentTree {
#define LNodeIDX (2 * node + 1)
#define RNodeIDX (2 * node + 2)
#define mid ((l + r) / 2)
    vector <Node> Tree;
    int SegSize = 1, size = 0;
    Node defVal = 0;

    Node Merge(Node &a, Node &b) {
        Node res;
        res.v = a.v + b.v;
        return res;
    }

    void update(int l, int r, int node, int idx, int val) {
        if (l == r) {
            Tree[node].v = val;
            return;
        }
        if (idx <= mid) {
            update(l, mid, LNodeIDX, idx, val);
        } else {
            update(mid + 1, r, RNodeIDX, idx, val);
        }
        Tree[node] = Merge(Tree[LNodeIDX], Tree[RNodeIDX]);
    }

    Node query(int l, int r, int node, int Lx, int Rx) {
        if (Lx > r or Rx < l) { return defVal; }
        if (Lx <= l and r <= Rx) { return Tree[node]; }
        Node L = query(l, mid, LNodeIDX, Lx, Rx);
        Node R = query(mid + 1, r, RNodeIDX, Lx, Rx);
        return Merge(L, R);
    }

public:
    SegmentTree(int n) {
        while (SegSize < n) SegSize *= 2;
        size = n;
        Tree.assign(2 * SegSize, defVal);
    }

    void update(int idx, int val) { update(0, SegSize - 1, 0, idx, val); }

    Node query(int l, int r) { return query(0, SegSize - 1, 0, l, r); }
};

struct MergeTree {
#define LNodeIDX (2 * node + 1)
#define RNodeIDX (2 * node + 2)
#define mid ((l + r) / 2)
    vector <SegmentTree> Tree;
    int SegSize = 1;
    Node defVal = 0;

    SegmentTree Merge(SegmentTree &a, SegmentTree &b) {
        SegmentTree res(a.size + b.size);
        int i = 0, j = 0, idx = 0;
        while (i < a.size and j < b.size) {
            int x = a.query(i, i).v, y = b.query(j, j).v;
            if (x <= y) res.update(idx++, x), ++i;
            else res.update(idx++, y), ++j;
        }
        while (i < a.size) res.update(idx++, a.query(i, i).v), ++i;
        while (j < b.size) res.update(idx++, b.query(j, j).v), ++j;
        return res;
    }

    void build(int l, int r, int node, vector<int> &a) {
        if (l == r) {
            if (l < a.size()) {
                Tree[node] = SegmentTree(1);
                Tree[node].update(0, a[l]);
            }
            return;
        }
        build(l, mid, LNodeIDX, a);
        build(mid + 1, r, RNodeIDX, a);
        Tree[node] = SegmentTree(Tree[LNodeIDX].size + Tree[RNodeIDX].size);
        Tree[node] = Merge(Tree[LNodeIDX], Tree[RNodeIDX]);
    }

    int getSum(int node, int k) {
        int l = 0, r = Tree[node].size - 1, md, ans = -1;
        while (l <= r) {
            md = (l + r) / 2;
            if (Tree[node].query(md, md).v <= k) {
                ans = md;
                l = md + 1;
            } else r = md - 1;
        }
        int res = 0;
        if (ans != -1) { res = Tree[node].query(0, ans).v; }
        return res;
    }

    int query(int l, int r, int node, int Lx, int Rx, int k) {
        if (Lx > r or Rx < l) return 0;
        if (Lx <= l and r <= Rx) {
            int x = getSum(node, k);
            return x;
        }
        int L = query(l, mid, LNodeIDX, Lx, Rx, k);
        int R = query(mid + 1, r, RNodeIDX, Lx, Rx, k);
        return L + R;
    }

public:
    MergeTree(vector<int> &a) {
        int n = (int) a.size();
        while (SegSize < n) SegSize *= 2;
        Tree.assign(2 * SegSize, SegmentTree(0));
        build(0, SegSize - 1, 0, a);
    }

    int query(int l, int r, int k) { return query(0, SegSize - 1, 0, l, r, k); }
};

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) { cin >> a[i]; }
    MergeTree tree(a);
    int q, B = 0;
    cin >> q;
    while (q--) {
        int l, r, k;
        cin >> l >> r >> k;
        l ^= B, r ^= B, k ^= B;
        --l, --r;
        B = tree.query(l, r, k);
        cout << B << '\n';
    }
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("Input.txt", "r", stdin);
    freopen("Output.txt", "w", stdout);
#endif
    int test = 1;
    //    cin >> test;
    for (int i = 1; i <= test; ++i) { solve(); }
    return 0;
}
#include <bits/stdc++.h>

using namespace std;
#define int long long
const int inf = 1e18;

struct Node {
    int sum = 0, min_val = inf;

    Node() {};

    Node(int v) {
        sum = min_val = v;
    };
};

struct SegmentTree {
#define Lidx (2 * node + 1)
#define Ridx (2 * node + 2)
#define mid ((l + r) / 2)
private:
    vector<Node> Seg;
    int SegSize = 1;
    Node difVal;

    Node Merge(Node a, Node b) {
        Node res{};
        res.sum = a.sum + b.sum;
        res.min_val = min(a.min_val, b.min_val);
        return res;
    }

    void update(int l, int r, int node, int idx, int v) {
        if (l == r) {
            Seg[node] = v;
            return;
        }
        if (mid >= idx) update(l, mid, Lidx, idx, v);
        else update(mid + 1, r, Ridx, idx, v);
        Seg[node] = Merge(Seg[Lidx], Seg[Ridx]);
    }

    Node query(int l, int r, int node, int Lx, int Rx) {
        if (l > Rx or r < Lx) return difVal;
        if (l >= Lx and r <= Rx) return Seg[node];
        Node Left = query(l, mid, Lidx, Lx, Rx);
        Node Right = query(mid + 1, r, Ridx, Lx, Rx);
        return Merge(Left, Right);
    }

    Node nxtSmaller(int l, int r, int node, int Lx, int Rx, int v) {
        if (l > Rx or r < Lx or Seg[node].min_val > v) return difVal;
        if (l == r)
            return Seg[node];
        Node left = nxtSmaller(l, mid, Lidx, Lx, Rx, v);
        if (left.min_val != inf)return left;
        return nxtSmaller(mid + 1, r, Ridx, Lx, Rx, v);
    }

public:
    SegmentTree(int n) {
        SegSize = 1;
        while (SegSize < n) SegSize *= 2;
        Seg.assign(2 * SegSize, difVal);
    }

    void update(int idx, int v) { update(0, SegSize - 1, 0, idx, v); }

    Node query(int l, int r) { return query(0, SegSize - 1, 0, l, r); }

    Node nxtSmaller(int l, int r, int k) { return nxtSmaller(0, SegSize - 1, 0, l, r, k); }

#undef Lidx
#undef Ridx
#undef mid
};

void solve() {
    int n, q;
    cin >> n >> q;
    SegmentTree seg(n + 1);
    for (int i = 1, x; i <= n; ++i)
        cin >> x, seg.update(i, x);

    while (q--) {
        int k, l, r;
        cin >> k >> l >> r;
        while (true) {
            auto node = seg.nxtSmaller(l, r, k);
            if (node.min_val == inf)break;
            k = k % node.min_val;
        }
        cout << k << '\n';
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
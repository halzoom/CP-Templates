#include <bits/stdc++.h>

using namespace std;
#define int long long

struct Node {
    Node *l, *r;
    int val = 0;
    Node(int val) : l(nullptr), r(nullptr), val(val) {}
    Node() : l(nullptr), r(nullptr) {}
    Node(Node *l, Node *r) : l(l), r(r) {
        if (l != nullptr) val += l->val;
        if (r != nullptr) val += r->val;
    }

    void addChild() {
        l = new Node();
        r = new Node();
    }
};

struct PST {
    int n;

    PST(int n) : n(n + 1) {}

    Node merge(Node x, Node y) {
        Node ret{};
        ret.val = x.val + y.val;
        return ret;
    }

    Node *update(Node *v, int i, int val, int lx, int rx) {
        if (lx == rx) return new Node(val);
        int mid = (lx + rx) / 2;
        if (!v->l) v->addChild();
        if (i <= mid) return new Node(update(v->l, i, val, lx, mid), v->r);
        return new Node(v->l, update(v->r, i, val, mid + 1, rx));
    }

    Node query(Node *v, int l, int r, int lx, int rx) {
        if (l > rx or r < lx) return {};
        if (l <= lx and r >= rx) return *v;
        if (!v->l) v->addChild();
        int mid = (lx + rx) / 2;
        return merge(query(v->l, l, r, lx, mid), query(v->r, l, r, mid + 1, rx));
    }

    int getKth(Node *a, Node *b, int k, int lx, int rx) {
        if (lx == rx) return lx;
        if (!a->l) a->addChild();
        if (!b->l) b->addChild();
        int rem = b->l->val - a->l->val;
        int mid = (lx + rx) / 2;
        if (rem >= k) return getKth(a->l, b->l, k, lx, mid);
        return getKth(a->r, b->r, k - rem, mid + 1, rx);
    }

    Node *update(Node *v, int i, int val) { return update(v, i, val, 0, n - 1); }
    Node query(Node *v, int l, int r) { return query(v, l, r, 0, n - 1); }
    int getKth(Node *a, Node *b, int k) { return getKth(a, b, k, 0, n - 1); }
};

void solve() {
    int n, q;
    cin >> n >> q;
    vector<Node *> roots(n + 1);
    roots[0] = new Node();
    int shift = 2e9;
    PST pst(5e9 + 5);
    for (int i = 1, x; i <= n; ++i) {
        cin >> x;
        roots[i] = pst.update(roots[i - 1], shift + x, 1);
    }
    while (q--) {
        int l, r, k;
        cin >> l >> r >> k;
        cout << pst.getKth(roots[l - 1], roots[r], k) - shift << '\n';
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
#include <bits/stdc++.h>

using namespace std;
#define int long long

struct Node {
    Node *l, *r;
    int val = 0, lazy = 0, isLazy = 0;
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
    void update(int v, int lx, int rx) {
        val += v * (rx - lx + 1);
        lazy += v;
        isLazy = 1;
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

    Node *propagate(Node *v, int lx, int rx) {
        if (!v) v = new Node();
        if (!v->isLazy) return v;
        Node *ret = new Node(*v);
        if (lx != rx) {
            if (!ret->l) ret->addChild();
            ret->l = new Node(*ret->l);
            ret->r = new Node(*ret->r);
            int mid = (lx + rx) / 2;
            ret->l->update(ret->lazy, lx, mid);
            ret->r->update(ret->lazy, mid + 1, rx);
        }
        ret->lazy = 0;
        ret->isLazy = 0;
        return ret;
    }

    Node *update(Node *v, int l, int r, int val, int lx, int rx) {
        if (!v) v = new Node();
        if (l > rx or r < lx) return v;
        Node *ret = new Node(*v);
        if (l <= lx and rx <= r) {
            ret->update(val, lx, rx);
            return ret;
        }
        ret = propagate(ret, lx, rx);
        int mid = (lx + rx) / 2;
        if (!ret->l) ret->addChild();
        ret->l = update(ret->l, l, r, val, lx, mid);
        ret->r = update(ret->r, l, r, val, mid + 1, rx);
        ret->val = ret->l->val + ret->r->val;
        return ret;
    }

    Node query(Node *v, int l, int r, int lx, int rx) {
        if (!v or l > rx or r < lx) return {};
        if (l <= lx and r >= rx) return *v;
        v = propagate(v, lx, rx);
        if (!v->l) v->addChild();
        int mid = (lx + rx) / 2;
        return merge(query(v->l, l, r, lx, mid), query(v->r, l, r, mid + 1, rx));
    }

    int getKth(Node *a, Node *b, int k, int lx, int rx) {
        if (lx == rx) return lx;
        a = propagate(a, lx, rx);
        b = propagate(b, lx, rx);
        if (!a->l) a->addChild();
        if (!b->l) b->addChild();
        int rem = b->l->val - a->l->val;
        int mid = (lx + rx) / 2;
        if (rem >= k) return getKth(a->l, b->l, k, lx, mid);
        return getKth(a->r, b->r, k - rem, mid + 1, rx);
    }

    Node *update(Node *v, int l, int r, int val) { return update(v, l, r, val, 0, n - 1); }
    Node *update(Node *v, int i, int val) { return update(v, i, i, val, 0, n - 1); }
    Node query(Node *v, int l, int r) { return query(v, l, r, 0, n - 1); }
    int getKth(Node *a, Node *b, int k) { return getKth(a, b, k, 0, n - 1); }
};

void solve() {
    int n, q;
    cin >> n >> q;
    vector<Node *> roots(q + 1);
    roots[0] = new Node();
    PST pst(n + 1);

    for (int i = 1, x; i <= n; ++i) {
        cin >> x;
        roots[0] = pst.update(roots[0], i, x);
    }

    int cur = 0;
    while (q--) {
        char c;
        int l, r, d;
        cin >> c;
        if (c == 'C') {
            cin >> l >> r >> d;
            roots[cur + 1] = pst.update(roots[cur], l, r, d);
            ++cur;
        } else if (c == 'Q') {
            cin >> l >> r;
            cout << pst.query(roots[cur], l, r).val << '\n';
        } else if (c == 'H') {
            cin >> l >> r >> d;
            cout << pst.query(roots[d], l, r).val << '\n';
        } else {
            cin >> d;
            cur = d;
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
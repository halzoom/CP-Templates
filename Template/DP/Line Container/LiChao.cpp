#include <bits/stdc++.h>

using namespace std;
#define int long long
const int inf = 2e18;
int LF = 0, RT = 5e5;

struct Line {
    int m, b;

    Line(int m = 0, int b = inf) : m(m), b(b) {}

    int get(int x) {
        return (int) ((__int128) m * x + b);
    }
};

vector<int> coord;

struct Node {
    Line ln;
    Node *l = nullptr, *r = nullptr;

    Node() : ln() {}
};

void add_line(Node *&t, int l, int r, Line nw) {
    if (!t) t = new Node();

    int mid = l + (r - l) / 2;

    bool left = nw.get(coord[l]) < t->ln.get(coord[l]);
    bool m = nw.get(coord[mid]) < t->ln.get(coord[mid]);

    if (m) swap(nw, t->ln);

    if (l == r) return;

    if (left != m) add_line(t->l, l, mid, nw);
    else add_line(t->r, mid + 1, r, nw);
}

void add_segment(Line nw, int ql, int qr, Node *&t, int l = LF, int r = RT) {
    if (qr < l || r < ql) return;
    if (ql <= l && r <= qr) {
        add_line(t, l, r, nw);
        return;
    }
    if (!t) t = new Node();

    int mid = l + (r - l) / 2;
    add_segment(nw, ql, qr, t->l, l, mid);
    add_segment(nw, ql, qr, t->r, mid + 1, r);
}

int query(int x, Node *t, int l = LF, int r = RT) {
    if (!t) return inf;
    int res = t->ln.get(coord[x]);
    if (l == r) return res;

    int mid = l + (r - l) / 2;
    if (x <= mid) return min(res, query(x, t->l, l, mid));
    return min(res, query(x, t->r, mid + 1, r));
}

void solve() {
    int n, q;
    cin >> n >> q;
    Node *T = nullptr;
    vector<array<int, 4>> a(n);
    vector<array<int, 5>> Q(q);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < 4; ++j)
            cin >> a[i][j];

    coord.clear();
    for (int i = 0; i < q; ++i) {
        cin >> Q[i][0];
        if (Q[i][0] == 0) {
            for (int j = 1; j < 5; ++j)
                cin >> Q[i][j];
        } else {
            cin >> Q[i][1];
            coord.emplace_back(Q[i][1]);
        }
    }

    sort(coord.begin(), coord.end());
    coord.erase(unique(coord.begin(), coord.end()), coord.end());

    auto add = [&](int l, int r, int c, int p) {
        int L = lower_bound(coord.begin(), coord.end(), l) - coord.begin();
        int R = lower_bound(coord.begin(), coord.end(), r) - coord.begin() - 1;
        if (L <= R)
            add_segment({c, p}, L, R, T);
    };

    for (auto [l, r, c, p]: a)
        add(l, r, c, p);

    for (auto [t, l, r, c, p]: Q) {
        if (t == 0) {
            add(l, r, c, p);
        } else {
            l = lower_bound(coord.begin(), coord.end(), l) - coord.begin();
            int cur = query(l, T);
            if (cur >= inf / 2)
                cout << "INFINITY\n";
            else
                cout << cur << '\n';
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
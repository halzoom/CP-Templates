#include <bits/stdc++.h>

using namespace std;
#define int long long
const int M = 2, N = 2e5 + 5;
int B[M], mods[M], pw[M][N];

void build() {
    if (B[0])return;

    mt19937 rng(chrono::system_clock::now().time_since_epoch().count());
    auto rnd = [&](int a, int b) {
        return a + rng() % (b - a + 1);
    };
    auto check = [&](int x) {
        for (int i = 2; i * i <= x; ++i)if (x % i == 0)return true;
        return false;
    };

    for (int i = 0; i < M; ++i) {
        B[i] = rnd(230, 500);
        mods[i] = rnd(1e9, 2e9);
        while (check(mods[i]))mods[i]--;

        pw[i][0] = 1;
        for (int j = 1; j < N; ++j)
            pw[i][j] = pw[i][j - 1] * B[i] % mods[i];
    }
}

struct Node {
    array<int, M> prefix{}, suffix{};
    int len = 0;

    Node() {};

    Node(int a) : prefix({a, a}), suffix({a, a}), len(1) {};
};


struct SegmentTree {
#define LNodeIDX (2 * node + 1)
#define RNodeIDX (2 * node + 2)
#define mid      ((l + r) / 2 )
    vector<Node> Tree;
    int SegSize = 1;
    Node defVal;

    Node Merge(const Node &a, const Node &b) {
        Node res;
        res.len = a.len + b.len;
        for (int i = 0; i < M; ++i) {
            res.prefix[i] = (pw[i][b.len] * a.prefix[i] % mods[i] + b.prefix[i]) % mods[i];
        }
        for (int i = 0; i < M; ++i) {
            res.suffix[i] = (pw[i][a.len] * b.suffix[i] % mods[i] + a.suffix[i]) % mods[i];
        }
        return res;
    }

    void build(int l, int r, int node, string &s) {
        if (l == r) {
            if (l < s.size()) Tree[node] = Node(s[l]);
            return;
        }
        build(l, mid, LNodeIDX, s);
        build(mid + 1, r, RNodeIDX, s);
        Tree[node] = Merge(Tree[LNodeIDX], Tree[RNodeIDX]);
    }

    void update(int l, int r, int node, int idx, int val) {
        if (l == r) {
            Tree[node] = Node(val);
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
        if (Lx > r or Rx < l) {
            return defVal;
        }
        if (Lx <= l and r <= Rx) {
            return Tree[node];
        }

        Node L = query(l, mid, LNodeIDX, Lx, Rx);
        Node R = query(mid + 1, r, RNodeIDX, Lx, Rx);

        return Merge(L, R);
    }

public:
    SegmentTree(int n) {
        while (SegSize < n)SegSize *= 2;
        Tree.assign(2 * SegSize, defVal);
    }

    void build(string &s) {
        build(0, SegSize - 1, 0, s);
    }

    void update(int idx, int val) {
        update(0, SegSize - 1, 0, idx, val);
    }

    bool query(int l, int r) {
        Node ret = query(0, SegSize - 1, 0, l, r);;
        return ret.prefix == ret.suffix;
    }
};

void solve() {
    int n, q;
    string s;
    cin >> n >> q >> s;
    build();
    SegmentTree seg(n);
    seg.build(s);

    for (int i = 0, t; i < q; ++i) {
        cin >> t;
        if (t == 1) {
            int idx;
            char c;
            cin >> idx >> c, --idx;
            seg.update(idx, c);
        } else {
            int l, r;
            cin >> l >> r, --l, --r;
            cout << (seg.query(l, r) ? "Adnan Wins\n" : "ARCNCD!\n");
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
    cin >> test;

    for (int i = 1; i <= test; ++i) {
        solve();
    }
    return 0;
}
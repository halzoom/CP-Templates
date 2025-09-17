#include<bits/stdc++.h>
using namespace std;
#define int long long

const int mod = 1e9 + 7;

struct SegmentTree2D {
    int rows, cols;
    vector <vector<int>> tree;

    SegmentTree2D(int n, int m) : rows(n), cols(m) {
        tree.assign(4 * rows, vector<int>(4 * cols, 0));
    }

    void build(vector <vector<int>> &data) { buildX(1, 0, rows - 1, data); }

    void buildX(int vx, int lx, int rx, const vector <vector<int>> &data) {
        if (lx != rx) {
            int mx = (lx + rx) / 2;
            buildX(vx * 2, lx, mx, data);
            buildX(vx * 2 + 1, mx + 1, rx, data);
        }
        buildY(vx, lx, rx, 1, 0, cols - 1, data);
    }

    void buildY(int vx, int lx, int rx, int vy, int ly, int ry,
                const vector <vector<int>> &data) {
        if (ly == ry) {
            if (lx == rx) tree[vx][vy] = data[lx][ly];
            else tree[vx][vy] = merge(tree[vx * 2][vy], tree[vx * 2 + 1][vy]);
        } else {
            int my = (ly + ry) / 2;
            buildY(vx, lx, rx, vy * 2, ly, my, data);
            buildY(vx, lx, rx, vy * 2 + 1, my + 1, ry, data);
            tree[vx][vy] = merge(tree[vx][vy * 2], tree[vx][vy * 2 + 1]);
        }
    }

    void update(int x, int y, int newValue) { updateX(1, 0, rows - 1, x, y, newValue); }

    int query(int x1, int y1, int x2, int y2) {
        return queryX(1, 0, rows - 1, x1, x2, y1, y2);
    }

private:
    int merge(int a, int b) { return gcd(a, b); }

    void updateY(int vx, int lx, int rx, int vy, int ly, int ry, int x, int y,
                 int newValue) {
        if (ly == ry) {
            if (lx == rx) tree[vx][vy] = newValue;
            else tree[vx][vy] = merge(tree[vx * 2][vy], tree[vx * 2 + 1][vy]);
        } else {
            int my = (ly + ry) / 2;
            if (y <= my) updateY(vx, lx, rx, vy * 2, ly, my, x, y, newValue);
            else updateY(vx, lx, rx, vy * 2 + 1, my + 1, ry, x, y, newValue);
            tree[vx][vy] = merge(tree[vx][vy * 2], tree[vx][vy * 2 + 1]);
        }
    }

    void updateX(int vx, int lx, int rx, int x, int y, int newValue) {
        if (lx != rx) {
            int mx = (lx + rx) / 2;
            if (x <= mx) updateX(vx * 2, lx, mx, x, y, newValue);
            else updateX(vx * 2 + 1, mx + 1, rx, x, y, newValue);
        }
        updateY(vx, lx, rx, 1, 0, cols - 1, x, y, newValue);
    }

    int queryY(int vx, int vy, int tly, int try_, int ly, int ry) {
        if (ly > ry) return 0;
        if (ly == tly && try_ == ry) return tree[vx][vy];
        int tmy = (tly + try_) / 2;
        return merge(queryY(vx, vy * 2, tly, tmy, ly, min(ry, tmy)),
                     queryY(vx, vy * 2 + 1, tmy + 1, try_, max(ly, tmy + 1), ry));
    }

    int queryX(int vx, int tlx, int trx, int lx, int rx, int ly, int ry) {
        if (lx > rx) return 0;
        if (lx == tlx && trx == rx) return queryY(vx, 1, 0, cols - 1, ly, ry);
        int tmx = (tlx + trx) / 2;
        return merge(queryX(vx * 2, tlx, tmx, lx, min(rx, tmx), ly, ry),
                     queryX(vx * 2 + 1, tmx + 1, trx, max(lx, tmx + 1), rx, ly, ry));
    }
};

void solve() {
    int n, m;
    cin >> n >> m;
    vector <vector<int>> grid(n, vector<int>(m));
    for (int i = 0; i < n; ++i)
        for (int j = 0, x; j < m; ++j) cin >> grid[i][j];
    SegmentTree2D seg(n, m);
    seg.build(grid);
    int answer = 1;
    int q;
    cin >> q;
    while (q--) {
        int x, y, k;
        cin >> x >> y >> k;
        --x, --y;
        int g = seg.query(x, y, x + k - 1, y + k - 1);
        answer = 1LL * answer * g % mod;
    }
    cout << answer << '\n';
}
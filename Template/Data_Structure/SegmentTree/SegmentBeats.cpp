#include <bits/stdc++.h>

using namespace std;
#define int long long
const int INF = (1LL << 62);

// All ranges are 0-indexed and half-open [l,r).
struct Node {
    int len = 0, sum = 0;
    int mn = INF, mn2 = INF, mnCnt = 0;
    int mx = -INF, mx2 = -INF, mxCnt = 0;
    int midGcd = 0;
    int lazyAdd = 0, lazySet = 0;
    bool hasSet = false;

    Node() = default;

    Node(int x) : len(1), sum(x), mn(x), mnCnt(1), mx(x), mxCnt(1) {}
};

struct SegTree {
    int n = 0, tree_size = 1;
    vector<Node> seg;

    SegTree() = default;

    SegTree(int n, int x = 0) { init(n, x); }

    SegTree(const vector<int> &a) { build(a); }

    int G(int a, int b) { return std::gcd(a, b); }

    Node merge(const Node &a, const Node &b) {
        if (!a.len) {
            Node c = b;
            c.lazyAdd = c.lazySet = 0;
            c.hasSet = false;
            return c;
        }
        if (!b.len) {
            Node c = a;
            c.lazyAdd = c.lazySet = 0;
            c.hasSet = false;
            return c;
        }

        Node c;
        c.len = a.len + b.len;
        c.sum = a.sum + b.sum;

        c.mx = max(a.mx, b.mx);
        c.mxCnt = (a.mx == c.mx ? a.mxCnt : 0) + (b.mx == c.mx ? b.mxCnt : 0);
        c.mx2 = max(a.mx == c.mx ? a.mx2 : a.mx, b.mx == c.mx ? b.mx2 : b.mx);

        c.mn = min(a.mn, b.mn);
        c.mnCnt = (a.mn == c.mn ? a.mnCnt : 0) + (b.mn == c.mn ? b.mnCnt : 0);
        c.mn2 = min(a.mn == c.mn ? a.mn2 : a.mn, b.mn == c.mn ? b.mn2 : b.mn);

        bool hasRep = false;
        int rep = 0;

        auto addVal = [&](int x) {
            if (x <= c.mn || x >= c.mx)return;
            if (!hasRep)rep = x, hasRep = true;
            else c.midGcd = G(c.midGcd, x - rep);
        };

        auto addChild = [&](const Node &x) {
            if (x.mn2 < x.mx) {
                int r = x.mn2;
                if (!hasRep)rep = r, hasRep = true;
                else c.midGcd = G(c.midGcd, r - rep);
                c.midGcd = G(c.midGcd, x.midGcd);
            }
            addVal(x.mn);
            if (x.mx != x.mn)addVal(x.mx);
        };

        addChild(a);
        addChild(b);
        return c;
    }

    void pull(int ni) {
        seg[ni] = merge(seg[2 * ni + 1], seg[2 * ni + 2]);
    }

    // Initializes n equal values in O(n).
    void init(int _n, int x = 0) {
        n = _n;
        tree_size = 1;
        while (tree_size < max<int>(1, n))tree_size <<= 1;
        seg.assign(2 * tree_size - 1, Node());
        for (int i = 0; i < n; ++i)seg[tree_size - 1 + i] = Node(x);
        for (int i = tree_size - 2; i >= 0; --i)pull(i);
    }

    // Builds from an array in O(n).
    void build(const vector<int> &a) {
        n = a.size();
        tree_size = 1;
        while (tree_size < max<int>(1, n))tree_size <<= 1;
        seg.assign(2 * tree_size - 1, Node());
        for (int i = 0; i < n; ++i)seg[tree_size - 1 + i] = Node(a[i]);
        for (int i = tree_size - 2; i >= 0; --i)pull(i);
    }

    void applySet(int ni, int x) {
        auto &t = seg[ni];
        if (!t.len)return;

        t.sum = x * t.len;

        t.mn = t.mx = x;
        t.mn2 = INF;
        t.mx2 = -INF;
        t.mnCnt = t.mxCnt = t.len;

        t.midGcd = 0;

        t.lazySet = x;
        t.hasSet = true;
        t.lazyAdd = 0;
    }

    void applyAdd(int ni, int x) {
        auto &t = seg[ni];
        if (!t.len || !x)return;

        t.sum += x * t.len;

        t.mn += x;
        t.mx += x;

        if (t.mn2 != INF)t.mn2 += x;
        if (t.mx2 != -INF)t.mx2 += x;

        if (t.hasSet)t.lazySet += x;
        else t.lazyAdd += x;
    }

    void applyChmin(int ni, int x) {
        auto &t = seg[ni];
        if (!t.len || t.mx <= x)return;

        if (x <= t.mn) {
            applySet(ni, x);
            return;
        }

        assert(t.mx2 < x);

        int old = t.mx;
        t.sum -= (old - x) * t.mxCnt;

        if (t.mn2 == old)t.mn2 = x;

        t.mx = x;
    }

    void applyChmax(int ni, int x) {
        auto &t = seg[ni];
        if (!t.len || t.mn >= x)return;

        if (x >= t.mx) {
            applySet(ni, x);
            return;
        }

        assert(t.mn2 > x);

        int old = t.mn;
        t.sum += (x - old) * t.mnCnt;

        if (t.mx2 == old)t.mx2 = x;

        t.mn = x;
    }

    void push(int ni) {
        int l = 2 * ni + 1, r = l + 1;
        if (l >= (int) seg.size())return;

        if (seg[ni].hasSet) {
            applySet(l, seg[ni].lazySet);
            applySet(r, seg[ni].lazySet);
            seg[ni].hasSet = false;
        }

        if (seg[ni].lazyAdd) {
            int x = seg[ni].lazyAdd;
            applyAdd(l, x);
            applyAdd(r, x);
            seg[ni].lazyAdd = 0;
        }

        if (seg[l].mx > seg[ni].mx)applyChmin(l, seg[ni].mx);
        if (seg[r].mx > seg[ni].mx)applyChmin(r, seg[ni].mx);

        if (seg[l].mn < seg[ni].mn)applyChmax(l, seg[ni].mn);
        if (seg[r].mn < seg[ni].mn)applyChmax(r, seg[ni].mn);
    }

    void rangeChmin(int l, int r, int x, int ni, int lx, int rx) {
        if (rx <= l || r <= lx || seg[ni].mx <= x)return;

        if (l <= lx && rx <= r && seg[ni].mx2 < x) {
            applyChmin(ni, x);
            return;
        }

        push(ni);

        int mid = (lx + rx) / 2;

        rangeChmin(l, r, x, 2 * ni + 1, lx, mid);
        rangeChmin(l, r, x, 2 * ni + 2, mid, rx);

        pull(ni);
    }

    void rangeChmax(int l, int r, int x, int ni, int lx, int rx) {
        if (rx <= l || r <= lx || seg[ni].mn >= x)return;

        if (l <= lx && rx <= r && seg[ni].mn2 > x) {
            applyChmax(ni, x);
            return;
        }

        push(ni);

        int mid = (lx + rx) / 2;

        rangeChmax(l, r, x, 2 * ni + 1, lx, mid);
        rangeChmax(l, r, x, 2 * ni + 2, mid, rx);

        pull(ni);
    }

    void rangeAdd(int l, int r, int x, int ni, int lx, int rx) {
        if (rx <= l || r <= lx)return;

        if (l <= lx && rx <= r) {
            applyAdd(ni, x);
            return;
        }

        push(ni);

        int mid = (lx + rx) / 2;

        rangeAdd(l, r, x, 2 * ni + 1, lx, mid);
        rangeAdd(l, r, x, 2 * ni + 2, mid, rx);

        pull(ni);
    }

    void rangeAssign(int l, int r, int x, int ni, int lx, int rx) {
        if (rx <= l || r <= lx)return;

        if (l <= lx && rx <= r) {
            applySet(ni, x);
            return;
        }

        push(ni);

        int mid = (lx + rx) / 2;

        rangeAssign(l, r, x, 2 * ni + 1, lx, mid);
        rangeAssign(l, r, x, 2 * ni + 2, mid, rx);

        pull(ni);
    }

    Node query(int l, int r, int ni, int lx, int rx) {
        if (rx <= l || r <= lx)return Node();

        if (l <= lx && rx <= r)
            return seg[ni];

        push(ni);

        int mid = (lx + rx) / 2;

        return merge(
                query(l, r, 2 * ni + 1, lx, mid),
                query(l, r, 2 * ni + 2, mid, rx)
        );
    }

    int firstGE(int l, int r, int x, int ni, int lx, int rx) {
        if (rx <= l || r <= lx || seg[ni].mx < x)return -1;
        if (rx - lx == 1)return lx < n ? lx : -1;

        push(ni);

        int mid = (lx + rx) / 2;

        int ans = firstGE(l, r, x, 2 * ni + 1, lx, mid);

        if (ans != -1)return ans;

        return firstGE(l, r, x, 2 * ni + 2, mid, rx);
    }

    int firstLE(int l, int r, int x, int ni, int lx, int rx) {
        if (rx <= l || r <= lx || seg[ni].mn > x)return -1;
        if (rx - lx == 1)return lx < n ? lx : -1;

        push(ni);

        int mid = (lx + rx) / 2;

        int ans = firstLE(l, r, x, 2 * ni + 1, lx, mid);

        if (ans != -1)return ans;

        return firstLE(l, r, x, 2 * ni + 2, mid, rx);
    }

    int lastGE(int l, int r, int x, int ni, int lx, int rx) {
        if (rx <= l || r <= lx || seg[ni].mx < x)return -1;
        if (rx - lx == 1)return lx < n ? lx : -1;

        push(ni);

        int mid = (lx + rx) / 2;

        int ans = lastGE(l, r, x, 2 * ni + 2, mid, rx);

        if (ans != -1)return ans;

        return lastGE(l, r, x, 2 * ni + 1, lx, mid);
    }

    int lastLE(int l, int r, int x, int ni, int lx, int rx) {
        if (rx <= l || r <= lx || seg[ni].mn > x)return -1;
        if (rx - lx == 1)return lx < n ? lx : -1;

        push(ni);

        int mid = (lx + rx) / 2;

        int ans = lastLE(l, r, x, 2 * ni + 2, mid, rx);

        if (ans != -1)return ans;

        return lastLE(l, r, x, 2 * ni + 1, lx, mid);
    }

    int nodeGcd(const Node &t) {
        if (!t.len)return 0;

        int g = G(t.mn, t.mx);

        if (t.mn2 < t.mx) {
            g = G(g, t.mn2);
            g = G(g, t.midGcd);
        }

        return g;
    }

    // Returns the number of elements in O(1).
    int size() { return n; }

    // Checks whether the tree is empty in O(1).
    bool empty() { return !n; }

    // Applies a[i] = min(a[i],x) on [l,r) in O(log^2 n) amortized.
    void rangeChmin(int l, int r, int x) {
        rangeChmin(l, r, x, 0, 0, tree_size);
    }

    // Applies a[i] = max(a[i],x) on [l,r) in O(log^2 n) amortized.
    void rangeChmax(int l, int r, int x) {
        rangeChmax(l, r, x, 0, 0, tree_size);
    }

    // Adds x to every element in [l,r) in O(log n).
    void rangeAdd(int l, int r, int x) {
        rangeAdd(l, r, x, 0, 0, tree_size);
    }

    // Assigns x to every element in [l,r) in O(log n).
    void rangeAssign(int l, int r, int x) {
        rangeAssign(l, r, x, 0, 0, tree_size);
    }

    // Clamps every element in [l,r) to [lo,hi] in O(log^2 n) amortized.
    void rangeClamp(int l, int r, int lo, int hi) {
        assert(lo <= hi);
        rangeChmax(l, r, lo);
        rangeChmin(l, r, hi);
    }

    // Returns all maintained information on [l,r) in O(log n).
    Node query(int l, int r) {
        return query(l, r, 0, 0, tree_size);
    }

    // Returns the sum on [l,r) in O(log n).
    int rangeSum(int l, int r) {
        return query(l, r).sum;
    }

    // Returns the minimum on [l,r) in O(log n).
    int rangeMin(int l, int r) {
        return query(l, r).mn;
    }

    // Returns the maximum on [l,r) in O(log n).
    int rangeMax(int l, int r) {
        return query(l, r).mx;
    }

    // Returns the gcd of all values on [l,r) in O(log n).
    int rangeGcd(int l, int r) {
        return nodeGcd(query(l, r));
    }

    // Returns how many times the minimum appears on [l,r) in O(log n).
    int rangeCountMin(int l, int r) {
        return query(l, r).mnCnt;
    }

    // Returns how many times the maximum appears on [l,r) in O(log n).
    int rangeCountMax(int l, int r) {
        return query(l, r).mxCnt;
    }

    // Returns the first index in [l,r) with a[i]>=x, or -1, in O(log n).
    int firstAtLeast(int l, int r, int x) {
        return firstGE(l, r, x, 0, 0, tree_size);
    }

    // Returns the first index in [l,r) with a[i]<=x, or -1, in O(log n).
    int firstAtMost(int l, int r, int x) {
        return firstLE(l, r, x, 0, 0, tree_size);
    }

    // Returns the last index in [l,r) with a[i]>=x, or -1, in O(log n).
    int lastAtLeast(int l, int r, int x) {
        return lastGE(l, r, x, 0, 0, tree_size);
    }

    // Returns the last index in [l,r) with a[i]<=x, or -1, in O(log n).
    int lastAtMost(int l, int r, int x) {
        return lastLE(l, r, x, 0, 0, tree_size);
    }

    // Returns a[i] in O(log n).
    int get(int i) {
        return rangeSum(i, i + 1);
    }

    // Changes a[i] to x in O(log n).
    void setPoint(int i, int x) {
        rangeAssign(i, i + 1, x);
    }

    // Returns the total sum in O(1).
    int totalSum() {
        return seg[0].sum;
    }

    // Returns the global minimum in O(1).
    int totalMin() {
        return seg[0].mn;
    }

    // Returns the global maximum in O(1).
    int totalMax() {
        return seg[0].mx;
    }

    // Returns the global gcd in O(1).
    int totalGcd() {
        return nodeGcd(seg[0]);
    }

    void minimize(int l, int r, int x) { rangeChmin(l, r, x); }

    void maximize(int l, int r, int x) { rangeChmax(l, r, x); }

    void add(int l, int r, int x) { rangeAdd(l, r, x); }

    void set(int l, int r, int x) { rangeAssign(l, r, x); }

    int get_sum(int l, int r) { return rangeSum(l, r); }

    int get_mn(int l, int r) { return rangeMin(l, r); }

    int get_mx(int l, int r) { return rangeMax(l, r); }

    int get_gcd(int l, int r) { return rangeGcd(l, r); }
};
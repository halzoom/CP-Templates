#include <bits/stdc++.h>

using namespace std;
#define int long long

struct SegmentTree {
#define Lidx (2 * node + 1)
#define Ridx (2 * node + 2)
#define mid ((l + r) / 2)
private:
    vector<int> Seg;
    int SegSize = 1;

    int merge(int a, int b) {
        return a + b;
    }

    void update(int l, int r, int node, int idx, int v) {
        if (l == r) {
            Seg[node] = v;
            return;
        }
        if (mid >= idx) update(l, mid, Lidx, idx, v);
        else update(mid + 1, r, Ridx, idx, v);
        Seg[node] = merge(Seg[Lidx], Seg[Ridx]);
    }

    int query(int l, int r, int node, int Lx, int Rx) {
        if (l > Rx or r < Lx) return 0;
        if (l >= Lx and r <= Rx) return Seg[node];
        int Left = query(l, mid, Lidx, Lx, Rx);
        int Right = query(mid + 1, r, Ridx, Lx, Rx);
        return merge(Left, Right);
    }

    int Kth(int l, int r, int node, int k) {
        if (l == r) return l;
        if (Seg[Lidx] >= k)
            return Kth(l, mid, Lidx, k);
        else
            return Kth(mid + 1, r, Ridx, k - Seg[Lidx]);
    }

public:
    SegmentTree(int n) {
        SegSize = 1;
        while (SegSize < n) SegSize *= 2;
        Seg.assign(2 * SegSize, 0);
    }

    void update(int idx, int v) { update(0, SegSize - 1, 0, idx, v); }

    int query(int l, int r) { return query(0, SegSize - 1, 0, l, r); }

    int Kth(int k) { return Kth(0, SegSize - 1, 0, k); }

#undef Lidx
#undef Ridx
#undef mid
};


vector<int> addOrders(vector<int> &a, vector<int> &b) {
    int n = a.size();
    vector<int> c(n);
    int carry = 0;
    for (int i = n - 1; i >= 0; --i) {
        int radix = n - i;
        int sum = a[i] + b[i] + carry;
        c[i] = sum % radix;
        carry = sum / radix;
    }
    return c;
}

vector<int> getOrder(vector<int> &a) {
    int n = a.size();
    vector<int> ord(n);
    SegmentTree seg(n);
    for (int i = 0; i < n; ++i)
        seg.update(i, 1);

    for (int i = 0; i < n; ++i) {
        seg.update(a[i], 0);
        ord[i] = seg.query(0, a[i]);
    }
    return ord;
}

vector<int> Kth_Perm(vector<int> &coff) {
    int n = coff.size();
    vector<int> answer(n);

    SegmentTree seg(n + 1);
    for (int i = 0; i < n; ++i)
        seg.update(i, 1);

    for (int i = 0; i < n; ++i) {
        int idx = seg.Kth(coff[i] + 1);
        answer[i] = idx;
        seg.update(idx, 0);
    }
    return answer;
}

void solve() {
    int n;
    cin >> n;
    vector<int> a(n), b(n);
    for (int i = 0; i < n; ++i)cin >> a[i];
    for (int i = 0; i < n; ++i)cin >> b[i];

    auto coff1 = getOrder(a);
    auto coff2 = getOrder(b);
    auto coff = addOrders(coff1, coff2);
    auto answer = Kth_Perm(coff);
    for (auto i: answer)
        cout << i << ' ';
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
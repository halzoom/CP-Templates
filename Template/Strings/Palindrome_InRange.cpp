#include<bits/stdc++.h>
using namespace std;
#define int long long

template<typename T>
struct WaveletTree {
    int lo, hi;
    WaveletTree *l = nullptr, *r = nullptr;
    vector<int> b;  // prefix-counts of "go-left"
    vector <ll> c;   // prefix-sums of values
    // Build from [first, last), assuming all values in range [x, y]
    template<typename It>
    WaveletTree(It first, It last, T x, T y) : lo(x), hi(y) {
        int n = distance(first, last);
        if (n <= 0) return;
        T mid = T((lo + hi) >> 1);
        b.reserve(n + 1);
        c.reserve(n + 1);
        b.push_back(0);
        c.push_back(0);
        for (It it = first; it != last; ++it) {
            b.push_back(b.back() + (*it <= mid));
            c.push_back(c.back() + *it);
        }
        if (lo == hi) return;
        auto pivot = stable_partition(first, last, [mid](T v) { return v <= mid; });
        l = new WaveletTree(first, pivot, lo, mid);
        r = new WaveletTree(pivot, last, mid + 1, hi);
    }

    // k-th smallest in [L, R], 1-based k
    T kth(int L, int R, int k) const {
        if (L > R) return T(0);
        if (lo == hi) return lo;
        int inLeft = b[R] - b[L - 1];
        int lb = b[L - 1], rb = b[R];
        if (k <= inLeft) return l->kth(lb + 1, rb, k);
        else return r->kth(L - lb, R - rb, k - inLeft);
    }

    // count <= k in [L, R]
    int LTE(int L, int R, T k) const {
        if (L > R || k < lo) return 0;
        if (hi <= k) return R - L + 1;
        int lb = b[L - 1], rb = b[R];
        return l->LTE(lb + 1, rb, k) + r->LTE(L - lb, R - rb, k);
    }

    // sum of all values <= k in [L, R]
    ll sum(int L, int R, T k) const {
        if (L > R || k < lo) return 0;
        if (hi <= k) return c[R] - c[L - 1];
        int lb = b[L - 1], rb = b[R];
        return l->sum(lb + 1, rb, k) + r->sum(L - lb, R - rb, k);
    }

    ~WaveletTree() {
        delete l;
        delete r;
    }
};

struct PalWavelet {
    int n;
    vector<int> d1, d2;
    vector <ll> A, B, C, D;
    WaveletTree<ll> *oddl = nullptr, *oddr = nullptr, *evenl = nullptr, *evenr = nullptr;
    static constexpr int MAXV = 5000000;

    PalWavelet(const string &s) {
        n = s.size();
        manacher(s, d1, d2);
        A.resize(n + 1);
        B.resize(n + 1);
        C.resize(n + 1);
        D.resize(n + 1);
        for (int i = 1; i <= n; i++) {
            A[i] = ll(d1[i - 1]) - i;
            B[i] = ll(d1[i - 1]) + i;
            C[i] = ll(d2[i - 1]) - i;
            D[i] = ll(d2[i - 1]) + i;
        }
        // build four wavelet trees over 1..n
        oddl = new WaveletTree<ll>(A.begin() + 1, A.end(), -MAXV, MAXV);
        oddr = new WaveletTree<ll>(B.begin() + 1, B.end(), -MAXV, MAXV);
        evenl = new WaveletTree<ll>(C.begin() + 1, C.end(), -MAXV, MAXV);
        evenr = new WaveletTree<ll>(D.begin() + 1, D.end(), -MAXV, MAXV);
    }

    ~PalWavelet() {
        delete oddl;
        delete oddr;
        delete evenl;
        delete evenr;
    }

    // answer number of palindromic substrings sums in [l, r]
    ll query(int l, int r) const { return odd(l, r) + even(l, r); }

private:
    // Manacher algorithm
    static void manacher(const string &s, vector<int> &d1, vector<int> &d2) {
        int n = s.size();
        d1.assign(n, 0);
        for (int i = 0, l = 0, rr = -1; i < n; i++) {
            int k = i > rr ? 1 : min(d1[l + rr - i], rr - i + 1);
            while (i - k >= 0 && i + k < n && s[i - k] == s[i + k]) k++;
            d1[i] = k--;
            if (i + k > rr) {
                l = i - k;
                rr = i + k;
            }
        }
        d2.assign(n, 0);
        for (int i = 0, l = 0, rr = -1; i < n; i++) {
            int k = i > rr ? 0 : min(d2[l + rr - i + 1], rr - i + 1);
            while (i - k - 1 >= 0 && i + k < n && s[i - k - 1] == s[i + k]) k++;
            d2[i] = k--;
            if (i + k > rr) {
                l = i - k - 1;
                rr = i + k;
            }
        }
    }

    static inline ll get_sum(int l, int r) {
        // sum of 1..r minus sum of 1..(l-1)
        return ll(r) * (r + 1) / 2 - ll(l - 1) * l / 2;
    }

    ll odd(int l, int r) const {
        int m = (l + r) >> 1;
        // left half [l..m]
        ll c = 1 - l;
        int less = oddl->LTE(l, m, c);
        ll ansL = get_sum(l, m) + oddl->sum(l, m, c) + ll(m - l + 1 - less) * c;
        // right half [m+1..r]
        c = 1 + r;
        less = oddr->LTE(m + 1, r, c);
        ll ansR = -get_sum(m + 1, r) + oddr->sum(m + 1, r, c) + ll(r - m - less) * c;
        return ansL + ansR;
    }

    ll even(int l, int r) const {
        int m = (l + r) >> 1;
        // left half [l..m]
        ll c = -l;
        int less = evenl->LTE(l, m, c);
        ll ansL = get_sum(l, m) + evenl->sum(l, m, c) + ll(m - l + 1 - less) * c;
        // right half [m+1..r]
        c = 1 + r;
        less = evenr->LTE(m + 1, r, c);
        ll ansR = -get_sum(m + 1, r) + evenr->sum(m + 1, r, c) + ll(r - m - less) * c;
        return ansL + ansR;
    }
};

signed main() {
    string s;
    cin >> s;
    PalWavelet pw(s);
    int q;
    cin >> q;
    while (q--) {
        int l, r;
        cin >> l >> r;
        cout << pw.query(l, r) << "\n";
    }
    return 0;
}
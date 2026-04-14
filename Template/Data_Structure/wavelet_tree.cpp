// from l to r how many values between x and y
#include <bits/stdc++.h>
using namespace std;
const int N = 2e5 + 5;

// wavelet_tree T(a+1, a+n+1, 1, MAX)
struct wavelet_tree {
    vector<int> b;
    int lo, hi;
    wavelet_tree *l, *r;

    // Arrays are in range [x, y]
    // array indices are [from, to)
    wavelet_tree(int *from, int *to, int x, int y) {
        lo = x, hi = y;
        l = r = nullptr;
        if (lo == hi || from >= to) return;

        int mid = (lo + hi) / 2;
        auto f = [mid](int val) {
            return val <= mid;
        };

        b.reserve(to - from + 1);
        b.push_back(0);
        for (auto it = from; it != to; it++)
            b.push_back(b.back() + f(*it));

        auto pivot = stable_partition(from, to, f);
        l = new wavelet_tree(from, pivot, lo, mid);
        r = new wavelet_tree(pivot, to, mid + 1, hi);
    }


    // kth smallest element in [l, r] (1-based indexing)
    int kth(int lpos, int rpos, int k) {
        if (lpos > rpos) return 0;
        if (lo == hi) return lo;

        int inLeft = b[rpos] - b[lpos - 1];
        int lb = b[lpos - 1];  // amount of nos in first (l-1) nos that go to left
        int rb = b[rpos];      // amount of nos in first (r) nos that go to left

        if (k <= inLeft) {
            return this->l->kth(lb + 1, rb, k);
        }
        return this->r->kth(lpos - lb, rpos - rb, k - inLeft);
    }

    // count of numbers in [l, r] Less than or equal to k
    int LTE(int lpos, int rpos, int k) {
        if (lpos > rpos or k < lo) return 0;
        if (hi <= k) return rpos - lpos + 1;

        int lb = b[lpos - 1], rb = b[rpos];
        return this->l->LTE(lb + 1, rb, k) +
               this->r->LTE(lpos - lb, rpos - rb, k);
    }

    // count of numbers in [l, r] equal to k
    int count(int lpos, int rpos, int k) {
        if (lpos > rpos or k < lo or k > hi) return 0;
        if (lo == hi) return rpos - lpos + 1;

        int lb = b[lpos - 1], rb = b[rpos];
        int mid = (lo + hi) / 2;

        if (k <= mid)
            return this->l->count(lb + 1, rb, k);

        return this->r->count(lpos - lb, rpos - rb, k);
    }

    // count of numbers in [l, r] greater than or equal to k
    int GTE(int lpos, int rpos, int k) {
        return (rpos - lpos + 1) - LTE(lpos, rpos, k - 1);
    }

    // Destructor to free memory
    ~wavelet_tree() {
        if (l) delete l;
        if (r) delete r;
    }
};

int a[N + 5];  // 1-based array

void solve() {

    int n, q;
    cin >> n >> q;
    vector<int> mp;
    for (int i = 1; i <= n; ++i)
        cin >> a[i], mp.emplace_back(a[i]);
    vector<array<int, 4>> Q(q);
    for (int i = 0; i < q; ++i) {
        for (int j = 0; j < 4; ++j)
            cin >> Q[i][j];

        mp.emplace_back(Q[i][2]);
        mp.emplace_back(Q[i][3]);
    }
    sort(mp.begin(), mp.end());
    mp.erase(unique(mp.begin(), mp.end()), mp.end());

    for (int i = 1; i <= n; ++i)
        a[i] = lower_bound(mp.begin(), mp.end(), a[i]) - mp.begin() + 1;

    wavelet_tree T(a + 1, a + n + 1, 1, mp.size() + 5);

    for (auto [l, r, x, y]: Q) {
        x = lower_bound(mp.begin(), mp.end(), x) - mp.begin() + 1;
        y = lower_bound(mp.begin(), mp.end(), y) - mp.begin() + 1;
        int answer = T.LTE(l, r, y) - T.LTE(l, r, x - 1);
        cout << answer << '\n';
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
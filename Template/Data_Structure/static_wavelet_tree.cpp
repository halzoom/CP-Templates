#include <bits/stdc++.h>

using namespace std;
#define int long long

// Static Wavelet Tree: positions are 1-based and ranges are inclusive [l,r].
struct StaticWaveletTree {
private:
    struct Node {
        int32_t lo = 0, hi = 0;
        vector<int32_t> pref;
        vector<int64_t> ps;
        unique_ptr<Node> l, r;

        template<class It>
        Node(It from, It to, int32_t _lo, int32_t _hi, const vector<int> &coord) : lo(_lo), hi(_hi) {
            int32_t n = to - from;
            pref.reserve(n + 1);
            ps.reserve(n + 1);
            pref.push_back(0);
            ps.push_back(0);
            for (auto it = from; it != to; ++it)
                ps.push_back(ps.back() + coord[*it]);
            if (!n || lo == hi)return;
            int32_t mid = lo + (hi - lo) / 2;
            auto goLeft = [mid](int32_t x) { return x <= mid; };
            for (auto it = from; it != to; ++it)
                pref.push_back(pref.back() + goLeft(*it));
            auto pivot = stable_partition(from, to, goLeft);
            if (from != pivot)l = make_unique<Node>(from, pivot, lo, mid, coord);
            if (pivot != to)r = make_unique<Node>(pivot, to, mid + 1, hi, coord);
        }

        int32_t kth(int32_t L, int32_t R, int32_t k) const {
            if (lo == hi)return lo;
            int32_t lb = pref[L - 1], rb = pref[R], inLeft = rb - lb;
            if (k <= inLeft)return l->kth(lb + 1, rb, k);
            return r->kth(L - lb, R - rb, k - inLeft);
        }

        int32_t LTE(int32_t L, int32_t R, int32_t x) const {
            if (L > R || x < lo)return 0;
            if (hi <= x)return R - L + 1;
            int32_t lb = pref[L - 1], rb = pref[R], ans = 0;
            if (l)ans += l->LTE(lb + 1, rb, x);
            if (r)ans += r->LTE(L - lb, R - rb, x);
            return ans;
        }

        int32_t count(int32_t L, int32_t R, int32_t x) const {
            if (L > R || x < lo || x > hi)return 0;
            if (lo == hi)return R - L + 1;
            int32_t mid = lo + (hi - lo) / 2;
            int32_t lb = pref[L - 1], rb = pref[R];
            if (x <= mid)return l ? l->count(lb + 1, rb, x) : 0;
            return r ? r->count(L - lb, R - rb, x) : 0;
        }

        int64_t sumLTE(int32_t L, int32_t R, int32_t x) const {
            if (L > R || x < lo)return 0;
            if (hi <= x)return ps[R] - ps[L - 1];
            int32_t lb = pref[L - 1], rb = pref[R];
            int64_t ans = 0;
            if (l)ans += l->sumLTE(lb + 1, rb, x);
            if (r)ans += r->sumLTE(L - lb, R - rb, x);
            return ans;
        }

        int64_t sumKSmallest(int32_t L, int32_t R, int32_t k, const vector<int> &coord) const {
            if (!k || L > R)return 0;
            if (lo == hi)return coord[lo] * k;
            int32_t lb = pref[L - 1], rb = pref[R], inLeft = rb - lb;
            if (k <= inLeft)return l->sumKSmallest(lb + 1, rb, k, coord);
            int64_t leftSum = l ? l->ps[rb] - l->ps[lb] : 0;
            return leftSum + r->sumKSmallest(L - lb, R - rb, k - inLeft, coord);
        }

        int64_t rangeSum(int32_t L, int32_t R) const {
            return L > R ? 0 : ps[R] - ps[L - 1];
        }
    };

    int32_t n = 0;
    vector<int> coord;
    unique_ptr<Node> root;

    int32_t rankLE(int x) const {
        return upper_bound(coord.begin(), coord.end(), x) - coord.begin() - 1;
    }

    int32_t rankLT(int x) const {
        return lower_bound(coord.begin(), coord.end(), x) - coord.begin() - 1;
    }

public:
    StaticWaveletTree() = default;

    StaticWaveletTree(const vector<int> &a) { build(a); }

    // Builds without modifying the original array in O(n log n).
    void build(const vector<int> &a) {
        n = a.size();
        root.reset();
        coord = a;
        sort(coord.begin(), coord.end());
        coord.erase(unique(coord.begin(), coord.end()), coord.end());
        if (!n)return;
        vector<int32_t> rank(n);
        for (int i = 0; i < n; ++i)
            rank[i] = lower_bound(coord.begin(), coord.end(), a[i]) - coord.begin();
        root = make_unique<Node>(rank.begin(), rank.end(), 0, (int32_t) coord.size() - 1, coord);
    }

    // Returns the array size in O(1).
    int size() const { return n; }

    // Checks whether the array is empty in O(1).
    bool empty() const { return !n; }

    // Returns the k-th smallest value in [l,r] in O(log n); k is 1-based.
    int kth(int l, int r, int k) const {
        assert(1 <= l && l <= r && r <= n && 1 <= k && k <= r - l + 1);
        return coord[root->kth(l, r, k)];
    }

    // Returns the minimum value in [l,r] in O(log n).
    int rangeMin(int l, int r) const { return kth(l, r, 1); }

    // Returns the maximum value in [l,r] in O(log n).
    int rangeMax(int l, int r) const { return kth(l, r, r - l + 1); }

    // Returns the lower median of [l,r] in O(log n).
    int median(int l, int r) const { return kth(l, r, (r - l + 2) / 2); }

    // Counts values < x in [l,r] in O(log n).
    int LT(int l, int r, int x) const {
        if (!root)return 0;
        int32_t rk = rankLT(x);
        return rk < 0 ? 0 : root->LTE(l, r, rk);
    }

    // Counts values <= x in [l,r] in O(log n).
    int LTE(int l, int r, int x) const {
        if (!root)return 0;
        int32_t rk = rankLE(x);
        return rk < 0 ? 0 : root->LTE(l, r, rk);
    }

    // Counts values > x in [l,r] in O(log n).
    int GT(int l, int r, int x) const { return r - l + 1 - LTE(l, r, x); }

    // Counts values >= x in [l,r] in O(log n).
    int GTE(int l, int r, int x) const { return r - l + 1 - LT(l, r, x); }

    // Counts occurrences of x in [l,r] in O(log n).
    int count(int l, int r, int x) const {
        if (!root)return 0;
        auto it = lower_bound(coord.begin(), coord.end(), x);
        if (it == coord.end() || *it != x)return 0;
        return root->count(l, r, it - coord.begin());
    }

    // Counts values in [x,y] inside [l,r] in O(log n).
    int rangeFreq(int l, int r, int x, int y) const {
        if (x > y)return 0;
        return LTE(l, r, y) - LT(l, r, x);
    }

    // Returns the sum of all values in [l,r] in O(1).
    int rangeSum(int l, int r) const {
        return root ? root->rangeSum(l, r) : 0;
    }

    // Returns the sum of values < x in [l,r] in O(log n).
    int sumLT(int l, int r, int x) const {
        if (!root)return 0;
        int32_t rk = rankLT(x);
        return rk < 0 ? 0 : root->sumLTE(l, r, rk);
    }

    // Returns the sum of values <= x in [l,r] in O(log n).
    int sumLTE(int l, int r, int x) const {
        if (!root)return 0;
        int32_t rk = rankLE(x);
        return rk < 0 ? 0 : root->sumLTE(l, r, rk);
    }

    // Returns the sum of values in [x,y] inside [l,r] in O(log n).
    int rangeSumByValue(int l, int r, int x, int y) const {
        if (x > y)return 0;
        return sumLTE(l, r, y) - sumLT(l, r, x);
    }

    // Returns the sum of the k smallest values in [l,r] in O(log n).
    int sumKSmallest(int l, int r, int k) const {
        assert(0 <= k && k <= r - l + 1);
        return root ? root->sumKSmallest(l, r, k, coord) : 0;
    }

    // Returns the sum of the k largest values in [l,r] in O(log n).
    int sumKLargest(int l, int r, int k) const {
        assert(0 <= k && k <= r - l + 1);
        return rangeSum(l, r) - sumKSmallest(l, r, r - l + 1 - k);
    }

    // Stores the largest value <= x in ans and returns whether it exists in O(log n).
    bool predecessor(int l, int r, int x, int &ans) const {
        int c = LTE(l, r, x);
        if (!c)return false;
        ans = kth(l, r, c);
        return true;
    }

    // Stores the smallest value >= x in ans and returns whether it exists in O(log n).
    bool successor(int l, int r, int x, int &ans) const {
        int c = LT(l, r, x), len = r - l + 1;
        if (c == len)return false;
        ans = kth(l, r, c + 1);
        return true;
    }

    // Stores the largest value < x in ans and returns whether it exists in O(log n).
    bool previous(int l, int r, int x, int &ans) const {
        int c = LT(l, r, x);
        if (!c)return false;
        ans = kth(l, r, c);
        return true;
    }

    // Stores the smallest value > x in ans and returns whether it exists in O(log n).
    bool next(int l, int r, int x, int &ans) const {
        int c = LTE(l, r, x), len = r - l + 1;
        if (c == len)return false;
        ans = kth(l, r, c + 1);
        return true;
    }
};

using WaveletTree = StaticWaveletTree;
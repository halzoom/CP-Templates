#include <bits/stdc++.h>
using namespace std;
#define int long long

// Dynamic range order statistics with point updates.
// Positions are 1-based, ranges are inclusive [l,r].
// Every future pair {position,newValue} must be passed before build.
struct DynamicWaveletTree {
private:
    struct Meta {
        int32_t posOff = 0, len = 0, bitOff = 0;
    };

    int n = 0, vsz = 1;
    vector<int> a, values;
    vector<Meta> meta;
    vector<int32_t> posFlat, bitCnt;
    vector<int> bitSum;

    int rankOf(int x) const {
        auto it = lower_bound(values.begin(), values.end(), x);
        return it == values.end() || *it != x ? -1 : it - values.begin();
    }

    int lowerRank(int x) const {
        return lower_bound(values.begin(), values.end(), x) - values.begin();
    }

    int upperRank(int x) const {
        return upper_bound(values.begin(), values.end(), x) - values.begin();
    }

    pair<int,int> localRange(int node, int l, int r) const {
        const auto &m = meta[node];
        auto first = posFlat.begin() + m.posOff;
        auto last = first + m.len;
        int L = lower_bound(first, last, (int32_t)l) - first;
        int R = upper_bound(first, last, (int32_t)r) - first;
        return {L, R};
    }

    int bitQueryCnt(int node, int k) const {
        const auto &m = meta[node];
        int ans = 0;
        for (int i = k; i > 0; i -= i & -i)
            ans += bitCnt[m.bitOff + i];
        return ans;
    }

    int bitQuerySum(int node, int k) const {
        const auto &m = meta[node];
        int ans = 0;
        for (int i = k; i > 0; i -= i & -i)
            ans += bitSum[m.bitOff + i];
        return ans;
    }

    void bitAdd(int node, int pos, int dc, int ds) {
        const auto &m = meta[node];
        auto first = posFlat.begin() + m.posOff;
        auto last = first + m.len;
        int idx = lower_bound(first, last, (int32_t)pos) - first;
        assert(idx < m.len && first[idx] == pos);
        for (int i = idx + 1; i <= m.len; i += i & -i) {
            bitCnt[m.bitOff + i] += dc;
            bitSum[m.bitOff + i] += ds;
        }
    }

    int nodeCount(int node, int l, int r) const {
        if (!meta[node].len || l > r)return 0;
        auto [L, R] = localRange(node, l, r);
        return bitQueryCnt(node, R) - bitQueryCnt(node, L);
    }

    int nodeSum(int node, int l, int r) const {
        if (!meta[node].len || l > r)return 0;
        auto [L, R] = localRange(node, l, r);
        return bitQuerySum(node, R) - bitQuerySum(node, L);
    }

    void change(int pos, int rank, int dc, int value) {
        for (int node = vsz + rank; node; node >>= 1)
            bitAdd(node, pos, dc, dc * value);
    }

    int countRanks(int l, int r, int ql, int qr) const {
        if (ql > qr || ql >= (int)values.size() || qr < 0)return 0;
        ql = max<int>(ql, 0);
        qr = min<int>(qr, values.size() - 1);
        int ans = 0;
        for (ql += vsz, qr += vsz; ql <= qr; ql >>= 1, qr >>= 1) {
            if (ql & 1)ans += nodeCount(ql++, l, r);
            if (!(qr & 1))ans += nodeCount(qr--, l, r);
        }
        return ans;
    }

    int sumRanks(int l, int r, int ql, int qr) const {
        if (ql > qr || ql >= (int)values.size() || qr < 0)return 0;
        ql = max<int>(ql, 0);
        qr = min<int>(qr, values.size() - 1);
        int ans = 0;
        for (ql += vsz, qr += vsz; ql <= qr; ql >>= 1, qr >>= 1) {
            if (ql & 1)ans += nodeSum(ql++, l, r);
            if (!(qr & 1))ans += nodeSum(qr--, l, r);
        }
        return ans;
    }

public:
    DynamicWaveletTree() = default;

    DynamicWaveletTree(const vector<int> &arr, const vector<pair<int,int>> &futureUpdates = {}) {
        build(arr, futureUpdates);
    }

    // Builds in O((n+u) log(n+u) log sigma); updates contain {1-based position,newValue}.
    void build(const vector<int> &arr, const vector<pair<int,int>> &futureUpdates = {}) {
        n = arr.size();
        a.assign(n + 1, 0);
        values = arr;
        for (auto [pos, x]: futureUpdates) {
            assert(1 <= pos && pos <= n);
            values.push_back(x);
        }
        sort(values.begin(), values.end());
        values.erase(unique(values.begin(), values.end()), values.end());

        vsz = 1;
        while (vsz < max<int>(1, values.size()))vsz <<= 1;

        vector<vector<int32_t>> tmp(2 * vsz);

        auto registerPair = [&](int pos, int x) {
            int rk = rankOf(x);
            for (int node = vsz + rk; node; node >>= 1)
                tmp[node].push_back(pos);
        };

        for (int i = 1; i <= n; ++i) {
            a[i] = arr[i - 1];
            registerPair(i, a[i]);
        }

        for (auto [pos, x]: futureUpdates)
            registerPair(pos, x);

        meta.assign(2 * vsz, {});
        int totalPos = 0, totalBit = 0;

        for (int node = 1; node < 2 * vsz; ++node) {
            auto &v = tmp[node];
            sort(v.begin(), v.end());
            v.erase(unique(v.begin(), v.end()), v.end());
            meta[node].posOff = totalPos;
            meta[node].len = v.size();
            meta[node].bitOff = totalBit;
            totalPos += v.size();
            totalBit += v.size() + 1;
        }

        posFlat.resize(totalPos);

        for (int node = 1; node < 2 * vsz; ++node) {
            auto &v = tmp[node];
            copy(v.begin(), v.end(), posFlat.begin() + meta[node].posOff);
            vector<int32_t>().swap(v);
        }

        vector<vector<int32_t>>().swap(tmp);

        bitCnt.assign(totalBit, 0);
        bitSum.assign(totalBit, 0);

        for (int i = 1; i <= n; ++i)
            change(i, rankOf(a[i]), 1, a[i]);
    }

    // Returns the array size in O(1).
    int size() const { return n; }

    // Checks whether the array is empty in O(1).
    bool empty() const { return !n; }

    // Checks whether set(pos,x) was registered before build in O(log n).
    bool canSet(int pos, int x) const {
        int rk = rankOf(x);
        if (rk < 0 || pos < 1 || pos > n)return false;
        int node = vsz + rk;
        const auto &m = meta[node];
        auto first = posFlat.begin() + m.posOff;
        return binary_search(first, first + m.len, (int32_t)pos);
    }

    // Changes a[pos] to x in O(log n log sigma); {pos,x} must be registered before build.
    void set(int pos, int x) {
        assert(canSet(pos, x));
        if (a[pos] == x)return;
        change(pos, rankOf(a[pos]), -1, a[pos]);
        a[pos] = x;
        change(pos, rankOf(a[pos]), 1, a[pos]);
    }

    // Returns a[pos] in O(1).
    int get(int pos) const {
        assert(1 <= pos && pos <= n);
        return a[pos];
    }

    // Returns the k-th smallest value in [l,r] in O(log n log sigma); k is 1-based.
    int kth(int l, int r, int k) const {
        assert(1 <= l && l <= r && r <= n && 1 <= k && k <= r - l + 1);
        int node = 1;
        while (node < vsz) {
            int left = node << 1;
            int c = nodeCount(left, l, r);
            if (k <= c)node = left;
            else k -= c, node = left | 1;
        }
        int rk = node - vsz;
        assert(rk < (int)values.size());
        return values[rk];
    }

    // Returns the minimum value in [l,r] in O(log n log sigma).
    int rangeMin(int l, int r) const { return kth(l, r, 1); }

    // Returns the maximum value in [l,r] in O(log n log sigma).
    int rangeMax(int l, int r) const { return kth(l, r, r - l + 1); }

    // Returns the lower median of [l,r] in O(log n log sigma).
    int median(int l, int r) const { return kth(l, r, (r - l + 2) / 2); }

    // Counts values < x in [l,r] in O(log n log sigma).
    int LT(int l, int r, int x) const {
        int rk = lowerRank(x) - 1;
        return countRanks(l, r, 0, rk);
    }

    // Counts values <= x in [l,r] in O(log n log sigma).
    int LTE(int l, int r, int x) const {
        int rk = upperRank(x) - 1;
        return countRanks(l, r, 0, rk);
    }

    // Counts values > x in [l,r] in O(log n log sigma).
    int GT(int l, int r, int x) const { return r - l + 1 - LTE(l, r, x); }

    // Counts values >= x in [l,r] in O(log n log sigma).
    int GTE(int l, int r, int x) const { return r - l + 1 - LT(l, r, x); }

    // Counts occurrences of x in [l,r] in O(log n log sigma).
    int count(int l, int r, int x) const {
        int rk = rankOf(x);
        return rk < 0 ? 0 : countRanks(l, r, rk, rk);
    }

    // Counts values in [x,y] inside [l,r] in O(log n log sigma).
    int rangeFreq(int l, int r, int x, int y) const {
        if (x > y)return 0;
        return LTE(l, r, y) - LT(l, r, x);
    }

    // Returns the sum of all values in [l,r] in O(log n).
    int rangeSum(int l, int r) const {
        return nodeSum(1, l, r);
    }

    // Returns the sum of values < x in [l,r] in O(log n log sigma).
    int sumLT(int l, int r, int x) const {
        int rk = lowerRank(x) - 1;
        return sumRanks(l, r, 0, rk);
    }

    // Returns the sum of values <= x in [l,r] in O(log n log sigma).
    int sumLTE(int l, int r, int x) const {
        int rk = upperRank(x) - 1;
        return sumRanks(l, r, 0, rk);
    }

    // Returns the sum of values in [x,y] inside [l,r] in O(log n log sigma).
    int rangeSumByValue(int l, int r, int x, int y) const {
        if (x > y)return 0;
        return sumLTE(l, r, y) - sumLT(l, r, x);
    }

    // Returns the sum of the k smallest values in [l,r] in O(log n log sigma).
    int sumKSmallest(int l, int r, int k) const {
        assert(0 <= k && k <= r - l + 1);
        if (!k)return 0;
        int node = 1, ans = 0;
        while (node < vsz) {
            int left = node << 1;
            int c = nodeCount(left, l, r);
            if (k <= c)node = left;
            else {
                ans += nodeSum(left, l, r);
                k -= c;
                node = left | 1;
            }
        }
        if (k)ans += values[node - vsz] * k;
        return ans;
    }

    // Returns the sum of the k largest values in [l,r] in O(log n log sigma).
    int sumKLargest(int l, int r, int k) const {
        assert(0 <= k && k <= r - l + 1);
        return rangeSum(l, r) - sumKSmallest(l, r, r - l + 1 - k);
    }

    // Stores the largest value <= x in ans and returns whether it exists.
    bool predecessor(int l, int r, int x, int &ans) const {
        int c = LTE(l, r, x);
        if (!c)return false;
        ans = kth(l, r, c);
        return true;
    }

    // Stores the smallest value >= x in ans and returns whether it exists.
    bool successor(int l, int r, int x, int &ans) const {
        int c = LT(l, r, x), len = r - l + 1;
        if (c == len)return false;
        ans = kth(l, r, c + 1);
        return true;
    }

    // Stores the largest value < x in ans and returns whether it exists.
    bool previous(int l, int r, int x, int &ans) const {
        int c = LT(l, r, x);
        if (!c)return false;
        ans = kth(l, r, c);
        return true;
    }

    // Stores the smallest value > x in ans and returns whether it exists.
    bool next(int l, int r, int x, int &ans) const {
        int c = LTE(l, r, x), len = r - l + 1;
        if (c == len)return false;
        ans = kth(l, r, c + 1);
        return true;
    }
};
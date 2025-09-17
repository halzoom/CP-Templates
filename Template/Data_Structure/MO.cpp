#include<bits/stdc++.h>
using namespace std;
#define int long long

struct MO {
    vector<int> v, frq;
    int B = 0, n, ans = 0;

    MO(vector<int> &a) {
        v = a;
        n = (int) a.size();
        B = sqrt(n) + 1;
        frq.assign(n + 1, {});
    }

    void add(int idx) {}

    void erase(int idx) {}

    vector<int> done(vector <array<int, 3>> &query) {
        sort(query.begin(), query.end(), [&](array<int, 3> a, array<int, 3> b) {
            return make_pair(a[0] / B, a[1]) < make_pair(b[0] / B, b[1]);
        });
        vector<int> ret(query.size());
        int l = query[0][0], r = l;
        add(l);
        for (const auto [lq, rq, idx]: query) {
            while (lq < l) --l, add(l);
            while (rq > r) ++r, add(r);
            while (lq > l) erase(l), ++l;
            while (rq < r) erase(r), --r;
            ret[idx] = ans;
        }
        return ret;
    }
};

// Mo optimizations
int block = 0;

// 1
struct Query {
    int l, r, idx;

    Query(int L, int R, int i) { l = L, r = R, idx = i; }

    inline pair<int, int> toPair() const {
        return make_pair(l / block, ((l / block) & 1) ? -r : +r);
    }
};

// 2
inline int64_t hilbertOrder(int x, int y, int pow, int rotate) {
    if (pow == 0) { return 0; }
    int hpow = 1 << (pow - 1);
    int seg = (x < hpow) ? ((y < hpow) ? 0 : 3) : ((y < hpow) ? 1 : 2);
    seg = (seg + rotate) & 3;
    const int rotateDelta[4] = {3, 0, 0, 1};
    int nx = x & (x ^ hpow), ny = y & (y ^ hpow);
    int nrot = (rotate + rotateDelta[seg]) & 3;
    int64_t subSquareSize = int64_t(1) << (2 * pow - 2);
    int64_t ans = seg * subSquareSize;
    int64_t add = hilbertOrder(nx, ny, pow - 1, nrot);
    ans += (seg == 1 || seg == 2) ? add : (subSquareSize - add - 1);
    return ans;
}

struct Query {
    int l, r, idx;
    int64_t ord;  // Hilbert order value
    Query(int ll, int rr, int iidx) {
        l = ll, r = rr, idx = iidx;
        ord = hilbertOrder(l, r, 21, 0);
    }
};

bool operator<(const Query &a, const Query &b) { return a.ord < b.ord; }
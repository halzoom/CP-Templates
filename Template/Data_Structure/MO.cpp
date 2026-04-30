#include<bits/stdc++.h>

using namespace std;
#define int long long
const int N = 1 << 20;
int block = 460;

struct Query {
    int l, r, idx;

    Query() {};

    Query(int L, int R, int i) { l = L, r = R, idx = i; }

    inline pair<int, int> toPair() const {
        return make_pair(l / block, ((l / block) & 1) ? -r : +r);
    }
};

struct MO {
    vector<int> v, frq;
    int n, answer = 0;

    MO(vector<int> &a) {
        v = a;
        n = a.size();
        frq.assign(N, {});
    }

    void add(int idx) {
        if (++frq[v[idx]] == 1)answer++;
    }

    void erase(int idx) {
        if (--frq[v[idx]] == 0)answer--;
    }

    vector<int> go(vector<Query> &query) {
        sort(query.begin(), query.end(), [&](Query a, Query b) {
            return make_pair(a.l / block, a.r) < make_pair(b.l / block, b.r);
        });
        vector<int> ret(query.size());
        int l = query[0].l, r = l;
        add(l);
        for (const auto [lq, rq, idx]: query) {
            while (lq < l) --l, add(l);
            while (rq > r) ++r, add(r);
            while (lq > l) erase(l), ++l;
            while (rq < r) erase(r), --r;
            ret[idx] = answer;
        }
        return ret;
    }
};

void solve() {
    int n;
    cin >> n;
    vector<int> a(n + 1);
    for (int i = 1; i <= n; ++i)
        cin >> a[i];

    int q;
    cin >> q;
    vector<Query> Q(q);
    for (int i = 0, l, r; i < q; ++i) {
        cin >> l >> r;
        Q[i] = {l, r, i};
    }

    MO T(a);
    auto answer = T.go(Q);
    for (auto i: answer)
        cout << i << '\n';
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


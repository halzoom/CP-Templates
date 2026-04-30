#include <bits/stdc++.h>

using namespace std;
#define int long long
const int inf = 1e18;
struct Query {
    int l, r, t, id;
};

struct Update {
    int pos, old, newv;
};

struct MoWithUpdates {
    int n, answer = 0, B;
    vector<int> a, res, cur, frq, frqOfrq, SQ;
    vector<Query> queries;
    vector<Update> updates;

    MoWithUpdates(vector<int> &_a) {
        n = _a.size();
        a = _a;
        B = pow(n, 2.0 / 3.0) + 1;
        frq.assign(2 * n + 1, {});
        frqOfrq.assign(n + 1, {});
        SQ.assign(B + 1, {});
        SQ[0] = 1;
        frqOfrq[0] = inf;
    }

    void addUpdate(int pos, int val) {
        updates.push_back({pos, 0, val});
    }

    void addQuery(int l, int r) {
        queries.push_back({l, r, (int) updates.size(), (int) queries.size()});
    }

    void add(int x) {
        x = cur[x];
        if (--frqOfrq[frq[x]] == 0)SQ[frq[x] / B]--;
        frq[x]++;
        if (++frqOfrq[frq[x]] == 1)SQ[frq[x] / B]++;
    }

    void erase(int x) {
        x = cur[x];
        if (--frqOfrq[frq[x]] == 0)SQ[frq[x] / B]--;
        frq[x]--;
        if (++frqOfrq[frq[x]] == 1)SQ[frq[x] / B]++;
    }

    int get() {
        int idx = 0;
        while (SQ[idx] == B)++idx;
        idx = idx * B;
        while (frqOfrq[idx])++idx;
        return idx;
    }

    vector<int> Process() {
        sort(queries.begin(), queries.end(), [&](const Query &a, const Query &b) {
            int bl = a.l / B, bl2 = b.l / B;
            if (bl != bl2) return bl < bl2;
            int br = a.r / B, br2 = b.r / B;
            if (br != br2) return (bl & 1) ? br > br2 : br < br2;
            return (br & 1) ? a.t > b.t : a.t < b.t;
        });
        cur = a;
        for (auto &u: updates) {
            u.old = cur[u.pos];
            cur[u.pos] = u.newv;
        }
        res.assign(queries.size(), 0);
        cur = a;
        int l = 1, r = 0, t = 0;

        auto apply = [&](int idx) {
            int pos = updates[idx].pos;
            int newV = updates[idx].newv;

            if (l <= pos and pos <= r) {
                erase(pos);
                cur[pos] = newV;
                add(pos);
            } else {
                cur[pos] = newV;
            }
        };

        auto cancel = [&](int idx) {
            int pos = updates[idx].pos;
            int oldV = updates[idx].old;

            if (l <= pos and pos <= r) {
                erase(pos);
                cur[pos] = oldV;
                add(pos);
            } else {
                cur[pos] = oldV;
            }
        };

        for (const auto &[lq, rq, tq, idx]: queries) {
            while (t < tq) apply(t++);
            while (t > tq) cancel(--t);

            while (l > lq) add(--l);
            while (r < rq) add(++r);
            while (l < lq) erase(l++);
            while (r > rq) erase(r--);
            res[idx] = get();
        }

        return res;
    }
};

void solve() {
    int n, q;
    cin >> n >> q;
    vector<int> a(n + 1);
    map<int, int> mp;
    for (int i = 1; i <= n; ++i)
        cin >> a[i], mp[a[i]];

    vector<array<int, 3>> Q(q);
    for (int i = 0; i < q; ++i) {
        for (int j = 0; j < 3; ++j)
            cin >> Q[i][j];

        if (Q[i][0] == 2)
            mp[Q[i][2]];
    }

    int id = 1;
    for (auto &[_, v]: mp)
        v = id++;
    for (int i = 1; i <= n; ++i)
        a[i] = mp[a[i]];

    MoWithUpdates T(a);
    for (auto [t, l, r]: Q) {
        if (t == 1)
            T.addQuery(l, r);
        else
            T.addUpdate(l, mp[r]);
    }
    auto answer = T.Process();
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
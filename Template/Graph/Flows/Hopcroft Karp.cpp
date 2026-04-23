#include<bits/stdc++.h>

using namespace std;
#define int long long

// O(E * sqrt(V))
struct HopCroftKarp {
    int n, m;
    vector<vector<int>> g;
    vector<int> l, r, d;
    int ans;

    HopCroftKarp(int n, int m) : n(n), m(m), g(n), l(n, -1), r(m, -1), ans(0) {}

    void add_edge(int u, int v) { g[u].push_back(v); }

    int match() {
        while (true) {
            queue<int> q;
            d.assign(n, -1);
            for (int i = 0; i < n; i++)
                if (l[i] == -1) q.push(i), d[i] = 0;
            while (!q.empty()) {
                int x = q.front();
                q.pop();
                for (int y: g[x])
                    if (r[y] != -1 and d[r[y]] == -1) d[r[y]] = d[x] + 1, q.push(r[y]);
            }
            bool match = false;
            for (int i = 0; i < n; i++)
                if (l[i] == -1 and dfs(i)) ++ans, match = true;
            if (!match) break;
        }
        return ans;
    }

    bool dfs(int x) {
        for (int y: g[x])
            if (r[y] == -1 or (d[r[y]] == d[x] + 1 and dfs(r[y])))
                return l[x] = y, r[y] = x, d[x] = -1, true;
        return d[x] = -1, false;
    }
};

void solve() {
    int n, m, k;
    cin >> n >> m >> k;
    HopCroftKarp T(n + 3, m + 1);

    vector<vector<int>> valid(n + 1);
    for (int i = 0, u, v; i < k; ++i) {
        cin >> u >> v;
        valid[u].emplace_back(v);
        T.add_edge(u, v);
    }
    int match = T.match(), answer = match;
    auto left = T.l;
    auto right = T.r;
    for (int i = 1; i <= n; ++i) {
        int m1 = n + 1, m2 = n + 2;
        T.g[m1] = valid[i];
        T.g[m2] = valid[i];
        answer = max(answer, T.match());
        T.ans = match;
        T.l = left, T.r = right;
        T.g[m1].clear();
        T.g[m2].clear();
    }
    cout << answer << '\n';
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
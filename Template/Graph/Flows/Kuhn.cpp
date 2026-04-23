#include<bits/stdc++.h>

using namespace std;
// O(N * M)
struct Kuhn {
    vector<vector<int>> adj;
    vector<int> matchL, matchR, vis;
    int n, m, timer = 1;

    Kuhn(int n, int m) : n(n), m(m), matchL(n + 1, -1),
                         matchR(m + 1, -1), vis(n + 1), adj(n + 1) {};

    void addEdge(int u, int v) {
        adj[u].emplace_back(v);
    }


    bool try_kuhn(int u, int limit) {
        if (u <= limit or vis[u] == timer) return false;
        vis[u] = timer;
        for (auto v: adj[u]) {
            if (matchR[v] == -1 or try_kuhn(matchR[v], limit)) {
                matchR[v] = u;
                matchL[u] = v;
                return true;
            }
        }
        return false;
    }

    int max_matching() {
        int answer = 0;
        for (int i = 1; i <= n; ++i) {
            if (matchL[i] != -1)continue;
            timer++;
            if (try_kuhn(i, 0))
                answer++;
        }
        return answer;
    }

    bool force(int li, int ri) {
        if (matchL[li] == ri) return true;
        int u = matchR[ri];
        int v = matchL[li];
        matchL[li] = ri;
        matchR[ri] = li;
        matchL[u] = -1;
        matchR[v] = -1;

        timer++;
        if (try_kuhn(u, li))
            return true;

        matchL[li] = v;
        matchR[v] = li;
        matchR[ri] = u;
        matchL[u] = ri;
        return false;
    }
};

void solve() {
    int n, m;
    cin >> n >> m;
    vector<set<int>> adj(n + 1);
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= n; ++j)
            adj[i].insert(j);
    for (int i = 0, u, v; i < m; ++i) {
        cin >> u >> v;
        adj[u].erase(v);
    }

    Kuhn T(n, n);
    for (int i = 1; i <= n; ++i)
        for (auto u: adj[i])
            T.addEdge(i, u);
    if (T.max_matching() != n)
        return void(cout << -1 << '\n');
    vector<int> used(n + 1);
    for (int i = 1; i <= n; ++i) {
        for (auto v: adj[i]) {
            if (!used[v] and T.force(i, v)) {
                used[v] = true;
                break;
            }
        }
    }
    for (int i = 1; i <= n; ++i)
        cout << T.matchL[i] << ' ';
    cout << '\n';
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
#ifdef HALZOOM
    freopen("Input.txt", "r", stdin);
    freopen("Output.txt", "w", stdout);
#endif

    int test = 1;
    cin >> test;

    for (int i = 1; i <= test; ++i) {
        solve();
    }
    return 0;
}
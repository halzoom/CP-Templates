#include<bits/stdc++.h>
using namespace std;
#define int long long

const int mod = 2131131137, N = 1e5 + 5, B = 219;
int in[2][N], out[2][N], pw[N], timer = 1;
vector <vector<int>> adj[2];

void tour(int u, int p, int t) {
    in[t][u] = timer++;
    for (auto v: adj[t][u]) {
        if (v == p) continue;
        tour(v, u, t);
    }
    out[t][u] = timer;
}

int dfs(int u, int p, int t) {
    int hash = '(';
    vector <pair<int, int>> res;  // hash , number of nodes
    for (auto v: adj[t][u]) {
        if (v == p) continue;
        int h = dfs(v, u, t);
        res.emplace_back(h, out[t][v] - in[t][v]);
    }
    sort(res.begin(), res.end());
    for (auto [h, cnt]: res) { hash = (hash * pw[cnt] % mod + h) % mod; }
    hash = (hash * B + ')') % mod;
    return hash;
}

void solve() {
    int n;
    cin >> n;
    auto get = [&](int t) {
        adj[t].assign(n + 1, {});
        for (int i = 1, a, b; i < n; ++i) {
            cin >> a >> b;
            adj[t][a].emplace_back(b);
            adj[t][b].emplace_back(a);
        }
    };
    get(0), get(1);
    tour(1, 0, 0);
    timer = 1;
    tour(1, 0, 1);
    timer = 1;
    int hash1 = dfs(1, 0, 0);
    int hash2 = dfs(1, 0, 1);
    cout << (hash1 == hash2 ? "YES" : "NO") << '\n';
}
// pw[0] = 1;
// for (int i = 1; i < N; ++i) { pw[i] = pw[i - 1] * B % mod; }

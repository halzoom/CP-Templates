#include<bits/stdc++.h>
using namespace std;
#define int long long

const int mod = 2131131137, N = 1e5 + 5, B = 219;
int in[2][N], out[2][N], pw[N], timer = 1, n;
vector <vector<int>> adj[2];

void tour(int u, int p, int t) {
    in[t][u] = timer++;
    for (auto v: adj[t][u]) {
        if (v == p) continue;
        tour(v, u, t);
    }
    out[t][u] = timer;
}

vector<int> getCentroid(int u, int p, int t) {
    vector<int> result;
    bool isCentroid = true;
    for (auto v: adj[t][u]) {
        if (v == p) continue;
        auto answer = getCentroid(v, u, t);
        for (auto c: answer) result.emplace_back(c);
        if (out[t][v] - in[t][v] > n / 2) isCentroid = false;
    }
    if (n - (out[t][u] - in[t][u]) > n / 2) isCentroid = false;
    if (isCentroid) result.emplace_back(u);
    return result;
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
    auto Centroid1 = getCentroid(1, 0, 0);
    auto Centroid2 = getCentroid(1, 0, 1);
    for (auto i: Centroid1) {
        for (auto j: Centroid2) {
            tour(i, 0, 0);
            timer = 1;
            tour(j, 0, 1);
            if (dfs(i, 0, 0) == dfs(j, 0, 1)) {
                cout << "YES\n";
                return;
            }
        }
    }
    cout << "NO\n";
}
// pw[0] = 1;
// for (int i = 1; i < N; ++i) { pw[i] = pw[i - 1] * B % mod; }
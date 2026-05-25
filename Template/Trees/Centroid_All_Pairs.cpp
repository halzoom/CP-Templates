#include <bits/stdc++.h>

using namespace std;
#define ll long long
const int N = 2e5 + 5;
int sz[N], removed[N], frq[N];
ll answer, k;
vector<vector<int>> adj;

int get_size(int u, int p) {
    sz[u] = 1;
    for (auto v: adj[u]) {
        if (v == p or removed[v])continue;
        sz[u] += get_size(v, u);
    }
    return sz[u];
}

int get_centroid(int u, int p, int s) {
    for (auto v: adj[u]) {
        if (v == p or removed[v])continue;
        if (sz[v] * 2 > s)return get_centroid(v, u, s);
    }
    return u;
}

void add(int u, int p, int op, int depth) {
    frq[depth] += op;
    for (auto v: adj[u]) {
        if (v == p or removed[v]) continue;
        add(v, u, op, depth + 1);
    }
}

void get(int u, int p, int depth) {
    if (depth <= k) answer += frq[k - depth];
    for (auto v: adj[u]) {
        if (v == p or removed[v]) continue;
        get(v, u, depth + 1);
    }
}

void decompose(int u) {
    int size = get_size(u, 0);
    int centroid = get_centroid(u, 0, size);
    frq[0] = 1;
    for (auto v: adj[centroid]) {
        if (removed[v]) continue;
        get(v, centroid, 1);
        add(v, centroid, 1, 1);
    }
    add(centroid, 0, -1, 0);

    removed[centroid] = 1;
    for (auto v: adj[centroid])
        if (!removed[v])decompose(v);
}

void solve() {
    int n;
    cin >> n >> k;
    adj.assign(n + 1, {});
    for (int i = 1, u, v; i < n; ++i) {
        cin >> u >> v;
        adj[u].emplace_back(v);
        adj[v].emplace_back(u);
    }
    decompose(1);
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
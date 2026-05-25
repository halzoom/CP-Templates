#include <bits/stdc++.h>

using namespace std;
#define int long long
const int N = 2e5 + 5, lg = 22, inf = 1e18;
int sz[N], removed[N], parent[N], level[N], dist[lg][N];
vector<int> adj[N];

int get_size(int u, int p) {
    sz[u] = 1;
    for (auto v: adj[u]) {
        if (v == p or removed[v])continue;
        sz[u] += get_size(v, u);
    }
    return sz[u];
}

void add_edge_to_centroid_tree(int u, int p) {
    parent[u] = p;
    level[u] = level[p] + 1;
}

int get_centroid(int u, int p, int s) {
    for (auto v: adj[u]) {
        if (v == p or removed[v])continue;
        if (sz[v] * 2 > s)return get_centroid(v, u, s);
    }
    return u;
}

void process(int u, int p, int lvl) {
    for (auto v: adj[u]) {
        if (v == p or removed[v])continue;
        dist[lvl][v] = dist[lvl][u] + 1;
        process(v, u, lvl);
    }
}

void decompose(int u, int p) {
    int size = get_size(u, -1);
    int centroid = get_centroid(u, 0, size);
    add_edge_to_centroid_tree(centroid, p);
    process(centroid, 0, level[centroid]);
    removed[centroid] = true;
    for (auto v: adj[centroid]) {
        if (removed[v]) continue;
        decompose(v, centroid);
    }
}

void init(int n) {
    for (int i = 0; i <= n; ++i) {
        removed[i] = parent[i] = level[i] = 0;
        adj[i].clear();
        for (int j = 0; j < lg; ++j)
            dist[j][i] = 0;
    }
}

void solve() {
    int n, q;
    cin >> n >> q;
    init(n);
    for (int i = 1, u, v; i < n; ++i) {
        cin >> u >> v;
        adj[u].emplace_back(v);
        adj[v].emplace_back(u);
    }

    vector<int> min_centroid(n + 1, inf);
    decompose(1, 0);
    auto update = [&](int u) {
        int cur = u;
        while (cur) {
            min_centroid[cur] = min(min_centroid[cur], dist[level[cur]][u]);
            cur = parent[cur];
        }
    };
    auto go = [&](int u) {
        int answer = inf;
        int cur = u;
        while (cur) {
            answer = min(answer, dist[level[cur]][u] + min_centroid[cur]);
            cur = parent[cur];
        }
        return answer;
    };
    update(1);
    while (q--) {
        int t, u;
        cin >> t >> u;
        if (t == 1)
            update(u);
        else
            cout << go(u) << '\n';
    }
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
#include <bits/stdc++.h>

using namespace std;
#define ll long long
const int N = 2e5 + 5;
int sz[N], removed[N], centroid_tree_root = -1, answer[N];
vector<int> adj[N], CentroidTree[N];

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

int decompose(int u) {
    int size = get_size(u, -1);
    int centroid = get_centroid(u, -1, size);
    if (centroid_tree_root == -1) centroid_tree_root = centroid;
    removed[centroid] = true;
    for (auto v: adj[centroid]) {
        if (removed[v]) continue;
        int x = decompose(v);
        CentroidTree[centroid].emplace_back(x);
        CentroidTree[x].emplace_back(centroid);
    }
    return centroid;
}

void dfs(int u, int p, int id) {
    answer[u] = id;
    for (auto v: CentroidTree[u]) {
        if (v == p)continue;
        dfs(v, u, id + 1);
    }
}

void solve() {
    int n;
    cin >> n;
    for (int i = 1, u, v; i < n; ++i) {
        cin >> u >> v;
        adj[u].emplace_back(v);
        adj[v].emplace_back(u);
    }

    decompose(1);
    dfs(centroid_tree_root, 0, 0);
    for (int i = 1; i <= n; ++i)
        cout << char(answer[i] + 'A') << ' ';
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
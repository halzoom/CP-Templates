#include<bits/stdc++.h>
using namespace std;
#define int long long

// check if vertices in subtree u of level h can form a palindrome string
const int N = 5e5 + 5;
int in[N], out[N], tree[N], level[N], timer = 1;
vector <vector<int>> adj;

void tour(int u, int p) {
    tree[timer] = u;
    in[u] = timer++;
    level[u] = level[p] + 1;
    for (auto v: adj[u]) {
        if (v == p) continue;
        tour(v, u);
    }
    out[u] = timer;
}

int frq[N][26], answer[N];
vector <pair<int, int>> query[N];
string s;

void add(int u) { frq[level[u]][s[u]]++; }

void erase(int u) { frq[level[u]][s[u]]--; }

int get(int h) {
    int res = 0;
    for (int i = 0; i < 26; ++i) res += frq[h][i] & 1;
    return res < 2;
}

void dfs(int u, int p, int keep) {
    int bgChild = -1, maxS = -1;
    for (auto v: adj[u]) {
        if (v == p or out[v] - in[v] <= maxS) continue;
        maxS = out[v] - in[v];
        bgChild = v;
    }
    for (auto v: adj[u]) {
        if (v == p or v == bgChild) continue;
        dfs(v, u, 0);
    }
    if (~bgChild) dfs(bgChild, u, 1);
    for (auto v: adj[u]) {
        if (v == p or v == bgChild) continue;
        for (int i = in[v]; i < out[v]; ++i) add(tree[i]);
    }
    add(u);
    for (auto [h, i]: query[u]) answer[i] = get(h);
    if (!keep) {
        for (int i = in[u]; i < out[u]; ++i) erase(tree[i]);
    }
}

void solve() {
    int n, q;
    cin >> n >> q;
    adj.assign(n + 1, {});
    for (int i = 2, a; i <= n; ++i) {
        cin >> a;
        adj[a].emplace_back(i);
        adj[i].emplace_back(a);
    }
    cin >> s;
    s = '#' + s;
    for (auto &i: s) i -= 'a';
    for (int i = 0, u, h; i < q; ++i) {
        cin >> u >> h;
        query[u].emplace_back(h, i);
    }
    tour(1, 0);
    dfs(1, 0, 0);
    for (int i = 0; i < q; ++i) cout << (answer[i] ? "Yes" : "No") << '\n';
}
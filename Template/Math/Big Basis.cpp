// https://vjudge.net/contest/784608#problem/O
#include <bits/stdc++.h>

using namespace std;
#define int long long
const int lg = 171, N = 2e5 + 5, M = 170;
typedef bitset<M> bigInt;
const int mod = 998244353;

struct Basis {
    int size = 0;
    vector<bigInt> basis;
    vector<int> vis;

    Basis() {
        size = 0;
        basis.assign(lg, {});
        vis.assign(lg, {});
    };

    bool on(bigInt n, int bit) {
        return n[bit];
    }

    bool can(bigInt n) {
        for (int i = lg - 1; ~i; --i)
            if (vis[i] and on(n, i))
                n ^= basis[i];
        return n == 0;
    }

    bool insert(bigInt n) {
        for (int i = lg - 1; ~i; --i) {
            if (!on(n, i))continue;
            if (!vis[i]) {
                vis[i] = 1;
                basis[i] = n;
                size++;
                return true;
            }
            n ^= basis[i];
        }
        return false;
    }

    bool merge(Basis &w) {
        bool repeat = false;
        for (int i = 0; i < lg; ++i)
            if (w.vis[i] and insert(w.basis[i]))
                repeat = true;
        return repeat;
    }
};

vector<vector<int>> adj;
vector<Basis> dp;
bigInt a[N], b[N], sum[N], mask[1001];
int sz[N];

void go(int u, int p) {
    sz[u] = 1;
    sum[u] = a[u];
    dp[u].insert(a[u] ^ b[u]);
    for (auto v: adj[u]) {
        if (v == p)continue;
        go(v, u);
        dp[u].merge(dp[v]);
        sz[u] += sz[v];
        sum[u] ^= sum[v];
    }
}

void solve() {
    int n;
    cin >> n;
    adj.assign(n + 1, {});
    dp.assign(n + 1, {});
    for (int i = 1, x; i <= n; ++i)
        cin >> x, a[i] = mask[x];
    for (int i = 1, x; i <= n; ++i)
        cin >> x, b[i] = mask[x];
    for (int i = 1, u, v; i < n; ++i) {
        cin >> u >> v;
        adj[u].emplace_back(v);
        adj[v].emplace_back(u);
    }
    go(1, 0);
    int q;
    cin >> q;
    vector<int> pw(n + 1, 1);
    for (int i = 1; i <= n; ++i)
        pw[i] = pw[i - 1] * 2 % mod;
    while (q--) {
        int u;
        cin >> u;
        if (dp[u].can(sum[u]))
            cout << pw[sz[u] - dp[u].size] << '\n';
        else if (!sum[u].any() and sz[u] > dp[u].size)
            cout << pw[sz[u] - dp[u].size] << '\n';
        else
            cout << 0 << '\n';
    }
}


bool isPrime(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
#ifdef HALZOOM
    freopen("Input.txt", "r", stdin);
    freopen("Output.txt", "w", stdout);
#endif
    vector<int> id(1001);
    for (int i = 1, cnt = 0; i <= 1000; ++i)
        if (isPrime(i))
            id[i] = cnt++;

    for (int i = 1; i <= 1000; ++i) {
        int n = i;
        for (int j = 2; j <= 1000; ++j) {
            if (!isPrime(j))continue;
            while (n % j == 0) n /= j, mask[i].flip(id[j]);
        }
    }

    int test = 1;
//    cin >> test;

    for (int i = 1; i <= test; ++i) {
        solve();
    }
    return 0;
}
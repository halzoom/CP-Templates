#include <bits/stdc++.h>

using namespace std;
const int N = 1e6 + 5, mod = 998244353;
int dp[N], frq[N], pw[N], chg[N];
vector<int> Div[N];
vector<pair<int, int>> trans[N];

int exp(int base, int pow) {
    if (pow < 0)return 0;
    int res = 1;
    while (pow) {
        if (pow & 1) res = 1ll * res * base % mod;
        base = 1ll * base * base % mod, pow >>= 1;
    }
    return res;
}

void update(int m, int op) {
    for (auto d: Div[m]) {
        if (op == 1) {
            chg[d] = pw[frq[d]];
            frq[d]++;
        } else {
            frq[d]--;
            chg[d] = mod - pw[frq[d]];
        }
    }

    for (auto [d, nd]: trans[m]) {
        chg[d] -= chg[nd];
        if (chg[d] < 0)chg[d] += mod;
    }

    for (auto d: Div[m]) {
        dp[d] += chg[d];
        if (dp[d] >= mod)dp[d] -= mod;
    }
}

void solve() {
    int n;
    cin >> n;
    vector<int> a(n + 1);
    pw[0] = 1;
    for (int i = 1; i <= n; ++i)
        cin >> a[i], frq[a[i]]++, pw[i] = pw[i - 1] * 2ll % mod;

    for (int i = 1; i < N; ++i)
        for (int j = 2 * i; j < N; j += i)
            frq[i] += frq[j];

    for (int i = N - 1; i; --i) {
        dp[i] = pw[frq[i]] - 1;
        if (dp[i] < 0)dp[i] += mod;
        for (int j = 2 * i; j < N; j += i) {
            dp[i] -= dp[j];
            if (dp[i] < 0)dp[i] += mod;
        }
    }

    int total = exp(pw[n] - 1, mod - 2);
    int q;
    cin >> q;
    while (q--) {
        int t, x, g;
        cin >> t >> x;
        if (t == 1)
            cout << 1ll * dp[x] * total % mod << '\n';
        else {
            cin >> g;
            if (g == a[x])continue;
            update(a[x], -1);
            a[x] = g;
            update(a[x], 1);
        }
    }
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
#ifdef HALZOOM
    freopen("Input.txt", "r", stdin);
    freopen("Output.txt", "w", stdout);
#endif

    for (int i = 1; i < N; ++i)
        for (int j = i; j < N; j += i)
            Div[j].emplace_back(i);

    for (int m = 1; m < N; ++m) {
        for (auto p: Div[m]) {
            if (Div[p].size() != 2)continue;
            for (auto d: Div[m]) {
                if (m / d % p)continue;
                trans[m].emplace_back(d, d * p);
            }
        }
    }

    int test = 1;
//    cin >> test;

    for (int i = 1; i <= test; ++i)
        solve();
    return 0;
}
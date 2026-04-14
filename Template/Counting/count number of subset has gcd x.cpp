#include <bits/stdc++.h>

using namespace std;
const int N = 1e5 + 5, mod = 998244353;
int dp[N], sumDp[N], frq[N], pw[N];
vector<int> Div[N];

int exp(int base, int pow) {
    if (pow < 0)return 0;
    int res = 1;
    while (pow) {
        if (pow & 1) res = 1ll * res * base % mod;
        base = 1ll * base * base % mod, pow >>= 1;
    }
    return res;
}

void solve() {
    int n;
    cin >> n;
    vector<int> a(n + 1);
    pw[0] = 1;
    for (int i = 1; i <= n; ++i)
        cin >> a[i], frq[a[i]]++, pw[i] = pw[i - 1] * 2 % mod;

    for (int i = 1; i < N; ++i)
        for (int j = 2 * i; j < N; j += i)
            frq[i] += frq[j];

    for (int i = N - 1; i; --i) {
        dp[i] = pw[frq[i]] - 1;
        for (int j = 2 * i; j < N; j += i) {
            dp[i] -= dp[j];
            sumDp[i] += dp[j];
            if (dp[i] < 0)dp[i] += mod;
            if (sumDp[i] >= mod)sumDp[i] -= mod;
        }
    }

    auto update = [&](int m, int op = 1) {
        for (auto d: Div[m]) {
            frq[d] += op;
            int old = dp[d];
            dp[d] = pw[frq[d]] - 1 - sumDp[d];
            if (dp[d] < 0)dp[d] += mod;

            for (auto div: Div[d]) {
                if (div == d)continue;
                sumDp[div] += dp[d] - old;
                if (sumDp[div] < 0)sumDp[div] += mod;
                if (sumDp[div] >= mod)sumDp[div] -= mod;
            }
        }
    };

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

    for (int i = 1; i < N; ++i) {
        for (int j = i; j < N; j += i)
            Div[j].emplace_back(i);
        reverse(Div[i].begin(), Div[i].end());
    }
    int test = 1;
//    cin >> test;

    for (int i = 1; i <= test; ++i) {
        solve();
    }
    return 0;
}
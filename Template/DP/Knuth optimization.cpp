#include <bits/stdc++.h>

using namespace std;
#define int long long
const int inf = 1e18;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n + 1);
    for (int i = 1; i <= n; ++i)
        cin >> a[i], a[i] += a[i - 1];
    vector<vector<int>> dp(n + 2, vector<int>(n + 2, inf));
    vector<vector<int>> opt(n + 2, vector<int>(n + 2));
    for (int i = 1; i <= n; ++i)
        dp[i][i] = 0, opt[i][i] = i;

    for (int len = 2; len <= n; ++len) {
        for (int l = 1; l + len - 1 <= n; ++l) {
            int r = l + len - 1;
            for (int k = opt[l][r - 1]; k <= opt[l + 1][r]; ++k) {
                int add = a[r] - a[l - 1];
                if (dp[l][r] > dp[l][k] + dp[k + 1][r] + add) {
                    dp[l][r] = dp[l][k] + dp[k + 1][r] + add;
                    opt[l][r] = k;
                }
            }
        }
    }
    cout << dp[1][n] << '\n';
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
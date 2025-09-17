// Your task is to count the number of ways you can
// fill an n * m grid using 1* 2

#include <bits/stdc++.h>

using namespace std;
#define int long long
const int mod = 1e9 + 7;
int n, m;
vector<int> dp, cur_dp;

void go(int i, int j, int mask, int nxt) {
    if (i == n) {
        cur_dp[nxt] += dp[mask];
        if (cur_dp[nxt] >= mod)cur_dp[nxt] -= mod;
        return;
    }

    if (mask >> i & 1)
        return go(i + 1, j, mask, nxt);

    if (i + 1 < n and (mask >> i & 1) + (mask >> (i + 1) & 1) == 0)
        go(i + 2, j, mask, nxt);

    if (j + 1 < m)
        go(i + 1, j, mask, nxt ^ 1 << i);
}

void solve() {
    cin >> n >> m;
    if (n > m)swap(n, m);
    dp.assign(1 << n, {});
    cur_dp.assign(1 << n, {});
    dp[0] = 1;
    for (int i = 0; i < m; ++i) {
        cur_dp.assign(1 << n, {});
        for (int mask = 0; mask < 1 << n; ++mask) {
            go(0, i, mask, 0);
        }
        swap(dp, cur_dp);
    }
    cout << dp[0] << '\n';
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
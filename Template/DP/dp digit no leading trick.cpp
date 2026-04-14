#include <bits/stdc++.h>

using namespace std;
#define int long long

int dp[19][1 << 10][10][2];
string s;

int go(int idx, int mask, int mx, int lead, int tie) {
    if (idx == -1)
        return mx == __builtin_popcount(mask) and lead;

    int &dpi = dp[idx][mask][mx][tie];
    if (tie and ~dpi)return dpi;
    int res = 0;
    int up = tie ? 9 : s[idx] - '0';
    for (int d = 0; d <= up; ++d) {
        int ntie = tie or (d < s[idx] - '0');
        if (!lead and d == 0)
            res += go(idx - 1, 0, 0, false, ntie);
        else
            res += go(idx - 1, mask | 1 << d, max(d, mx), true, ntie);
    }
    return tie ? dpi = res : res;
}

void solve() {
    memset(dp, -1, sizeof dp);
    int T;
    cin >> T;
    // number of distinct = max
    while (T--) {
        cin >> s;
        reverse(s.begin(), s.end());
        cout << go(s.size() - 1, 0, 0, 0, 0) << '\n';
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
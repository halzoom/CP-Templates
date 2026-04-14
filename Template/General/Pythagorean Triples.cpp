#include <bits/stdc++.h>


// Count number of Pythagorean primitive triples (a^2 + b^2 = c^2)
using namespace std;
const int N = 2e7 + 5;
vector<int> SQ;
int dp[N];
void solve() {
    for (int m = 2; m * m < N; m++) {
        for (int n = 1; n < m; n++) {
            if (((m - n) & 1) == 0) continue;
            if (gcd(m, n) != 1) continue;

            int a = m * m - n * n;
            int b = 2 * m * n;
            int c = m * m + n * n;

            if (c >= N) break;
            if (a <= 0 or a >= b) continue;
            dp[c]++;
        }
    }

    for (int i = 1; i < N; i++)
        dp[i] += dp[i - 1];
    int T;
    cin >> T;
    while (T--) {
        int n;
        cin >> n;
        cout << dp[n] << '\n';
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
    for (int i = 1; i * i <= N; ++i)
        SQ.emplace_back(i * i);
    for (int i = 1; i <= test; ++i) {
        solve();
    }
    return 0;
}
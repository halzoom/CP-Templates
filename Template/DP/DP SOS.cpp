#include <bits/stdc++.h>

using namespace std;
#define int long long

const int B = 20, mod = 1e9 + 7;
const int M = 1 << B;

// subset contribute to its superset
void forward(vector<int> &dp) {
    for (int i = 0; i < B; ++i)
        for (int m = 0; m < M; ++m)
            if (m & (1 << i))
                dp[m] += dp[m ^ (1 << i)];
}

// superset contribute to its subset
void forwardRev(vector<int> &dp) {
    for (int i = 0; i < B; ++i)
        for (int m = M - 1; ~m; --m)
            if (m & (1 << i))
                dp[m ^ (1 << i)] += dp[m];
}

// remove subset contribution from superset
void backward(vector<int> &dp) {
    for (int i = 0; i < B; ++i)
        for (int m = M - 1; ~m; --m)
            if (m & (1 << i))
                dp[m] -= dp[m ^ (1 << i)];
}

// remove superset contribution from subset
void backwardRev(vector<int> &dp) {
    for (int i = 0; i < B; ++i)
        for (int m = 0; m < M; ++m)
            if (m & (1 << i))
                dp[m ^ (1 << i)] -= dp[m];
}

void solve() {
    int n;
    cin >> n;
    vector<int> dp(M);
    for (int i = 0, x; i < n; ++i)
        cin >> x, dp[x]++;

    forwardRev(dp);
    vector<int> pw(M, 1);
    for (int i = 1; i < M; ++i)
        pw[i] = pw[i - 1] * 2 % mod;

    for (auto &i: dp)
        i = pw[i] - 1;

    backwardRev(dp);
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
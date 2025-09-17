#include<bits/stdc++.h>
using namespace std;
#define int long long

// count number of pairs has gcd equal to x
const int N = 1e6 + 6;
int cnt[N], dp[N];

void solve() {
    int n, mx = 0;
    cin >> n;
    for (int i = 0, t; i < n; ++i) {
        cin >> t;
        mx = max(mx, t);
        cnt[t]++;
    }
    for (int i = mx; i; --i) {
        int mul = 0;
        for (int j = i; j <= mx; j += i) { mul += cnt[j]; }
        dp[i] = mul * (mul - 1) / 2;
        for (int j = i + i; j <= mx; j += i) dp[i] -= dp[j];
    }
    cout << dp[1];
}

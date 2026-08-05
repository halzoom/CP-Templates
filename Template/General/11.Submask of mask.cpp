#include<bits/stdc++.h>

using namespace std;

int main() {
    int n;
    cin >> n;
    vector<int> dp(1LL << n, -1e8), cost(1 << n);
    dp[0] = 0;
    for (int mask = 1; mask < (1LL << n); ++mask) {
        for (int sub = mask; sub; sub = (sub - 1) & mask) {
            dp[mask] = max(dp[mask], dp[mask ^ sub] + cost[sub]);
        }
    }
    cout << dp[(1LL << n) - 1] << '\n';
    return 0;
}
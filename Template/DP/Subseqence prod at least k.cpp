#include <bits/stdc++.h>

using namespace std;
#define int long long
const int mod = 1e9 + 7;

int exp(int base, int pow) {
    if (pow < 0)return 0;
    int res = 1;
    while (pow) {
        if (pow & 1) res = (res * base) % mod;
        base = (base * base) % mod, pow >>= 1;
    }
    return res;
}
// O(n * sqrt(k))
void solve() {
    int n, k;
    cin >> n >> k;
    vector<int> a(n);
    for (int &x: a) cin >> x;

    int K = k - 1;
    int B = sqrtl(K);

    while ((B + 1) * (B + 1) <= K) B++;
    while (B * B > K) B--;

    int C = K / (B + 1);
    vector<int> states;
    for (int need = 1; need <= B + 1; ++need)
        states.push_back(need);
    for (int i = C; i >= 1; --i)
        states.push_back(K / i + 1);

    auto getId = [&](int need) {
        if (need <= B + 1)
            return need - 1;

        int q = need - 1;
        int i = K / q;
        return B + 1 + C - i;
    };

    vector<int> dp(states.size());
    dp[getId(k)] = 1;
    int ones = 0;
    for (int x: a) {
        if (x == 1) {
            ones++;
            continue;
        }
        x = min(x, k);
        for (int i = 0; i < (int) states.size(); ++i) {
            int newNeed = (states[i] + x - 1) / x;
            int j = getId(newNeed);
            dp[j] += dp[i];
            if (dp[j] >= mod)
                dp[j] -= mod;
        }
    }
    cout << dp[0] * exp(2, ones) % mod << '\n';
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
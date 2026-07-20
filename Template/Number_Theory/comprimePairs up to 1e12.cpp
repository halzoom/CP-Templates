#include <bits/stdc++.h>

using namespace std;
#define int long long

const int mod = 1e9 + 7, inv2 = 500000004;
int lim;
vector<int> prefix, pr;
unordered_map<int, int> dp;

void sieve() {
    prefix.assign(lim + 1, 0);
    if (lim >= 1) prefix[1] = 1;
    for (int i = 2; i <= lim; ++i) {
        if (prefix[i] == 0) {
            prefix[i] = i - 1;
            pr.push_back(i);
        }
        for (int p: pr) {
            int x = 1LL * i * p;
            if (x > lim) break;
            if (i % p == 0) {
                prefix[x] = prefix[i] * p;
                break;
            }
            prefix[x] = prefix[i] * (p - 1);
        }
    }

    for (int i = 1; i <= lim; ++i) {
        prefix[i] += prefix[i - 1];
        if (prefix[i] >= mod)
            prefix[i] -= mod;
    }
}

int sumPhi(int n) {
    if (n <= lim)
        return prefix[n];

    auto it = dp.find(n);
    if (it != dp.end())
        return it->second;

    int answer = n % mod;
    answer = answer * ((n + 1) % mod) % mod;
    answer = answer * inv2 % mod;
    for (int left = 2, right; left <= n; left = right + 1) {
        int quotient = n / left;
        right = n / quotient;
        int count = (right - left + 1) % mod;
        answer -= count * sumPhi(quotient) % mod;
        if (answer < 0)
            answer += mod;
    }
    return dp[n] = answer;
}

void solve() {
    int n;
    cin >> n;
    int value = pow((int) n, 2.0L / 3.0L) * 1.6L;
    lim = min<int>(n, (int) value + 10);
    sieve();
    dp.reserve(2 * n / lim + 100);
    dp.max_load_factor(0.7);
    cout << sumPhi(n) << '\n';
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
#include <bits/stdc++.h>

using namespace std;
#define int long long
const int mod = 1e9 + 7, N = 1e5 + 5;

// mu(1)=1, mu(n)=0 if p^2|n, otherwise mu(n)=(-1)^(number of distinct prime factors).
// sum_{d|n} mu(d) = [n==1], so [gcd(a,b)==1] = sum_{d|a,d|b} mu(d).
// Think Mobius when gcd=1/exact gcd is required and counting values divisible by d is easy.
// Coprime pairs = sum mu(d)*C(cnt[d],2), gcd-1 subsets = sum mu(d)*(2^cnt[d]-1).
// Inversion: F(n)=sum_{d|n}f(d) => f(n)=sum_{d|n}mu(d)*F(n/d).
vector<int> Mobius(int n) {
    vector<int> mu(n + 1), primes;
    vector<bool> composite(n + 1);
    mu[1] = 1;
    for (int i = 2; i <= n; ++i) {
        if (!composite[i]) {
            primes.push_back(i);
            mu[i] = -1;
        }
        for (int p: primes) {
            if (i * p > n) break;
            composite[i * p] = true;
            if (i % p == 0) {
                mu[i * p] = 0;
                break;
            }
            mu[i * p] = -mu[i];
        }
    }
    return mu;
}

void solve() {
    auto mu = Mobius(N);
    vector<int> dp(N), sum(N);
    vector<vector<int>> Div(N);
    for (int i = 1; i < N; ++i) {
        for (int j = i; j < N; j += i) {
            if (mu[i] == 0)continue;
            Div[j].emplace_back(i);
        }
    }

    for (int i = 1; i < N; ++i) {
        dp[i] = 1;
        for (auto d: Div[i])
            dp[i] = (dp[i] + mu[d] * sum[d] % mod + mod) % mod;

        for (auto d: Div[i])
            sum[d] = (sum[d] + dp[i]) % mod;
    }
    for (int i = 1; i < N; ++i)
        dp[i] = (dp[i] + dp[i - 1]) % mod;
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

    for (int i = 1; i <= test; ++i) {
        solve();
    }
    return 0;
}
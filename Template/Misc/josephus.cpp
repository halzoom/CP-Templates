#include <bits/stdc++.h>

using namespace std;
#define int long long

// O(K log(N))
int josephus(int n, int k) {
    if (n == 1) return 0;
    if (k == 1) return n - 1;
    if (k > n)
        return (josephus(n - 1, k) + k) % n;

    int cnt = n / k;
    int res = josephus(n - cnt, k);
    res -= n % k;
    if (res < 0) res += n;
    else res += res / (k - 1);
    return res;
}

void solve() {
    int n, m;
    cin >> n >> m;
    cout << josephus(n, m) + 1 << '\n';
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
#ifdef HALZOOM
    freopen("Input.txt", "r", stdin);
    freopen("Output.txt", "w", stdout);
#endif

    int test = 1;
    cin >> test;

    for (int i = 1; i <= test; ++i) {
        solve();
    }
    return 0;
}
#include <bits/stdc++.h>

using namespace std;
#define int long long
const int mod = 1e9 + 7;
using row = vector<int>;
using matrix = vector<row>;

matrix mul(const matrix &a, const matrix &b) {
    int n = a.size() ,m = a[0].size() ,k = b[0].size();
    matrix res(n, row(k, 0));
    for (int i = 0; i < n; ++i) {
        for (int o = 0; o < m; ++o) {
            if (a[i][o] == 0) continue;
            for (int j = 0; j < k; ++j) {
                res[i][j] = (res[i][j] + a[i][o] * b[o][j]) % mod;
            }
        }
    }
    return res;
}

matrix exp(matrix &a, int b) {
    int n = a.size();
    matrix res(n, row(n));
    for (int i = 0; i < n; ++i) res[i][i] = 1;

    while (b) {
        if (b & 1) res = mul(res, a);
        a = mul(a, a), b >>= 1;
    }
    return res;
}

void solve() {
    int n;
    cin >> n;
    matrix state = {{0, 1}};
    matrix T = {{0, 1},
                {1, 1}};
    T = exp(T, n);
    auto answer = mul(state, T);
    cout << answer[0][0] << '\n';
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
        cout << setprecision(10) << fixed;
        solve();
    }
    return 0;
}
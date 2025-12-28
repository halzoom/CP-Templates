#include <bits/stdc++.h>

using namespace std;
#define int long long


// a^x = b (mod m) -> get x
bool BSGS(int a, int b, int m, int &x) {
    a %= m, b %= m;
    int k = 1, add = 0, g;
    while ((g = gcd(a, m)) > 1) {
        if (b == k) {
            x = add;
            return true;
        }
        if (b % g)
            return false;
        b /= g, m /= g, ++add;
        k = (k * 1 * a / g) % m;
    }

    int n = sqrtl(m) + 1;
    int an = 1;
    for (int i = 0; i < n; ++i)
        an = (an * 1 * a) % m;

    map<int, int> vals;
    for (int q = 0, cur = b; q <= n; ++q) {
        vals[cur] = q;
        cur = (cur * 1 * a) % m;
    }

    for (int p = 1, cur = k; p <= n; ++p) {
        cur = (cur * 1 * an) % m;
        if (vals.count(cur)) {
            int ans = n * p - vals[cur] + add;
            x = ans;
            return true;
        }
    }
    return false;
}

void solve() {
    int a, b, n, x;
    cin >> a >> b >> n;
    if (a == 0)
        return void(cout << "1\n");
    BSGS(a, b, n, x);
    cout << x << '\n';
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
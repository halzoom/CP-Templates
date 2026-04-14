#include <bits/stdc++.h>

using namespace std;
#define int long long

int exp(int base, int pow, int mod) {
    if (pow < 0)return 0;
    int res = 1;
    while (pow) {
        if (pow & 1) res = (res * base) % mod;
        base = (base * base) % mod, pow >>= 1;
    }
    return res;
}

// a / b
int KthDecDigit(int a, int b, int k) {
    int d = 10 * (a * exp(10, (k - 1) % b, b) % b) / b;
    return d;
}

void solve() {
    int a, b, c;
    cin >> a >> b >> c;

    for (int i = 1; i <= b; ++i) {
        int d = KthDecDigit(a, b, i);
        if (d == c)
            return void(cout << i << '\n');
    }
    cout << "-1\n";

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
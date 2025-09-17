#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <cmath>
#include <numeric>

using namespace std;
typedef long long ll;
const ll oo = 1e18;
const int N = 3e6 + 10;
const int mod = 1e9 + 7;
vector<int> fact(N);
vector<int> inv(N);

int add(int a, int b) {
    return (a + b) % mod;
}

int mul(int a, int b) {
    return (1LL * a * b) % mod;
}

int fastpow(int a, int b) {
    int res = 1;
    while (b) {
        if (b & 1)
            res = mul(res, a);
        a = mul(a, a);
        b >>= 1;
    }
    return res;
}

int modinv(int a) {
    return fastpow(a, mod - 2);
}

void init() {
    fact[0] = 1;
    for (int i = 1; i < N; i++)
        fact[i] = mul(fact[i - 1], i);
    inv[N - 1] = modinv(fact[N - 1]);
    for (int i = N - 2; i >= 0; i--)
        inv[i] = mul(inv[i + 1], i + 1);
}

int nCr(int n, int r) {
    if (r > n || r < 0)
        return 0;
    return mul(fact[n], mul(inv[r], inv[n - r]));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    //init();
    int t = 1;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        ll sum = 0;
        vector<ll> l(n), r(n);
        for (int i = 0; i < n; ++i) cin >> l[i] >> r[i], sum += r[i] - l[i];
        vector<ll> lr(n);
        ll suml = 0;
        for (int i = 0; i < n; ++i) {
            lr[i] = l[i] + r[i];
            suml += l[i];
        }
        vector<int> idx(n);
        for (int i = 0; i < n; ++i) idx[i] = i;
        sort(idx.begin(), idx.end(), [&](int a, int b) {
            if (lr[a] != lr[b]) return lr[a] > lr[b];
            return a < b;
        });
        vector<int> pos(n);
        for (int i = 0; i < n; ++i) pos[idx[i]] = i;
        vector<ll> pref(n + 1, 0);
        for (int i = 0; i < n; ++i) pref[i + 1] = pref[i] + lr[idx[i]];

        if (n % 2 == 0) {
            int mid = n / 2;
            ll ans = pref[mid] - suml;
            cout << sum + ans << '\n';
        } else {
            int mid = n / 2;
            ll ans = -oo;
            for (int k = 0; k < n; ++k) {
                int p = pos[k];
                ll temp = 0;
                if (p < mid) {
                    temp = pref[mid + 1] - lr[k];
                } else {
                    temp = pref[mid];
                }
                temp = temp - suml + l[k];
                if (temp > ans) ans = temp;
            }
            cout << sum + ans << '\n';
        }
    }
}

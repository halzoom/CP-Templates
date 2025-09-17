#include<bits/stdc++.h>
using namespace std;
#define int long long

vector<int> linear_sieve(int n) {
    vector<int> lp(n + 1);
    vector<int> pr;
    for (int i = 2; i <= n; ++i) {
        if (lp[i] == 0) {
            lp[i] = i;
            pr.push_back(i);
        }
        for (int j = 0; i * pr[j] <= n; ++j) {
            lp[i * pr[j]] = pr[j];
            if (pr[j] == lp[i]) { break; }
        }
    }
    return lp;
}

struct HashString {
    const int A = 31;
    const int B = 991831889;
    vector<int> pows, sums;

    HashString(string s) {
        int n = s.size();
        pows.resize(n + 1);
        pows[0] = 1;
        sums.resize(n + 1);
        sums[0] = 0;
        for (int i = 1; i <= n; i++) {
            pows[i] = pows[i - 1] * A % B;
            sums[i] = (sums[i - 1] * A + s[i - 1]) % B;
        }
    }

    int hash(int a, int b) {
        int h = sums[b + 1] - sums[a] * pows[b - a + 1];
        return (h % B + B) % B;
    }
};

void solve() {
    int n;
    string s;
    cin >> n >> s;
    HashString Hash(s);
    auto lp = linear_sieve(s.size());
    auto minPeriod = [&](int l, int r) -> long long {
        if ((l == r) || Hash.hash(l, r - 1) == Hash.hash(l + 1, r)) return 1;
        int len = (r - l + 1), ans = len;
        while (len > 1) {
            if (Hash.hash(l, r - ans / lp[len]) == Hash.hash(l + ans / lp[len], r))
                ans /= lp[len];
            len /= lp[len];
        }
        return ans;
    };
    int q;
    cin >> q;
    while (q--) {
        int l, r;
        cin >> l >> r;
        cout << minPeriod(l - 1, r - 1) << '\n';
    }
}
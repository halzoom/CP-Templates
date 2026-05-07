#include <bits/stdc++.h>

using namespace std;
#define int long long

vector<int> manacher_odd(string s) {
    int n = s.size();
    s = "$" + s + "^";
    vector<int> p(n + 2);
    int l = 0, r = 1;
    for (int i = 1; i <= n; i++) {
        p[i] = min(r - i, p[l + (r - i)]);
        while (s[i - p[i]] == s[i + p[i]]) p[i]++;
        if (i + p[i] > r) l = i - p[i], r = i + p[i];
    }
    return vector<int>(begin(p) + 1, end(p) - 1);
}


// from s to manacher i -> 2 * i + 1;
// from manacher to s i -> (i - 1) / 2;
vector<int> manacher(string s) {
    string t;
    for (auto c: s) { t += string("#") + c; }
    auto res = manacher_odd(t + "#");
    return res;
}

void solve() {
    string s;
    cin >> s;

    auto man = manacher(s);

    auto isPal = [&](int l, int r) { // 1-indexed
        --l, --r;
        l = 2 * l + 1, r = 2 * r + 1;
        int len = (r - l + 1);
        int mid = l + len / 2;
        return man[mid] - 1 >= (len + 1) / 2;
    };
    int idx = 0, len = 0;
    for (int i = 1; i < man.size(); ++i) {
        if (man[i] - 1 > len) {
            len = man[i] - 1;
            idx = i;
        }
    }
    idx = (idx - len) / 2;
    cout << s.substr(idx, len) << '\n';
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
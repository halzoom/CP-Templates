#include <bits/stdc++.h>

using namespace std;
#define int long long
string minRotation(string s) {
    int n = s.size();
    s += s;
    int best = 0;
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            if (best + k == i or s[best + k] < s[i + k]) {
                i += max(0ll, k - 1);
                break;
            }
            if (s[best + k] > s[i + k]) {
                best = i;
                break;
            }
        }
    }
    return s.substr(best, n);
}

void solve() {
    string s;
    cin >> s;
    cout << minRotation(s) << '\n';
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
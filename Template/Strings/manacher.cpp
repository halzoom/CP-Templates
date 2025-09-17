#include<bits/stdc++.h>
using namespace std;
#define int long long

// Extend to Palindrome -> append character to make the string palindrome
vector<int> manacher_odd(string s) {
    int n = s.size();
    s = "$" + s + "^";
    vector<int> p(n + 2);
    int l = 0, r = 1;
    for (int i = 1; i <= n; i++) {
        p[i] = min(r - i, p[l + (r - i)]);
        while (s[i - p[i]] == s[i + p[i]]) { p[i]++; }
        if (i + p[i] > r) { l = i - p[i], r = i + p[i]; }
    }
    return vector<int>(begin(p) + 1, end(p) - 1);
}

vector<int> manacher(string s) {
    string t;
    for (auto c: s) { t += string("#") + c; }
    auto res = manacher_odd(t + "#");
    return res;
}

signed main() {
    string s;
    while (cin >> s) {
        auto x = manacher(s);
        int maxIDX = s.size();
        for (int i = 1; i < x.size() - 1; ++i) {
            x[i]--;
            int start = (i - x[i]) / 2;
            if (start + x[i] == s.size()) { maxIDX = min(maxIDX, start); }
        }
        cout << s;
        --maxIDX;
        while (~maxIDX) cout << s[maxIDX--];
        cout << '\n';
    }
}
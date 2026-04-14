#include <bits/stdc++.h>
using namespace std;
#define int long long
const int N = 2e5 + 5;

struct SuffixArray {
    // suff is the suffix array with the empty suffix being suff[0]
    // lcp[i] holds the lcp between sa[i], sa[i - 1]
    // pos is rank of index i in suffix
    int n;
    vector<int> suff, lcp, pos;

    SuffixArray(string &s, int lim = 256) {
        n = s.size() + 1;
        int k = 0, a, b;
        vector<int> c(s.begin(), s.end() + 1), tmp(n), frq(max(n, lim));
        c.back() = 0;
        suff = lcp = pos = tmp, iota(suff.begin(), suff.end(), 0);
        for (int j = 0, p = 0; p < n; j = max(1ll, j * 2), lim = p) {
            p = j, iota(tmp.begin(), tmp.end(), n - j);
            for (int i = 0; i < n; i++)
                if (suff[i] >= j) tmp[p++] = suff[i] - j;
            fill(frq.begin(), frq.end(), 0);
            for (int i = 0; i < n; i++) frq[c[i]]++;
            for (int i = 1; i < lim; i++) frq[i] += frq[i - 1];
            for (int i = n; i--;) suff[--frq[c[tmp[i]]]] = tmp[i];
            swap(c, tmp), p = 1, c[suff[0]] = 0;
            for (int i = 1; i < n; i++) {
                a = suff[i - 1], b = suff[i];
                c[b] = tmp[a] == tmp[b] && tmp[a + j] == tmp[b + j] ? p - 1 : p++;
            }
        }
        for (int i = 1; i < n; i++) pos[suff[i]] = i;
        for (int i = 0, j; i < n - 1; lcp[pos[i++]] = k)
            for (k &&k--, j = suff[pos[i] - 1]; s[i + k] == s[j + k];
        k++) {}
    }
};


// it take the suffix array from 1 to end (remove empty string)
const int K = 26, MN = 'a';
struct find_bwt {
    int n;
    char last;
    vector<array<int, K>> occ;
    array<int, K + 1> cnt;

    find_bwt(const string &s, const vector<int> &sa) : n(ssize(s)), last(s.empty() ? -1 : s.back() - MN), occ(n + 1) {
        fill(begin(cnt), end(cnt), 0);
        for (int i = 0; i < n; i++) {
            cnt[s[i] + 1 - MN]++;
            occ[i + 1] = occ[i];
            if (sa[i] == 0) continue;
            occ[i + 1][s[sa[i] - 1] - MN]++;
        }
        partial_sum(begin(cnt), end(cnt), begin(cnt));
    }

    // return the range (l <= t < r) in the sa so you need to shift it +1 to original suffix
    pair<int, int> find_str(const string &t) const {
        int sa_le = 0, sa_ri = n;
        for (int i = ssize(t) - 1, x = 0; sa_le < sa_ri && i >= 0; i--) {
            char c = t[i] - MN;
            sa_le = cnt[c] + occ[sa_le][c] + (c == last && x);
            sa_ri = cnt[c] + occ[sa_ri][c] + (c == last);
            x = 1;
        }
        return {sa_le, sa_ri};
    }
};

void solve() {
    string s;
    cin >> s;
    SuffixArray T(s);
    find_bwt G(s, vector(T.suff.begin() + 1, T.suff.end()));
    int q;
    cin >> q;
    while (q--) {
        int m;
        string t;
        cin >> m >> t;
        auto [l, r] = G.find_str(t);
        if (r - l < m) {
            cout << -1 << '\n';
            continue;
        }
        vector<int> all;
        for (int i = l; i < r; ++i)
            all.emplace_back(T.suff[i + 1]);
        sort(all.begin(), all.end());
        int answer = s.size();
        for (int i = 0; i + m - 1 < all.size(); ++i)
            answer = min(answer, all[i + m - 1] - all[i] + (int) t.size());
        cout << answer << '\n';
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
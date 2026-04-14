#include <bits/stdc++.h>

using namespace std;
#define int long long

struct SuffixArray {
    // suff is the suffix array with the empty suffix being suff[0]
    // lcp[i] holds the lcp between sa[i], sa[i - 1]
    // pos is rank of index i in suffix
    int n;
    vector<int> suff, lcp, pos, lg;
    vector<array<int, 21>> table;

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
        preLcp();
    }

    void preLcp() {
        lg.resize(n + 5);
        table.resize(n + 5);
        for (int i = 2; i < n + 5; ++i) lg[i] = lg[i / 2] + 1;
        for (int i = 0; i < n; ++i) table[i][0] = lcp[i];
        for (int j = 1; j <= lg[n]; ++j)
            for (int i = 0; i <= n - (1 << j); ++i)
                table[i][j] = min(table[i][j - 1], table[i + (1 << (j - 1))][j - 1]);
    }

    // pass the pos of the suffixes
    int queryLcp(int i, int j) {
        if (i == j) return n - suff[i] - 1;
        if (i > j) swap(i, j);
        i++;
        int len = lg[j - i + 1];
        return min(table[i][len], table[j - (1 << len) + 1][len]);
    }
};

void solve() {
    int n;
    string s, t;
    cin >> n >> s >> t;
    reverse(s.begin(), s.end());
    reverse(t.begin(), t.end());
    vector<int> prefix(n + 2);

    SuffixArray T(s);
    vector<int> R(n + 1, n + 1), L(n + 1, -1);
    for (int i = 1; i <= n; ++i)
        prefix[i] = prefix[i - 1] + t[T.suff[i]] - '0';
    stack<int> st;
    for (int i = n; i; --i) {
        while (!st.empty() and T.lcp[i] <= T.lcp[st.top()])
            st.pop();
        R[i] = (st.empty() ? n + 1 : st.top());
        st.push(i);
    }
    while (!st.empty())st.pop();
    for (int i = 1; i <= n; ++i) {
        while (!st.empty() and T.lcp[i] <= T.lcp[st.top()])
            st.pop();
        L[i] = (st.empty() ? -1 : st.top());
        st.push(i);
    }

    int answer = 0;
    int idx = 0;
    while (t[idx] == '1')++idx;
    answer = n - idx;
    for (int i = 0; i <= n; ++i) {
        int len = R[i] - L[i];
        int bad = (R[i] ? prefix[R[i] - 1] : 0);
        bad -= (L[i] ? prefix[L[i] - 1] : 0);
        answer = max(answer, (len - bad) * T.lcp[i]);
    }
    cout << answer << '\n';
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
#ifdef HALZOOM
    freopen("Input.txt", "r", stdin);
    freopen("Output.txt", "w", stdout);
#endif

    int test = 1;
////    cin >> test;

    for (int i = 1; i <= test; ++i) {
        solve();
    }
    return 0;
}
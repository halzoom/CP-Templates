#include <bits/stdc++.h>

using namespace std;

#define int long long
#define bint __int128
#define _3bkarm cin.tie(NULL); cout.tie(NULL); ios::sync_with_stdio(false);

const int oo = 2e18;

struct SuffixAutomaton {
    static const int A = 27;

    struct State {
        int len = 0, lnk = -1, cnt = 0, d = 1;
        int firstPos = 0, sum = 0; // if you need
        bool isClone = 0;
        array<int, A> nxt;
        State() { nxt.fill(-1); }
    };

    vector<State> t{{}};
    int lst = 0;

    SuffixAutomaton() {}

    int get(char ch) { return ch == '#'? 26 : ch - 'A'; }

    void insert(int ch) {
        int c = get(ch), me = t.size(), p = lst;
        t.push_back({});
        t[me].len = t[p].len + 1;
        t[me].firstPos = t[me].len - 1;
        t[me].cnt = 1;
        t[me].lnk = 0;
        lst = me;

        while(~p && t[p].nxt[c] == -1) {
            t[p].nxt[c] = me;
            p = t[p].lnk;
        }

        if(p == -1) return;

        int q = t[p].nxt[c];
        if(t[q].len == t[p].len + 1) {
            t[me].lnk = q;
            return;
        }

        int clone = t.size();
        t.push_back(t[q]);

        t[clone].len = t[p].len + 1;
        t[clone].isClone = 1;
        t[clone].cnt = 0;

        while (~p && t[p].nxt[c] == q) {
            t[p].nxt[c] = clone;
            p = t[p].lnk;
        }

        t[q].lnk = t[me].lnk = clone;
    }

    int move(int v, char c) { return ~v? t[v].nxt[get(c)] : -1; }
};

void getShitDone() {
    int n; cin >> n;
    SuffixAutomaton sa;

    sa.insert('#');
    for(int i = 0; i < n; ++i) {
        string s; cin >> s;
        for(char &ch: s) sa.insert(ch);
        sa.insert('#');
    }

    string s; cin >> s;
    n = size(s);

    int sz = size(sa.t);
    vector dp(n + 1, vector<int>(sz, oo));
    deque<array<int, 2>> q;

    auto push = [&](int i, int u, int w, int cst) {
        if(i > n || w + cst >= dp[i][u]) return;
        dp[i][u] = w + cst;
        if(cst) q.push_back({i, u});
        else q.push_front({i, u});
    };

    int st = sa.move(0, '#');

    push(0, st, 0, 0);

    while(!q.empty()) {
        auto [i, u] = q.front();
        q.pop_front();

        push(i + 1, u, dp[i][u], 1);

        int v = sa.move(u, '#');
        if(~v) push(i, st, dp[i][u], 0);

        for(int c = 'A'; c <= 'Z'; ++c) {
            v = sa.move(u, c);
            if(v == -1) continue;
            push(i, v, dp[i][u], 1);
            push(i + 1, v, dp[i][u], (s[i] != c));
        }
    }

    cout << dp[n][1] << '\n';
}

signed main() {
    _3bkarm

    int ts = 1;
//    cin >> ts;
    while (ts--) {
        getShitDone();
        if (ts != 0) cout << '\n';
    }

    return 0;
}
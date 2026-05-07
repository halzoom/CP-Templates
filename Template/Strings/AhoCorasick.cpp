#include <bits/stdc++.h>

using namespace std;
#define int long long

// Patterns must be distinct
struct AhoCorasick {
    int N, P, A, st;
    vector<vector<int>> nxt;
    vector<int> link, out_link, out;
    // nxt -> aho automaton
    // link -> failure link to the longest proper suffix
    // out -> end of pattern (the id of the pattern)
    // out_link -> point to the nearest out node
    AhoCorasick(int A, int st) : N(0), P(0), A(A), st(st) { node(); }

    int node() {
        nxt.emplace_back(A, 0);
        link.emplace_back(0);
        out_link.emplace_back(0);
        out.emplace_back(-1);
        return N++;
    }

    int get(char c) { return c - st; }

    int insert(const string &p) {
        int u = 0;
        for (auto &c: p) {
            if (!nxt[u][get(c)]) nxt[u][get(c)] = node();
            u = nxt[u][get(c)];
        }

        out[u] = P;
        return P++;
    }

    void build() {
        queue<int> q;
        for (q.push(0); !q.empty();) {
            int u = q.front();
            q.pop();
            for (int c = 0; c < A; ++c) {
                int v = nxt[u][c];
                if (!v) nxt[u][c] = nxt[link[u]][c];
                else {
                    link[v] = u ? nxt[link[u]][c] : 0;
                    out_link[v] = ~out[link[v]] ? link[v] : out_link[link[v]];
                    q.push(v);
                }
            }
        }
    }

    int advance(int u, char c) {
        while (u and !nxt[u][get(c)]) u = link[u];
        u = nxt[u][get(c)];
        return u;
    }
};

void solve() {
    string s, t;
    cin >> s;
    int n;
    cin >> n;
    map<string, int> mp;
    vector<int> id(n);
    AhoCorasick T(26, 'a');
    for (int i = 0; i < n; ++i) {
        cin >> t;
        if (mp.contains(t))id[i] = mp[t];
        else
            mp[t] = id[i] = T.insert(t);
    }
    T.build();
    vector<int> answer(T.P);
    int u = 0;
    for (auto c: s) {
        for (int v = T.advance(u, c); v; v = T.out_link[v]) {
            if (T.out[v] != -1)
                answer[T.out[v]]++;
        }
        u = T.advance(u, c);
    }
    for (int i = 0; i < n; ++i)
        cout << (answer[id[i]] ? "YES\n" : "NO\n");
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
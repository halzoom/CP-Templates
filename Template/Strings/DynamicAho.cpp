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

    void reset() {
        N = P = 0;
        nxt.clear();
        out_link.clear();
        link.clear();
        out.clear();
        node();
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

struct DynamicAhoCorasick {
    vector<pair<AhoCorasick, vector<string>>> T;

    void insert(string &s) {
        vector<string> cur = {s};
        while (!T.empty() and cur.size() == T.back().second.size()) {
            for (auto &t: T.back().second)
                cur.emplace_back(t);

            T.pop_back();
        }
        AhoCorasick aho(26, 'a');
        for (auto &t: cur)
            aho.insert(t);

        aho.build();
        T.emplace_back(aho, cur);
    }

    int count(string &s) {
        int answer = 0;
        for (auto &[aho, cur]: T) {
            int u = 0;
            for (auto c: s) {
                u = aho.advance(u, c);
                for (int v = u; v; v = aho.out_link[v])
                    if (aho.out[v] != -1)answer++;
            }
        }
        return answer;
    }
};

void solve() {
    int n;
    cin >> n;
    DynamicAhoCorasick active, bad;
    for (int i = 0, t; i < n; ++i) {
        string s;
        cin >> t >> s;
        if (t == 1)
            active.insert(s);
        else if (t == 2)
            bad.insert(s);
        else
            cout << active.count(s) - bad.count(s) << endl;
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
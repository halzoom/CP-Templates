#include <bits/stdc++.h>

using namespace std;
const int N = 5004, inf = 1e9;

struct Trie {
private:
    struct Node {
        map<char, int> child;
        bool isEnd = false;

        int &operator[](char x) { return child[x]; }
    };

public:
    vector<Node> node;

    Trie() {
        node.clear();
        node.emplace_back();
    };

    int newNode() {
        node.emplace_back();
        return node.size() - 1;
    }

    void update(string &s) {
        int cur = 0;
        for (auto c: s) {
            if (node[cur][c] == 0)
                node[cur][c] = newNode();
            cur = node[cur][c];
        }
        node[cur].isEnd = true;
    }
};

int dist[N][N];

void solve() {
    int n;
    cin >> n;
    Trie trie;
    string s;
    for (int i = 0; i < n; ++i) {
        cin >> s;
        trie.update(s);
    }
    cin >> s;
    deque<array<int, 3>> pq;
    for (int i = 0; i <= s.size(); ++i)
        for (int j = 0; j < N; ++j)
            dist[i][j] = inf;

    pq.push_back({0, 0, 0});
    dist[0][0] = 0;
    while (!pq.empty()) {
        auto [w, idx, cur] = pq.front();
        pq.pop_front();

        if (dist[idx][cur] < w)continue;
        if (idx < s.size() and dist[idx + 1][cur] > w + 1) {
            dist[idx + 1][cur] = w + 1;
            pq.push_back({w + 1, idx + 1, cur});
        }

        if (trie.node[cur].isEnd and dist[idx][0] > w) {
            dist[idx][0] = w;
            pq.push_front({w, idx, 0});
        }
        for (auto [c, nxt]: trie.node[cur].child) {
            if (idx < s.size() and dist[idx + 1][nxt] > w + (c != s[idx])) {
                dist[idx + 1][nxt] = w + (c != s[idx]);
                if (c != s[idx])
                    pq.push_back({w + 1, idx + 1, nxt});
                else
                    pq.push_front({w, idx + 1, nxt});
            }
            if (dist[idx][nxt] > w + 1) {
                dist[idx][nxt] = w + 1;
                pq.push_back({w + 1, idx, nxt});
            }
        }
    }
    cout << dist[s.size()][0] << '\n';
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int test = 1;
//    cin >> test;

    for (int i = 1; i <= test; ++i) {
        solve();
    }
    return 0;
}
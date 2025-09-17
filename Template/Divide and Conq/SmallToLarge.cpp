#include<bits/stdc++.h>
using namespace std;
#define int long long

/*
 * For each pair (i, j) such that i < j. Let M be the maximum number in the
 * array between i and j i.e.M = max(A_i , A_i+1, ..., A_j-1, A_j ). If the xor
 * of A_i and A_j is greater than M, add M to the sum
 * */
struct Trie_B {
private:
    struct Node {
        int child[2]{};
        int cnt = 0, isEnd = 0;

        int &operator[](int x) { return child[x]; }
    };

    vector <Node> node;
public:
    Trie_B() { node.emplace_back(); }

    int newNode() {
        node.emplace_back();
        return node.size() - 1;
    }

    int sz(int x) { return node[x].cnt; }

    int M = 30;

    void update(int x, int op) {  // op -> 1 add || op -> -1 erase
        int cur = 0;
        for (int i = M - 1; i >= 0; --i) {
            int c = x >> i & 1;
            if (node[cur][c] == 0) node[cur][c] = newNode();
            cur = node[cur][c];
            node[cur].cnt += op;
        }
        node[cur].isEnd += op;
    }

    // 17
    int count(int x, int k) {  // number of x ^ a[i] >= k
        int cur = 0, res = 0;
        for (int i = M - 1; i >= 0; --i) {
            int cx = x >> i & 1;
            int ck = k >> i & 1;
            if (!ck) res += sz(node[cur][!cx]);
            cur = node[cur][ck ^ cx];
            if (sz(cur) == 0) break;
        }
        return res;
    }
};

// #include "sparse_table.h"
void solve() {
    int n;
    cin >> n;
    vector <pair<int, int>> a(n + 1);
    for (int i = 1; i <= n; ++i) cin >> a[i].first, a[i].second = i;
    SparseTable sp(a, [&](pair<int, int> x, pair<int, int> y) { return max(x, y); });
    int answer = 0;
    // 24
    function < Trie_B(int, int) > go = [&](int l, int r) -> Trie_B {
        if (l > r) { return {}; }
        auto [M, idx] = sp.get(l, r);
        Trie_B trieL = go(l, idx - 1);
        Trie_B trieR = go(idx + 1, r);
        int Lsize = idx - l, Rsize = r - idx;
        if (Lsize > Rsize) {
            trieL.update(M, 1);
            for (int i = idx; i <= r; ++i) { answer += trieL.count(a[i].first, M) * M; }
        } else {
            trieR.update(M, 1);
            for (int i = l; i <= idx; ++i) { answer += trieR.count(a[i].first, M) * M; }
        }
        if (Rsize >= Lsize) {
            for (int i = l; i < idx; ++i) trieR.update(a[i].first, 1);
            return trieR;
        } else {
            for (int i = idx + 1; i <= r; ++i) trieL.update(a[i].first, 1);
            return trieL;
        }
    };
    go(1, n);
    cout << answer << '\n';
}
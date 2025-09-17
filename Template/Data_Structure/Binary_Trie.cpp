#include<bits/stdc++.h>
using namespace std;
#define int long long

// The Kth smallest subarray of length >= 2
// the value of a subarray is the minimum a[i] ^ a[j]
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

    int min_xor(int x) {  // min xor with x
        int cur = 0, res = 0;
        for (int i = M - 1; i >= 0; --i) {
            int cx = x >> i & 1;
            if (sz(node[cur][cx])) {
                cur = node[cur][cx];
            } else {
                res += 1LL << i;
                cur = node[cur][!cx];
            }
        }
        return res;
    }

    int max_xor(int x) {
        int cur = 0, res = 0;
        for (int i = M - 1; i >= 0; --i) {
            int cx = (x >> i) & 1;
            if (sz(node[cur][!cx])) {
                res |= (1LL << i);
                cur = node[cur][!cx];
            } else {
                cur = node[cur][cx];
            }
        }
        return res;
    }

    int count(int x, int k) {  // number of x ^ a[i] >= k
        int cur = 0, res = 0;
        for (int i = M - 1; i >= 0; --i) {
            int cx = x >> i & 1;
            int ck = k >> i & 1;
            if (!ck) res += sz(node[cur][!cx]);
            cur = node[cur][ck ^ cx];
            if (sz(cur) == 0) break;
        }
        return res + node[cur].cnt;
    }
};

signed main() {
    int n, k;
    cin >> n >> k;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    auto can = [&](int x) {
        int l = 0, r = 0, cnt_pairs = 0, res = 0;
        Trie_B trie;
        while (r < n) {
            cnt_pairs += trie.count(a[r], x);
            trie.update(a[r++], 1);
            while (cnt_pairs > 0) {
                trie.update(a[l], -1);
                cnt_pairs -= trie.count(a[l++], x);
            }
            res += l;
        }
        return res >= k;
    };
    int l = 0, r = 4e9, mid, ans = 0;
    while (l <= r) {
        mid = (l + r) / 2;
        if (can(mid)) {
            ans = mid;
            r = mid - 1;
        } else {
            l = mid + 1;
        }
    }
    cout << ans << '\n';
    return 0;
}

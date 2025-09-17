#include<bits/stdc++.h>
using namespace std;
#define int long long

struct Trie {
private:
    struct Node {
        map<char, int> child;
        int cnt = 0, isEnd = 0;

        int &operator[](char x) { return child[x]; }
    };

    vector <Node> tree;
public:
    Trie() { tree.emplace_back(); }

    int newNode() {
        tree.emplace_back();
        return (int) tree.size() - 1;
    }

    int sz(int x) { return tree[x].cnt; }

    void update(string &s, int op) {  // op -> 1 add || op -> -1 erase
        int cur = 0;
        for (auto &c: s) {
            if (tree[cur][c] == 0) tree[cur][c] = newNode();
            cur = tree[cur][c];
            tree[cur].cnt += op;
        }
        tree[cur].isEnd += op;
    }

    int count_prefix(string &s) {  // count strings that share a prefix s
        int cur = 0;
        for (auto &c: s) {
            if (tree[cur][c] == 0) return 0;
            cur = tree[cur][c];
        }
        return tree[cur].cnt;
    }

    int LCP(string &s) {  // longest common prefix
        int cur = 0, res = 0;
        for (auto &i: s) {
            if (sz(tree[cur][i]) == 0) { return res; }
            cur = tree[cur][i];
            res++;
        }
        return res;
    }

    string min_string(string &s) {  // return the smallest string have a prefix s
        string t = s;
        int cur = 0;
        for (auto &c: s) {
            if (tree[cur][c] == 0) return "-1";
            cur = tree[cur][c];
        }
        while (tree[cur].isEnd == 0) {
            for (char c = 'a'; c <= 'z'; ++c) {
                if (tree[cur][c] != 0) {
                    t += c;
                    cur = tree[cur][c];
                    break;
                }
            }
        }
        return t;
    }
};

signed main() {
    int n;
    cin >> n;
    vector <string> s(n);
    Trie trie;
    for (int i = 0; i < n; ++i) {
        cin >> s[i];
        trie.update(s[i], 1);
    }
    for (int i = 0; i < n; ++i) {
        trie.update(s[i], -1);
        cout << trie.LCP(s[i]) << '\n';
        trie.update(s[i], 1);
    }
}
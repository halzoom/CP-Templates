#include<bits/stdc++.h>
using namespace std;
#define int long long

struct Node {
    int len;
    int suffLink;
    map<char, int> next;
    int numOccur;
    int firstPos;

    Node(int l, int link) : len(l), suffLink(link), numOccur(0), firstPos(-1) {}
};

class PalindromicTree {
public:
    vector <Node> tree;
    string s;
    int suff;

    PalindromicTree(const string &str) : s(str) {
        tree.emplace_back(-1, 0);
        tree.emplace_back(0, 0);
        tree[0].suffLink = 0;
        suff = 1;
        build();
    }

    void build() {
        for (int i = 0; i < s.length(); ++i) addChar(i);
    }

    void addChar(int pos) {
        int cur = suff;
        char ch = s[pos];
        while (true) {
            int curlen = tree[cur].len;
            if (pos - curlen - 1 >= 0 && s[pos - curlen - 1] == ch) break;
            cur = tree[cur].suffLink;
        }
        if (tree[cur].next.count(ch)) {
            suff = tree[cur].next[ch];
            tree[suff].numOccur++;
            return;
        }
        int newNode = tree.size();
        tree.emplace_back(tree[cur].len + 2, 0);
        tree[newNode].firstPos = pos;
        tree[cur].next[ch] = newNode;
        if (tree[newNode].len == 1) {
            tree[newNode].suffLink = 1;
        } else {
            int temp = tree[cur].suffLink;
            while (true) {
                int templen = tree[temp].len;
                if (pos - templen - 1 >= 0 && s[pos - templen - 1] == ch) break;
                temp = tree[temp].suffLink;
            }
            tree[newNode].suffLink = tree[temp].next[ch];
        }
        tree[newNode].numOccur = 1;
        suff = newNode;
    }

    int countDistinctPalindromes() { return (int) tree.size() - 2; }

    vector<int> minPalindromePartitions(const string &s) {
        int n = s.size();
        vector<int> dp(n, INT_MAX);
        dp[0] = 0;
        for (int i = 0; i < n; ++i) {
            addChar(i);
            int cur = suff;
            while (cur > 1) {  // nodes 0 and 1 are roots
                int len = tree[cur].len;
                int start = i - len + 1;
                int prev = start - 1;
                dp[i] = min(dp[i], (prev >= 0 ? dp[prev] : 0) + 1);
                cur = tree[cur].suffLink;
            }
        }
        return dp;
    }

    void countOccurrences() {
        vector<int> order(tree.size());
        for (int i = 0; i < order.size(); ++i) order[i] = i;
        sort(order.begin(), order.end(),
             [&](int a, int b) { return tree[a].len > tree[b].len; });
        for (int i: order) {
            int link = tree[i].suffLink;
            if (i != link) tree[link].numOccur += tree[i].numOccur;
        }
    }

    void printAllPalindromes() {
        for (int i = 2; i < tree.size(); ++i) {
            int start = tree[i].firstPos - tree[i].len + 1;
            cout << s.substr(start, tree[i].len) << " -> " << tree[i].numOccur << '\n';
        }
    }

    vector <string> getPalindromes() {
        vector <string> result;
        for (int i = 2; i < tree.size(); ++i) {
            int start = tree[i].firstPos - tree[i].len + 1;
            result.push_back(s.substr(start, tree[i].len));
        }
        return result;
    }
};

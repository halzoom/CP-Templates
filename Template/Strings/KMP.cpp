#include<bits/stdc++.h>
using namespace std;
#define int long long

struct KMP {
    vector<int> pi;
    string pattern;
    int n = 0;

    KMP(string &pat) {
        pattern = pat;
        n = (int) pat.size();
        pi.assign(n + 1, {});
        for (int i = 1; i < n; ++i) {  // O(n)
            int k = pi[i - 1];
            while (k > 0 and pat[k] != pat[i]) k = pi[k - 1];
            k += pat[i] == pat[k];
            pi[i] = k;
        }
    }

    // find all occurrence of pattern in string s
    vector<int> find(string &s) {
        int k = 0;
        vector<int> ret;
        for (int i = 0; i < s.size(); ++i) {
            while (k and pattern[k] != s[i]) k = pi[k - 1];
            k += s[i] == pattern[k];
            if (k == pattern.size()) ret.push_back(i - k + 1);
        }
        return ret;
    }

    // count the number of appearances of each prefix
    vector<int> count() {
        vector<int> ans(n + 1);
        for (int i = 0; i < n; i++) ans[pi[i]]++;
        for (int i = n - 1; i > 0; i--) ans[pi[i - 1]] += ans[i];
        for (int i = 0; i < n; i++) ans[i]++;
        return ans;
    }

    // find all periods of a string
    // A period of a string is a prefix that can be used to
    // generate the whole string by repeating the prefix
    // if we want a full period the n % (n - k) == 0
    vector<int> period() {
        vector<int> ans;
        int k = pi[n - 1];
        while (k) {
            ans.push_back(n - k);
            k = pi[k - 1];
        }
        ans.push_back(n);
        return ans;
    }

    vector <vector<int>> compute_automaton(string s) {
        s += '#';
        vector <vector<int>> aut(s.size(), vector<int>(26));
        for (int i = 0; i < s.size(); ++i) {
            for (int c = 0; c < 26; ++c) {
                if (i and c + 'a' != s[i]) aut[i][c] = aut[pi[i - 1]][c];
                else aut[i][c] = i + (c + 'a' == s[i]);
            }
        }
        return aut;
    }
};

vector<int> computePrefix(string &s) {
    vector<int> longestPrefix(s.size());
    for (int i = 1; i < s.size(); ++i) {
        int k = longestPrefix[i - 1];
        while (k and s[k] != s[i]) k = longestPrefix[k - 1];
        k += s[k] == s[i];
        longestPrefix[i] = k;
    }
    return longestPrefix;
}
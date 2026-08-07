#include <bits/stdc++.h>
using namespace std;
#define int long long
// 1-indexed
struct Manacher {
    string s;
    vector<int32_t> odd, even;
    Manacher() {}
    Manacher(const string &str) { build(str); }
    // Builds odd/even palindrome radii in O(n).
    void build(const string &str) {
        s = " " + str;
        int n = str.size();
        odd.assign(n + 1, 0);
        even.assign(n + 1, 0);
        for (int i = 1, l = 1, r = 0; i <= n; ++i) {
            int k = i > r ? 1 : min<int>(odd[l + r - i], r - i + 1);
            while (i - k >= 1 && i + k <= n && s[i - k] == s[i + k]) ++k;
            odd[i] = k;
            if (i + k - 1 > r) l = i - k + 1, r = i + k - 1;
        }
        for (int i = 1, l = 1, r = 0; i <= n; ++i) {
            int k = i > r ? 0 : min<int>(even[l + r - i + 1], r - i + 1);
            while (i - k - 1 >= 1 && i + k <= n && s[i - k - 1] == s[i + k]) ++k;
            even[i] = k;
            if (i + k - 1 > r) l = i - k, r = i + k - 1;
        }
    }

    // Checks whether s[l..r] is a palindrome in O(1).
    bool isPalindrome(int l, int r) const {
        int len = r - l + 1;
        if (len & 1) return odd[(l + r) / 2] >= len / 2 + 1;
        return even[l + len / 2] >= len / 2;
    }
};
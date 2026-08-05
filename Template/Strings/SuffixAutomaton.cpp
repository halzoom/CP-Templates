#include <bits/stdc++.h>

using namespace std;
#define int long long

struct SuffixAutomaton {
    static const int A = 26;

    struct State {
        array<int, A> nxt;
        int lnk = -1, len = 0, pos = 0, cnt = 0;
        bool clone = false;
        vector<int> inv;

        State() {
            nxt.fill(-1);
        }
    };

    vector<State> t;
    string s;
    int lst = 0;

    SuffixAutomaton(const string &_s = "", bool occ = true, bool tree = true) {
        build(_s, occ, tree);
    }

    int id(char c) const {
        return c - 'a';
    }

    // Clears the old automaton and rebuilds it for the given string in O(n).
    void build(const string &_s, bool occ = 1, bool tree = 1) {
        s = _s;
        t.clear();
        t.reserve(max(1ll, 2 * (int) s.size()));
        t.emplace_back();
        lst = 0;

        for (auto c: s)
            insert(c);

        if (occ) buildCount();
        if (tree) buildTree();
    }

    // Adds one character to the end in amortized O(1), invalidating old counts and tree.
    void insert(char ch) {
        int c = id(ch), cur = t.size(), p = lst;
        assert(0 <= c and c < A);
        t.emplace_back();
        t[cur].len = t[lst].len + 1;
        t[cur].pos = t[cur].len - 1;
        t[cur].cnt = 1;

        while (~p and t[p].nxt[c] == -1) {
            t[p].nxt[c] = cur;
            p = t[p].lnk;
        }

        if (p == -1) {
            t[cur].lnk = 0;
        } else {
            int q = t[p].nxt[c];
            if (t[p].len + 1 == t[q].len) {
                t[cur].lnk = q;
            } else {
                int cl = t.size();
                t.push_back(t[q]);
                t[cl].len = t[p].len + 1;
                t[cl].clone = 1;
                t[cl].cnt = 0;
                while (~p and t[p].nxt[c] == q) {
                    t[p].nxt[c] = cl;
                    p = t[p].lnk;
                }
                t[q].lnk = t[cur].lnk = cl;
            }
        }
        lst = cur;
    }

    // Returns all states sorted by decreasing maximum length in O(states + n).
    vector<int> order() const {
        int mx = 0, sz = t.size();
        for (auto &v: t)
            mx = max(mx, v.len);

        vector<int> cnt(mx + 1), ord(sz);
        for (auto &v: t)
            cnt[v.len]++;

        for (int i = 1; i <= mx; ++i)
            cnt[i] += cnt[i - 1];

        for (int i = sz - 1; i >= 0; --i)
            ord[--cnt[t[i].len]] = i;

        reverse(ord.begin(), ord.end());
        return ord;
    }

    // Computes how many times every state/substrings represented by it occur in O(states + n).
    void buildCount() {
        for (int i = 0; i < t.size(); ++i)
            t[i].cnt = i and !t[i].clone;

        for (auto v: order())
            if (~t[v].lnk)
                t[t[v].lnk].cnt += t[v].cnt;
    }

    // Builds the suffix-link tree using reverse suffix-link edges in O(states).
    void buildTree() {
        for (auto &v: t)
            v.inv.clear();

        for (int i = 1; i < t.size(); ++i)
            t[t[i].lnk].inv.push_back(i);
    }

    // Walks through string p and returns its final state, or -1 if absent, in O(|p|).
    int move(const string &p) const {
        int v = 0;
        for (auto c: p) {
            int x = id(c);
            if (x < 0 or x >= A or t[v].nxt[x] == -1)
                return -1;
            v = t[v].nxt[x];
        }
        return v;
    }

    // Checks whether p is a substring of the original string in O(|p|).
    bool contains(const string &p) const {
        return move(p) != -1;
    }

    // Counts all distinct non-empty substrings in O(states).
    int distinct() const {
        int ans = 0;
        for (int i = 1; i < t.size(); ++i)
            ans += t[i].len - t[t[i].lnk].len;
        return ans;
    }

    // Returns the sum of lengths of all distinct substrings in O(states).
    int distinctLengthSum() const {
        int ans = 0;
        for (int i = 1; i < t.size(); ++i) {
            int l = t[t[i].lnk].len + 1;
            int r = t[i].len;
            ans += (r - l + 1) * (l + r) / 2;
        }
        return ans;
    }

    // Returns ans[len] = number of distinct substrings having length len in O(states + n).
    vector<int> distinctByLength() const {
        int n = s.size();
        vector<int> diff(n + 2), ans(n + 1);
        for (int i = 1; i < t.size(); ++i) {
            int l = t[t[i].lnk].len + 1;
            int r = t[i].len;
            diff[l]++;
            diff[r + 1]--;
        }
        for (int i = 1; i <= n; ++i) {
            diff[i] += diff[i - 1];
            ans[i] = diff[i];
        }
        return ans;
    }

    // Returns ans[cnt] = number of distinct substrings occurring exactly cnt times in O(states + n); requires buildCount.
    vector<int> distinctByOccurrences() const {
        vector<int> ans(s.size() + 1);
        for (int i = 1; i < t.size(); ++i)
            ans[t[i].cnt] += t[i].len - t[t[i].lnk].len;
        return ans;
    }

    // Counts distinct substrings occurring exactly k times in O(states); requires buildCount.
    int countExactly(int k) const {
        int ans = 0;
        for (int i = 1; i < t.size(); ++i)
            if (t[i].cnt == k)
                ans += t[i].len - t[t[i].lnk].len;
        return ans;
    }

    // Counts distinct substrings occurring at least k times in O(states); requires buildCount.
    int countAtLeast(int k) const {
        int ans = 0;
        for (int i = 1; i < t.size(); ++i)
            if (t[i].cnt >= k)
                ans += t[i].len - t[t[i].lnk].len;
        return ans;
    }

    // Returns the k-th distinct substring lexicographically in O(states + n + A * states + A * answer length).
    string kth(int k) const {
        const int inf = 4e18;
        vector<int> dp(t.size());
        for (auto v: order()) {
            for (int c = 0; c < A; ++c) {
                int u = t[v].nxt[c];
                if (u == -1)continue;
                dp[v] = min(inf, dp[v] + 1 + dp[u]);
            }
        }
        if (k <= 0 or k > dp[0])return "";

        string ans;
        int v = 0;
        while (k) {
            for (int c = 0; c < A; ++c) {
                int u = t[v].nxt[c];
                if (u == -1)continue;
                int block = 1 + dp[u];
                if (k <= block) {
                    ans += char('a' + c);
                    k--;
                    if (!k)return ans;
                    v = u;
                    break;
                }
                k -= block;
            }
        }
        return ans;
    }

    // Returns the k-th substring with repetitions in O(states + n + A * states + A * answer length); requires buildCount.
    string kthAll(int k) const {
        const int inf = 4e18;
        vector<int> dp(t.size());
        for (auto v: order()) {
            for (int c = 0; c < 26; ++c) {
                int u = t[v].nxt[c];
                if (u == -1)continue;
                dp[v] += t[u].cnt + dp[u];
                dp[v] = min(dp[v], inf);
            }
        }
        if (k <= 0 or k > dp[0])
            return "";

        string ans;
        int v = 0;
        while (true) {
            for (int c = 0; c < 26; ++c) {
                int u = t[v].nxt[c];
                if (u == -1)continue;
                int block = t[u].cnt + dp[u];
                if (k > block) {
                    k -= block;
                    continue;
                }
                ans += char('a' + c);
                if (k <= t[u].cnt)
                    return ans;
                k -= t[u].cnt;
                v = u;
                break;
            }
        }
    }

    // Returns the number of occurrences of p in O(|p|); requires buildCount.
    int occurrences(const string &p) const {
        if (p.empty())return s.size() + 1;
        int v = move(p);
        return v == -1 ? 0 : t[v].cnt;
    }

    // Returns one occurrence start position of p, or -1 if absent, in O(|p|).
    int firstOccurrence(const string &p) const {
        if (p.empty())return 0;
        int v = move(p);
        if (v == -1)return -1;
        return t[v].pos - p.size() + 1;
    }

    // Collects occurrence positions from the suffix-link subtree in O(subtree size).
    void collect(int v, int len, vector<int> &ans) const {
        vector<int> st{v};
        while (!st.empty()) {
            int x = st.back();
            st.pop_back();
            if (!t[x].clone)
                ans.push_back(t[x].pos - len + 1);
            for (auto u: t[x].inv)
                st.push_back(u);
        }
    }

    // Returns all sorted positions of p in O(|p| + subtree size + occurrences log occurrences); requires buildTree.
    vector<int> allOccurrences(const string &p) const {
        if (p.empty()) {
            vector<int> ans(s.size() + 1);
            iota(ans.begin(), ans.end(), 0);
            return ans;
        }
        int v = move(p);
        if (v == -1)return {};
        vector<int> ans;
        collect(v, p.size(), ans);
        sort(ans.begin(), ans.end());
        return ans;
    }

    // Returns the longest substring occurring at least k times in O(states); requires buildCount.
    string longestAtLeast(int k) const {
        int best = 0, pos = -1;
        for (int i = 1; i < t.size(); ++i) {
            if (t[i].cnt >= k and t[i].len > best) {
                best = t[i].len;
                pos = t[i].pos;
            }
        }
        if (!best)return "";
        return s.substr(pos - best + 1, best);
    }

    // Returns the longest substring occurring exactly k times in O(states); requires buildCount.
    string longestExactly(int k) const {
        int best = 0, pos = -1;
        for (int i = 1; i < t.size(); ++i) {
            if (t[i].cnt == k and t[i].len > best) {
                best = t[i].len;
                pos = t[i].pos;
            }
        }
        if (!best)return "";
        return s.substr(pos - best + 1, best);
    }

    // Returns the shortest substring occurring exactly once in O(states); requires buildCount.
    string shortestUnique() const {
        int best = 1e18, state = -1;
        for (int i = 1; i < t.size(); ++i) {
            int len = t[t[i].lnk].len + 1;
            if (t[i].cnt == 1 and len < best) {
                best = len;
                state = i;
            }
        }
        if (state == -1)return "";
        return s.substr(t[state].pos - best + 1, best);
    }

    // Returns the longest substring occurring at least twice in O(states); requires buildCount.
    string longestRepeated() const {
        return longestAtLeast(2);
    }

    // Returns max(length * occurrences) in O(states); requires buildCount.
    int maxRepeatValue() const {
        int ans = 0;
        for (int i = 1; i < t.size(); ++i)
            ans = max(ans, t[i].len * t[i].cnt);
        return ans;
    }

    // Returns the maximum occurrence count for every length in O((states + n) log states); requires buildCount.
    vector<int> maxOccurrencesByLength() const {
        int n = s.size();
        vector<vector<pair<int, int>>> add(n + 2);
        vector<int> ans(n + 1);
        for (int i = 1; i < t.size(); ++i) {
            int l = t[t[i].lnk].len + 1;
            int r = t[i].len;
            add[l].emplace_back(r, t[i].cnt);
        }

        priority_queue<pair<int, int>> pq;
        for (int len = 1; len <= n; ++len) {
            for (auto [r, cnt]: add[len])
                pq.emplace(cnt, r);
            while (!pq.empty() and pq.top().second < len)
                pq.pop();
            if (!pq.empty())
                ans[len] = pq.top().first;
        }

        return ans;
    }

    // Returns the longest substring with two non-overlapping occurrences in O(states + n).
    string longestNonOverlapping() const {
        const int inf = 1e18;
        int sz = t.size();
        vector<int> mn(sz, inf), mx(sz, -inf);
        for (int i = 1; i < sz; ++i) {
            if (!t[i].clone)
                mn[i] = mx[i] = t[i].pos;
        }

        for (auto v: order()) {
            if (~t[v].lnk) {
                int p = t[v].lnk;
                mn[p] = min(mn[p], mn[v]);
                mx[p] = max(mx[p], mx[v]);
            }
        }
        int best = 0, state = -1;
        for (int i = 1; i < sz; ++i) {
            int can = min(t[i].len, mx[i] - mn[i]);
            if (can > best) {
                best = can;
                state = i;
            }
        }
        if (!best)return "";
        return s.substr(t[state].pos - best + 1, best);
    }

    // Returns the shortest absent string over the first k letters in O(states + n + A * states + A * answer length).
    string shortestAbsent(int k) const {
        assert(1 <= k and k <= A);
        vector<int> dp(t.size(), 1e9);
        for (auto v: order()) {
            for (int c = 0; c < k; ++c) {
                int u = t[v].nxt[c];
                if (u == -1) {
                    dp[v] = 1;
                    break;
                }
                dp[v] = min(dp[v], 1 + dp[u]);
            }
        }

        string ans;
        int v = 0;
        while (1) {
            if (dp[v] == 1) {
                for (int c = 0; c < k; ++c) {
                    if (t[v].nxt[c] == -1) {
                        ans += char('a' + c);
                        return ans;
                    }
                }
            }

            for (int c = 0; c < k; ++c) {
                int u = t[v].nxt[c];
                if (~u and dp[v] == 1 + dp[u]) {
                    ans += char('a' + c);
                    v = u;
                    break;
                }
            }
        }
    }

    // Returns the lexicographically smallest cyclic shift of s in O(A * n).
    static string minCyclicShift(const string &s) {
        SuffixAutomaton sa(s + s, 0, 0);
        string ans;
        int v = 0;
        for (int i = 0; i < s.size(); ++i) {
            for (int c = 0; c < A; ++c) {
                int u = sa.t[v].nxt[c];
                if (~u) {
                    ans += char('a' + c);
                    v = u;
                    break;
                }
            }
        }
        return ans;
    }

    // Returns ans[i] = longest substring of s ending at p[i] in O(|p|).
    vector<int> matchLengths(const string &p) const {
        vector<int> ans(p.size());
        int v = 0, len = 0;
        for (int i = 0; i < p.size(); ++i) {
            int c = p[i] - 'a';
            while (v and (c < 0 or c >= A or t[v].nxt[c] == -1)) {
                v = t[v].lnk;
                len = t[v].len;
            }
            if (c >= 0 and c < A and ~t[v].nxt[c]) {
                v = t[v].nxt[c];
                len++;
            } else {
                v = len = 0;
            }
            ans[i] = len;
        }
        return ans;
    }

    // Returns the longest common substring of a and b in O(|a| + |b|).
    static string lcs(const string &a, const string &b) {
        SuffixAutomaton sa(a, 0, 0);
        int v = 0, len = 0, best = 0, pos = 0;
        for (int i = 0; i < b.size(); ++i) {
            int c = b[i] - 'a';
            while (v and (c < 0 or c >= A or sa.t[v].nxt[c] == -1)) {
                v = sa.t[v].lnk;
                len = sa.t[v].len;
            }
            if (c >= 0 and c < A and ~sa.t[v].nxt[c]) {
                v = sa.t[v].nxt[c];
                len++;
            } else {
                v = len = 0;
            }
            if (len > best) {
                best = len;
                pos = i;
            }
        }
        if (!best)return "";
        return b.substr(pos - best + 1, best);
    }

    // Returns the LCS and its starting positions in a and b in O(|a| + |b|).
    static tuple<string, int, int> lcsWithPos(const string &a, const string &b) {
        SuffixAutomaton sa(a, 0, 0);
        int v = 0, len = 0, best = 0, pa = -1, pb = -1;
        for (int i = 0; i < b.size(); ++i) {
            int c = b[i] - 'a';
            while (v and (c < 0 or c >= A or sa.t[v].nxt[c] == -1)) {
                v = sa.t[v].lnk;
                len = sa.t[v].len;
            }
            if (c >= 0 and c < A and ~sa.t[v].nxt[c]) {
                v = sa.t[v].nxt[c];
                len++;
            } else {
                v = len = 0;
            }
            if (len > best) {
                best = len;
                pb = i - best + 1;
                pa = sa.t[v].pos - best + 1;
            }
        }
        if (!best)return {"", -1, -1};
        return {b.substr(pb, best), pa, pb};
    }

    // Counts distinct substrings common to a and b in O(|a| + |b|).
    static int commonDistinct(const string &a, const string &b) {
        SuffixAutomaton sa(a, 0, 0);
        vector<int> best(sa.t.size());
        int v = 0, len = 0;
        for (auto ch: b) {
            int c = ch - 'a';
            while (v and (c < 0 or c >= A or sa.t[v].nxt[c] == -1)) {
                v = sa.t[v].lnk;
                len = sa.t[v].len;
            }
            if (c >= 0 and c < A and ~sa.t[v].nxt[c]) {
                v = sa.t[v].nxt[c];
                len++;
            } else {
                v = len = 0;
            }
            best[v] = max(best[v], len);
        }

        for (auto x: sa.order()) {
            if (~sa.t[x].lnk) {
                int p = sa.t[x].lnk;
                best[p] = max(best[p], min(best[x], sa.t[p].len));
            }
        }

        int ans = 0;
        for (int i = 1; i < sa.t.size(); ++i)
            ans += max<int>(0, best[i] - sa.t[sa.t[i].lnk].len);

        return ans;
    }

    // Returns the longest common substring of all strings in O(total length + strings count * states).
    static string lcsMany(vector<string> a) {
        if (a.empty())return "";
        int id = 0;
        for (int i = 1; i < a.size(); ++i)
            if (a[i].size() < a[id].size())
                id = i;

        swap(a[0], a[id]);
        SuffixAutomaton sa(a[0], 0, 0);
        vector<int> common(sa.t.size());
        auto ord = sa.order();
        for (int i = 0; i < sa.t.size(); ++i)
            common[i] = sa.t[i].len;
        for (int j = 1; j < a.size(); ++j) {
            vector<int> cur(sa.t.size());
            int v = 0, len = 0;
            for (auto ch: a[j]) {
                int c = ch - 'a';
                while (v and (c < 0 or c >= A or sa.t[v].nxt[c] == -1)) {
                    v = sa.t[v].lnk;
                    len = sa.t[v].len;
                }
                if (c >= 0 and c < A and ~sa.t[v].nxt[c]) {
                    v = sa.t[v].nxt[c];
                    len++;
                } else {
                    v = len = 0;
                }
                cur[v] = max(cur[v], len);
            }
            for (auto x: ord) {
                if (~sa.t[x].lnk) {
                    int p = sa.t[x].lnk;
                    cur[p] = max(cur[p], min(cur[x], sa.t[p].len));
                }
            }
            for (int i = 0; i < sa.t.size(); ++i)
                common[i] = min(common[i], cur[i]);
        }
        int best = 0, state = 0;
        for (int i = 1; i < sa.t.size(); ++i) {
            if (common[i] > best) {
                best = common[i];
                state = i;
            }
        }
        if (!best)return "";
        return a[0].substr(sa.t[state].pos - best + 1, best);
    }

    // Counts distinct substrings common to all strings in O(total length + strings count * states).
    static int commonMany(vector<string> a) {
        if (a.empty())return 0;
        int id = 0;
        for (int i = 1; i < a.size(); ++i)
            if (a[i].size() < a[id].size())
                id = i;

        swap(a[0], a[id]);
        SuffixAutomaton sa(a[0], 0, 0);
        vector<int> common(sa.t.size());
        auto ord = sa.order();
        for (int i = 0; i < sa.t.size(); ++i)
            common[i] = sa.t[i].len;
        for (int j = 1; j < a.size(); ++j) {
            vector<int> cur(sa.t.size());
            int v = 0, len = 0;
            for (auto ch: a[j]) {
                int c = ch - 'a';
                while (v and (c < 0 or c >= A or sa.t[v].nxt[c] == -1)) {
                    v = sa.t[v].lnk;
                    len = sa.t[v].len;
                }
                if (c >= 0 and c < A and ~sa.t[v].nxt[c]) {
                    v = sa.t[v].nxt[c];
                    len++;
                } else {
                    v = len = 0;
                }
                cur[v] = max(cur[v], len);
            }
            for (auto x: ord) {
                if (~sa.t[x].lnk) {
                    int p = sa.t[x].lnk;
                    cur[p] = max(cur[p], min(cur[x], sa.t[p].len));
                }
            }
            for (int i = 0; i < sa.t.size(); ++i)
                common[i] = min(common[i], cur[i]);
        }
        int ans = 0;
        for (int i = 1; i < sa.t.size(); ++i)
            ans += max<int>(0, common[i] - sa.t[sa.t[i].lnk].len);
        return ans;
    }
};

void solve() {

}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
#ifdef HALZOOM
    freopen("Input.txt", "r", stdin);
    freopen("Output.txt", "w", stdout);
#endif

    int test = 1;
    cin >> test;

    for (int i = 1; i <= test; ++i) {
        solve();
    }
    return 0;
}
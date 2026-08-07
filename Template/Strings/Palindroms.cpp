#include <bits/stdc++.h>

using namespace std;
#define int long long

#include "manacher.cpp"

struct PalRangeLongest {
    enum Type {
        ODD, EVEN, ODD_MINUS, EVEN_MINUS, ODD_PLUS, EVEN_PLUS
    };
    int32_t n = 0, sz = 1;
    vector<array<int32_t, 6>> seg;

    PalRangeLongest() {}

    PalRangeLongest(const Manacher &m) { build(m); }

    void build(const Manacher &m) {
        n = (int) m.odd.size() - 1;
        sz = 1;
        while (sz < n) sz <<= 1;
        seg.assign(2 * sz, {});
        for (int i = 1; i <= n; ++i) {
            auto &x = seg[sz + i - 1];
            x[ODD] = m.odd[i];
            x[EVEN] = m.even[i];
            x[ODD_MINUS] = m.odd[i] - i;
            x[EVEN_MINUS] = m.even[i] - i;
            x[ODD_PLUS] = m.odd[i] + i;
            x[EVEN_PLUS] = m.even[i] + i;
        }
        for (int i = sz - 1; i; --i)
            for (int t = 0; t < 6; ++t)
                seg[i][t] = max(seg[i << 1][t], seg[i << 1 | 1][t]);
    }

    // Returns the maximum stored value of type on [l,r] in O(log n).
    int query(int type, int l, int r) const {
        if (l > r) return LLONG_MIN;
        int ans = LLONG_MIN;
        for (l += sz - 1, r += sz - 1; l <= r; l >>= 1, r >>= 1) {
            if (l & 1) ans = max<int>(ans, seg[l++][type]);
            if (!(r & 1)) ans = max<int>(ans, seg[r--][type]);
        }
        return ans;
    }

    // Returns the first index in [l,r] with stored value >= x, or -1, in O(log n).
    int firstAtLeast(int type, int l, int r, int x) const {
        if (l > r || query(type, l, r) < x) return -1;
        return firstAtLeast(1, 1, sz, type, l, r, x);
    }

    // Returns the last index in [l,r] with stored value >= x, or -1, in O(log n).
    int lastAtLeast(int type, int l, int r, int x) const {
        if (l > r || query(type, l, r) < x) return -1;
        return lastAtLeast(1, 1, sz, type, l, r, x);
    }

    // Checks whether [l,r] contains a palindrome of length at least len in O(log n).
    bool hasAtLeast(int l, int r, int len) const {
        if (len <= 0) return true;
        if (len > r - l + 1) return false;
        int k = (len + 2) / 2;
        int L = l + k - 1, R = r - k + 1;
        if (L <= R && query(ODD, L, R) >= k) return true;
        k = (len + 1) / 2;
        L = l + k, R = r - k + 1;
        return L <= R && query(EVEN, L, R) >= k;
    }

    // Returns {start,length} of a longest palindrome fully inside [l,r] in O(log^2 n).
    pair<int, int> longest(int l, int r) const {
        int lo = 1, hi = r - l + 1, best = 1;
        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            if (hasAtLeast(l, r, mid)) best = mid, lo = mid + 1;
            else hi = mid - 1;
        }
        if (best & 1) {
            int k = (best + 1) / 2;
            int c = firstAtLeast(ODD, l + k - 1, r - k + 1, k);
            return {c - k + 1, best};
        }
        int k = best / 2;
        int c = firstAtLeast(EVEN, l + k, r - k + 1, k);
        return {c - k, best};
    }

    // Returns {l,length} of the longest palindromic prefix of s[l..r] in O(log n).
    pair<int, int> longestPrefix(int l, int r) const {
        int best = 1;
        int c = lastAtLeast(ODD_MINUS, l, (l + r) / 2, 1 - l);
        if (c != -1) best = max(best, 2 * (c - l) + 1);
        c = lastAtLeast(EVEN_MINUS, l + 1, (l + r + 1) / 2, -l);
        if (c != -1) best = max(best, 2 * (c - l));
        return {l, best};
    }

    // Returns {start,length} of the longest palindromic suffix of s[l..r] in O(log n).
    pair<int, int> longestSuffix(int l, int r) const {
        int best = 1, start = r;
        int c = firstAtLeast(ODD_PLUS, (l + r + 1) / 2, r, r + 1);
        if (c != -1) {
            int len = 2 * (r - c) + 1;
            if (len > best) best = len, start = c - (r - c);
        }
        c = firstAtLeast(EVEN_PLUS, (l + r + 2) / 2, r, r + 1);
        if (c != -1) {
            int len = 2 * (r - c + 1);
            if (len > best) best = len, start = c - (r - c + 1);
        }
        return {start, best};
    }

private:
    // Recursively finds the first valid index in O(log n).
    int firstAtLeast(int node, int l, int r, int type, int ql, int qr, int x) const {
        if (r < ql || qr < l || seg[node][type] < x) return -1;
        if (l == r) return l <= n ? l : -1;
        int mid = (l + r) / 2;
        int ans = firstAtLeast(node << 1, l, mid, type, ql, qr, x);
        if (ans != -1) return ans;
        return firstAtLeast(node << 1 | 1, mid + 1, r, type, ql, qr, x);
    }

    // Recursively finds the last valid index in O(log n).
    int lastAtLeast(int node, int l, int r, int type, int ql, int qr, int x) const {
        if (r < ql || qr < l || seg[node][type] < x) return -1;
        if (l == r) return l <= n ? l : -1;
        int mid = (l + r) / 2;
        int ans = lastAtLeast(node << 1 | 1, mid + 1, r, type, ql, qr, x);
        if (ans != -1) return ans;
        return lastAtLeast(node << 1, l, mid, type, ql, qr, x);
    }
};

struct WaveletCount {
    int32_t n = 0, lg = 1;
    vector<int32_t> zero;
    vector<vector<int32_t>> pref;

    WaveletCount() {}

    WaveletCount(const vector<int32_t> &a) { build(a); }

    void build(vector<int32_t> a) {
        n = a.size() - 1;
        int32_t mx = 0;
        for (int i = 1; i <= n; ++i) mx = max(mx, a[i]);
        lg = 1;
        while ((1LL << lg) <= mx) ++lg;
        zero.assign(lg, 0);
        pref.assign(lg, vector<int32_t>(n + 1));
        vector<int32_t> cur(a.begin() + 1, a.end()), nxt(n);
        for (int lv = 0; lv < lg; ++lv) {
            int bit = lg - 1 - lv;
            for (int i = 0; i < n; ++i)
                pref[lv][i + 1] = pref[lv][i] + (((cur[i] >> bit) & 1) == 0);
            zero[lv] = pref[lv][n];
            int p0 = 0, p1 = zero[lv];
            for (int x: cur) {
                if ((x >> bit) & 1) nxt[p1++] = x;
                else nxt[p0++] = x;
            }
            cur.swap(nxt);
        }
    }

    // Counts values < x in a[l..r] in O(log n).
    int countLess(int l, int r, int x) const {
        if (l > r || x <= 0) return 0;
        if (x >= (1LL << lg)) return r - l + 1;
        int L = l - 1, R = r, ans = 0;
        for (int lv = 0; lv < lg; ++lv) {
            int bit = lg - 1 - lv;
            int zl = pref[lv][L], zr = pref[lv][R];
            if ((x >> bit) & 1) {
                ans += zr - zl;
                L = zero[lv] + L - zl;
                R = zero[lv] + R - zr;
            } else {
                L = zl;
                R = zr;
            }
        }
        return ans;
    }

    // Counts values >= x in a[l..r] in O(log n).
    int countAtLeast(int l, int r, int x) const {
        if (l > r) return 0;
        return r - l + 1 - countLess(l, r, x);
    }
};

struct PalLenCount {
    WaveletCount odd, even;

    PalLenCount() {}

    PalLenCount(const Manacher &m) { build(m); }

    void build(const Manacher &m) {
        odd.build(m.odd);
        even.build(m.even);
    }

    // Counts palindrome occurrences of exactly len fully inside [l,r] in O(log n).
    int count(int l, int r, int len) const {
        if (len <= 0 || len > r - l + 1) return 0;
        if (len & 1) {
            int k = (len + 1) / 2;
            int L = l + k - 1, R = r - k + 1;
            return odd.countAtLeast(L, R, k);
        }
        int k = len / 2;
        int L = l + k, R = r - k + 1;
        return even.countAtLeast(L, R, k);
    }
};

struct PalRangeCountOffline {
    struct Answer {
        int total = 0, odd = 0, even = 0;
    };

    struct Fenwick {
        int n = 0;
        vector<int> cnt, sum;

        Fenwick() {}

        Fenwick(int n) { init(n); }

        void init(int n_) {
            n = n_;
            cnt.assign(n + 1, 0);
            sum.assign(n + 1, 0);
        }

        // Adds {countDelta,sumDelta} at index i in O(log n).
        void add(int i, int c, int s) {
            for (; i <= n; i += i & -i) cnt[i] += c, sum[i] += s;
        }

        // Returns prefix {count,sum} on [1..i] in O(log n).
        pair<int, int> get(int i) const {
            int c = 0, s = 0;
            for (; i; i -= i & -i) c += cnt[i], s += sum[i];
            return {c, s};
        }

        // Returns range {count,sum} on [l..r] in O(log n).
        pair<int, int> get(int l, int r) const {
            if (l > r) return {0, 0};
            auto a = get(r), b = get(l - 1);
            return {a.first - b.first, a.second - b.second};
        }
    };

    vector<int32_t> odd, even;
    vector<int> pOdd, pEven;
    int n = 0;

    PalRangeCountOffline() {}

    PalRangeCountOffline(const Manacher &m) { build(m); }

    void build(const Manacher &m) {
        odd = m.odd;
        even = m.even;
        n = odd.size() - 1;
        pOdd.assign(n + 1, 0);
        pEven.assign(n + 1, 0);
        for (int i = 1; i <= n; ++i) {
            pOdd[i] = pOdd[i - 1] + odd[i];
            pEven[i] = pEven[i - 1] + even[i];
        }
    }

    // Counts all palindrome occurrences for all queries in O((n+q) log n) time and O(n+q) memory.
    vector<Answer> count(const vector<pair<int, int>> &queries) const {
        int q = queries.size();
        vector<Answer> ans(q);
        vector<int> ord(q);
        iota(ord.begin(), ord.end(), 0);
        for (int id = 0; id < q; ++id) {
            auto [l, r] = queries[id];
            ans[id].odd = pOdd[r] - pOdd[l - 1];
            ans[id].even = pEven[r] - pEven[l];
        }
        auto leftSweep = [&](bool isEven) {
            vector<pair<int, int>> a(n);
            const auto &rad = isEven ? even : odd;
            for (int i = 1; i <= n; ++i)
                a[i - 1] = {i - rad[i] + (isEven ? 0 : 1), i};
            sort(a.begin(), a.end());
            sort(ord.begin(), ord.end(), [&](int x, int y) {
                return queries[x].first < queries[y].first;
            });
            Fenwick fw(n);
            int p = 0;
            for (int id: ord) {
                auto [l, r] = queries[id];
                while (p < n && a[p].first < l) {
                    fw.add(a[p].second, 1, a[p].first);
                    ++p;
                }
                int L = isEven ? l + 1 : l;
                int R = isEven ? (l + r + 1) / 2 : (l + r) / 2;
                auto [c, s] = fw.get(L, R);
                int cut = l * c - s;
                if (isEven) ans[id].even -= cut;
                else ans[id].odd -= cut;
            }
        };
        auto rightSweep = [&](bool isEven) {
            vector<pair<int, int>> a(n);
            const auto &rad = isEven ? even : odd;
            for (int i = 1; i <= n; ++i)
                a[i - 1] = {i + rad[i] - 1, i};
            sort(a.rbegin(), a.rend());
            sort(ord.begin(), ord.end(), [&](int x, int y) {
                return queries[x].second > queries[y].second;
            });
            Fenwick fw(n);
            int p = 0;
            for (int id: ord) {
                auto [l, r] = queries[id];
                while (p < n && a[p].first > r) {
                    fw.add(a[p].second, 1, a[p].first);
                    ++p;
                }
                int L = isEven ? (l + r + 1) / 2 + 1 : (l + r) / 2 + 1;
                int R = r;
                auto [c, s] = fw.get(L, R);
                int cut = s - r * c;
                if (isEven) ans[id].even -= cut;
                else ans[id].odd -= cut;
            }
        };
        leftSweep(false);
        rightSweep(false);
        leftSweep(true);
        rightSweep(true);
        for (auto &x: ans) x.total = x.odd + x.even;
        return ans;
    }
};

struct PalPatternRange {
    struct Node {
        int32_t len = 0, link = 0;
        array<int32_t, 26> next;

        Node(int32_t len = 0, int32_t link = 0) : len(len), link(link) {
            next.fill(-1);
        }
    };

    struct Wavelet {
        int32_t n = 0, lg = 1;
        vector<int32_t> zero;
        vector<vector<int32_t>> pref;

        void build(vector<int32_t> a, int32_t mx) {
            n = a.size();
            lg = 1;
            while ((1LL << lg) <= mx) ++lg;
            zero.assign(lg, 0);
            pref.assign(lg, vector<int32_t>(n + 1));
            vector<int32_t> nxt(n);
            for (int lv = 0; lv < lg; ++lv) {
                int bit = lg - 1 - lv;
                for (int i = 0; i < n; ++i)
                    pref[lv][i + 1] = pref[lv][i] + (((a[i] >> bit) & 1) == 0);
                zero[lv] = pref[lv][n];
                int p0 = 0, p1 = zero[lv];
                for (int x: a) {
                    if ((x >> bit) & 1) nxt[p1++] = x;
                    else nxt[p0++] = x;
                }
                a.swap(nxt);
            }
        }

        // Counts values < x in a[l..r) in O(log n).
        int countLess(int l, int r, int x) const {
            if (l >= r || x <= 0) return 0;
            if (x >= (1LL << lg)) return r - l;
            int ans = 0;
            for (int lv = 0; lv < lg; ++lv) {
                int bit = lg - 1 - lv;
                int zl = pref[lv][l], zr = pref[lv][r];
                if ((x >> bit) & 1) {
                    ans += zr - zl;
                    l = zero[lv] + l - zl;
                    r = zero[lv] + r - zr;
                } else {
                    l = zl;
                    r = zr;
                }
            }
            return ans;
        }

        // Counts values in [x,y] inside a[l..r) in O(log n).
        int count(int l, int r, int x, int y) const {
            if (l >= r || x > y) return 0;
            return countLess(l, r, y + 1) - countLess(l, r, x);
        }
    };

    string s;
    vector<Node> tree;
    vector<int32_t> suffAt, tin, tout;
    int32_t suff = 1;
    Wavelet wm;

    PalPatternRange() {}

    // Builds pattern-range queries from s in O(n log n).
    PalPatternRange(const string &s) { build(s); }

    // Builds exact palindrome-pattern occurrence queries in O(n log n) time and memory.
    void build(const string &str) {
        s.clear();
        tree.clear();
        tree.emplace_back(-1, 0);
        tree.emplace_back(0, 0);
        suffAt.clear();
        suff = 1;
        for (char ch: str) extend(ch);
        int m = tree.size();
        vector<vector<int32_t>> child(m);
        for (int v = 1; v < m; ++v) child[tree[v].link].push_back(v);
        tin.assign(m, 0);
        tout.assign(m, 0);
        vector<pair<int32_t, int32_t>> st = {{0, 0}};
        int32_t timer = 0;
        while (!st.empty()) {
            auto [v, state] = st.back();
            st.pop_back();
            if (!state) {
                tin[v] = timer++;
                st.push_back({v, 1});
                for (int i = (int) child[v].size() - 1; i >= 0; --i)
                    st.push_back({child[v][i], 0});
            } else {
                tout[v] = timer - 1;
            }
        }
        vector<int32_t> a(str.size());
        for (int i = 0; i < (int) str.size(); ++i) a[i] = tin[suffAt[i]];
        wm.build(a, m);
    }

    // Returns the Eertree node of palindrome p, or -1 if p is absent/not a palindrome, in O(|p|).
    int find(const string &p) const {
        if (p.empty()) return 1;
        int l = (p.size() - 1) / 2, r = p.size() / 2;
        int v = (p.size() & 1) ? 0 : 1;
        while (l >= 0) {
            if (p[l] != p[r]) return -1;
            v = tree[v].next[p[l] - 'a'];
            if (v == -1) return -1;
            --l, ++r;
        }
        return v;
    }

    // Counts occurrences of palindrome node v fully inside s[l..r] in O(log n).
    int count(int v, int l, int r) const {
        if (v < 2 || tree[v].len > r - l + 1) return 0;
        int L = l + tree[v].len - 1;
        return wm.count(L - 1, r, tin[v], tout[v]);
    }

    // Counts occurrences of palindrome p fully inside s[l..r] in O(|p|+log n).
    int count(const string &p, int l, int r) const {
        int v = find(p);
        return v == -1 ? 0 : count(v, l, r);
    }

private:
    // Returns the longest suffix node extendable by ch in amortized O(1).
    int getLink(int v, int pos, char ch) const {
        while (true) {
            int len = tree[v].len;
            if (pos - len - 1 >= 0 && s[pos - len - 1] == ch) return v;
            v = tree[v].link;
        }
    }

    // Appends ch to the internal Eertree in amortized O(1).
    void extend(char ch) {
        int pos = s.size(), c = ch - 'a';
        s += ch;
        int cur = getLink(suff, pos, ch);
        if (tree[cur].next[c] == -1) {
            int v = tree.size();
            tree.emplace_back(tree[cur].len + 2, 0);
            tree[cur].next[c] = v;
            if (tree[v].len == 1) tree[v].link = 1;
            else {
                int p = getLink(tree[cur].link, pos, ch);
                tree[v].link = tree[p].next[c];
            }
        }
        suff = tree[cur].next[c];
        suffAt.push_back(suff);
    }
};

struct PalWindow {
    struct Node {
        int len = 0, link = 0, occ = 0, firstEnd = -1;
        array<int, 26> next;

        Node(int len = 0, int link = 0) : len(len), link(link) {
            next.fill(-1);
        }
    };

    string s;
    int base = 1, suff = 1, distinctCnt = 0;
    vector<Node> tree;

    PalWindow() {}

    PalWindow(const string &s, int l, int r) { build(s, l, r); }

    // Builds an exact Eertree for s[l..r] in O(r-l+1) time and memory.
    void build(const string &str, int l, int r) {
        s = str.substr(l - 1, r - l + 1);
        base = l;
        tree.clear();
        tree.emplace_back(-1, 0);
        tree.emplace_back(0, 0);
        suff = 1;
        for (int i = 0; i < (int) s.size(); ++i) extend(i);
        for (int v = (int) tree.size() - 1; v >= 2; --v)
            tree[tree[v].link].occ += tree[v].occ;
        distinctCnt = tree.size() - 2;
    }

    // Counts distinct palindromes occurring at least k times in O(number of distinct palindromes).
    int countAtLeast(int k) const {
        int ans = 0;
        for (int v = 2; v < (int) tree.size(); ++v) ans += tree[v].occ >= k;
        return ans;
    }

    // Counts distinct palindromes occurring exactly k times in O(number of distinct palindromes).
    int countExactly(int k) const {
        int ans = 0;
        for (int v = 2; v < (int) tree.size(); ++v) ans += tree[v].occ == k;
        return ans;
    }

    // Returns {start,length} of the longest palindrome occurring at least k times in O(number of distinct palindromes).
    pair<int, int> longestAtLeast(int k) const {
        pair<int, int> ans = {-1, 0};
        for (int v = 2; v < (int) tree.size(); ++v) {
            if (tree[v].occ < k || tree[v].len <= ans.second) continue;
            ans = {base + tree[v].firstEnd - tree[v].len + 1, tree[v].len};
        }
        return ans;
    }

    // Returns {start,length} of the shortest palindrome occurring exactly k times in O(number of distinct palindromes).
    pair<int, int> shortestExactly(int k) const {
        pair<int, int> ans = {-1, LLONG_MAX};
        for (int v = 2; v < (int) tree.size(); ++v) {
            if (tree[v].occ != k || tree[v].len >= ans.second) continue;
            ans = {base + tree[v].firstEnd - tree[v].len + 1, tree[v].len};
        }
        if (ans.first == -1) ans.second = 0;
        return ans;
    }

    // Returns one shortest absent lowercase palindrome in O(26 * number of distinct palindromes).
    string shortestAbsent() const {
        int best = LLONG_MAX, node = -1, ch = -1;
        for (int v = 0; v < (int) tree.size(); ++v) {
            for (int c = 0; c < 26; ++c) {
                if (tree[v].next[c] != -1) continue;
                int len = tree[v].len + 2;
                if (len < best) best = len, node = v, ch = c;
            }
        }
        char c = 'a' + ch;
        if (node == 0) return string(1, c);
        if (node == 1) return string(2, c);
        int st = tree[node].firstEnd - tree[node].len + 1;
        return c + s.substr(st, tree[node].len) + c;
    }

    // Returns up to k longest distinct palindromes as {start,length} in O(d log d), d = distinct count.
    vector<pair<int, int>> topKDistinct(int k) const {
        vector<int> ids(tree.size() - 2);
        iota(ids.begin(), ids.end(), 2);
        sort(ids.begin(), ids.end(), [&](int a, int b) {
            return tree[a].len > tree[b].len;
        });
        if ((int) ids.size() > k) ids.resize(k);
        vector<pair<int, int>> ans;
        ans.reserve(ids.size());
        for (int v: ids)
            ans.push_back({base + tree[v].firstEnd - tree[v].len + 1, tree[v].len});
        return ans;
    }

private:
    // Returns the longest suffix node extendable at position pos in amortized O(1).
    int getLink(int v, int pos, char ch) const {
        while (true) {
            int len = tree[v].len;
            if (pos - len - 1 >= 0 && s[pos - len - 1] == ch) return v;
            v = tree[v].link;
        }
    }

    // Inserts s[pos] into the window Eertree in amortized O(1).
    void extend(int pos) {
        char ch = s[pos];
        int c = ch - 'a';
        int cur = getLink(suff, pos, ch);
        if (tree[cur].next[c] == -1) {
            int v = tree.size();
            tree.emplace_back(tree[cur].len + 2, 0);
            tree[v].firstEnd = pos;
            tree[cur].next[c] = v;
            if (tree[v].len == 1) tree[v].link = 1;
            else {
                int p = getLink(tree[cur].link, pos, ch);
                tree[v].link = tree[p].next[c];
            }
        }
        suff = tree[cur].next[c];
        tree[suff].occ++;
    }
};

struct DequeEertree {
    static const int A = 26;
    int32_t cap = 0, nodes = 2, distinctCnt = 0;
    vector<int32_t> len, link, par, freq;
    vector<array<int32_t, A>> next, jump;
    deque<int32_t> s, state, gap;

    DequeEertree() {}

    DequeEertree(int n) { init(n); }

    // Initializes an empty double-ended Eertree in O(n * A) memory.
    void init(int n) {
        cap = n + 2;
        nodes = 2;
        distinctCnt = 0;
        len.assign(cap, 0);
        link.assign(cap, 0);
        par.assign(cap, 0);
        freq.assign(cap, 0);
        next.assign(cap, {});
        jump.assign(cap, {});
        jump[0].fill(1);
        link[0] = 1;
        len[1] = -1;
        s.clear();
        state.clear();
        gap.clear();
        state.push_back(0);
    }

    // Returns the k-th character from the left in O(1), or -1 if outside.
    int32_t frontAt(const deque<int32_t> &d, int k) const {
        if (k < 0 || k >= (int) d.size()) return -1;
        return d[k];
    }

    // Returns the k-th character from the right in O(1), or -1 if outside.
    int32_t backAt(const deque<int32_t> &d, int k) const {
        if (k < 0 || k >= (int) d.size()) return -1;
        return d[(int) d.size() - 1 - k];
    }

    // Adds c to the left end in O(1) amortized for fixed alphabet.
    void pushFront(char c) {
        int x = c - 'a';
        s.push_front(x);
        int prev = state.front();
        int last = goFront(prev, x);
        if (freq[last]++ == 0) ++distinctCnt;
        int d = 2 + len[prev] - len[last];
        while (d + len[prev] <= len[last]) {
            state.pop_front();
            if (state.empty()) break;
            prev = state.front();
            d += gap.front();
            gap.pop_front();
        }
        if (!state.empty()) gap.push_front(d);
        state.push_front(last);
    }

    // Adds c to the right end in O(1) amortized for fixed alphabet.
    void pushBack(char c) {
        int x = c - 'a';
        s.push_back(x);
        int prev = state.back();
        int last = goBack(prev, x);
        if (freq[last]++ == 0) ++distinctCnt;
        int d = 2 + len[prev] - len[last];
        while (d + len[prev] <= len[last]) {
            state.pop_back();
            if (state.empty()) break;
            prev = state.back();
            d += gap.back();
            gap.pop_back();
        }
        if (!state.empty()) gap.push_back(d);
        state.push_back(last);
    }

    // Removes one character from the left end in O(1) amortized.
    void popFront() {
        int last = state.front();
        if (--freq[last] == 0) --distinctCnt;
        state.pop_front();
        s.pop_front();
        restoreFront(link[last], len[last] - len[link[last]]);
        restoreFront(par[last], 0);
        gap.pop_front();
    }

    // Removes one character from the right end in O(1) amortized.
    void popBack() {
        int last = state.back();
        if (--freq[last] == 0) --distinctCnt;
        state.pop_back();
        s.pop_back();
        restoreBack(link[last], len[last] - len[link[last]]);
        restoreBack(par[last], 0);
        gap.pop_back();
    }

    // Returns the number of distinct non-empty palindromes in O(1).
    int distinct() const {
        return distinctCnt;
    }

    // Returns the current deque length in O(1).
    int size() const {
        return s.size();
    }

private:
    // Creates/follows the palindrome transition after adding on the left in O(1) amortized.
    int goFront(int v, int c) {
        if (c != frontAt(s, len[v] + 1)) v = jump[v][c];
        if (!next[v][c]) {
            int u = next[jump[v][c]][c];
            link[nodes] = u;
            jump[nodes] = jump[u];
            int x = frontAt(s, len[u]);
            if (x != -1) jump[nodes][x] = u;
            len[nodes] = len[v] + 2;
            par[nodes] = v;
            next[v][c] = nodes++;
        }
        return next[v][c];
    }

    // Creates/follows the palindrome transition after adding on the right in O(1) amortized.
    int goBack(int v, int c) {
        if (c != backAt(s, len[v] + 1)) v = jump[v][c];
        if (!next[v][c]) {
            int u = next[jump[v][c]][c];
            link[nodes] = u;
            jump[nodes] = jump[u];
            int x = backAt(s, len[u]);
            if (x != -1) jump[nodes][x] = u;
            len[nodes] = len[v] + 2;
            par[nodes] = v;
            next[v][c] = nodes++;
        }
        return next[v][c];
    }

    // Restores one candidate state after a left pop in O(1).
    void restoreFront(int v, int d) {
        if (state.empty()) {
            state.push_front(v);
            gap.push_front(d);
            return;
        }
        int D = gap.front() - d;
        int prev = state.front();
        if (D + len[v] > len[prev]) {
            state.push_front(v);
            gap.pop_front();
            gap.push_front(D);
            gap.push_front(d);
        }
    }

    // Restores one candidate state after a right pop in O(1).
    void restoreBack(int v, int d) {
        if (state.empty()) {
            state.push_back(v);
            gap.push_back(d);
            return;
        }
        int D = gap.back() - d;
        int prev = state.back();
        if (D + len[v] > len[prev]) {
            state.push_back(v);
            gap.pop_back();
            gap.push_back(D);
            gap.push_back(d);
        }
    }
};

struct PalDistinctRange {
    string s;
    int L = 1, R = 0;
    DequeEertree tree;

    PalDistinctRange() {}

    PalDistinctRange(const string &s) { build(s); }

    // Prepares distinct-palindrome range queries in O(n * A) memory.
    void build(const string &str) {
        s = " " + str;
        tree.init(str.size());
        L = 1;
        R = 0;
    }

    // Resets the movable range to empty in O(n * A).
    void reset() {
        tree.init((int) s.size() - 1);
        L = 1;
        R = 0;
    }

    // Returns distinct palindromes in [l,r] in O(|L-l|+|R-r|) amortized moves.
    int count(int l, int r) {
        while (L > l) tree.pushFront(s[--L]);
        while (R < r) tree.pushBack(s[++R]);
        while (L < l) tree.popFront(), ++L;
        while (R > r) tree.popBack(), --R;
        return tree.distinct();
    }

    // Answers all queries with Mo in O((n+q)*sqrt(n)) amortized boundary moves.
    vector<int> countOffline(const vector<pair<int, int>> &queries) const {
        int n = (int) s.size() - 1, q = queries.size();
        int B = max<int>(1, sqrt(n));
        vector<int> ord(q), ans(q);
        iota(ord.begin(), ord.end(), 0);
        sort(ord.begin(), ord.end(), [&](int a, int b) {
            int A = queries[a].first / B, B2 = queries[b].first / B;
            if (A != B2) return A < B2;
            if (A & 1) return queries[a].second > queries[b].second;
            return queries[a].second < queries[b].second;
        });
        DequeEertree cur(n);
        int l = 1, r = 0;
        for (int id: ord) {
            auto [ql, qr] = queries[id];
            while (l > ql) cur.pushFront(s[--l]);
            while (r < qr) cur.pushBack(s[++r]);
            while (l < ql) cur.popFront(), ++l;
            while (r > qr) cur.popBack(), --r;
            ans[id] = cur.distinct();
        }
        return ans;
    }
};
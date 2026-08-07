#include <bits/stdc++.h>

using namespace std;
#define int long long

const int ALPHA = 26, M = 2;
const int32_t mod[M] = {1000000007, 1000000009};
const int32_t B[M] = {911382323, 97266353};
mt19937_64 rnd(chrono::steady_clock::now().time_since_epoch().count());

using H = array<int32_t, M>;
vector<H> pw(1, H{1, 1}), geo(1, H{0, 0});

void ensurePower(int n) {
    while ((int) pw.size() <= n) {
        H p{}, g{};
        int k = pw.size();
        for (int i = 0; i < M; ++i) {
            p[i] = (int64_t) pw.back()[i] * B[i] % mod[i];
            g[i] = (geo.back()[i] + pw[k - 1][i]) % mod[i];
        }
        pw.push_back(p);
        geo.push_back(g);
    }
}

struct Treap {
public:
    using Hash = H;
    using Mapping = array<uint8_t, ALPHA>;
    using Count = array<int32_t, ALPHA>;
private:
    struct Node {
        Node *l = nullptr, *r = nullptr;
        uint64_t pri = rnd();
        char val = 0;
        int32_t sz = 1;
        array<Hash, ALPHA> h{}, rh{};
        Count cnt{};
        Mapping to{};
        int8_t lazySort = 0;
        bool rev = false, lazyMap = false;

        Node(char c) : val(c) {
            for (int x = 0; x < ALPHA; ++x)to[x] = x;
            int x = c - 'a';
            cnt[x] = 1;
            for (int i = 0; i < M; ++i)
                h[x][i] = rh[x][i] = 1;
        }
    };

    using TreapNode = Node *;
    TreapNode root = nullptr;
    vector<TreapNode> garbage;

    int size(TreapNode t) { return t ? t->sz : 0; }

    void resetMap(Mapping &p) {
        for (int c = 0; c < ALPHA; ++c)p[c] = c;
    }

    int kthChar(const Count &cnt, int k, int dir) {
        if (dir > 0) {
            for (int c = 0; c < ALPHA; ++c)
                if (k < cnt[c])return c;
                else k -= cnt[c];
        } else {
            for (int c = ALPHA - 1; c >= 0; --c)
                if (k < cnt[c])return c;
                else k -= cnt[c];
        }
        return -1;
    }

    Count takeFirst(const Count &cnt, int k, int dir) {
        Count res{};
        if (dir > 0) {
            for (int c = 0; c < ALPHA && k; ++c) {
                int x = min<int>(cnt[c], k);
                res[c] = x, k -= x;
            }
        } else {
            for (int c = ALPHA - 1; c >= 0 && k; --c) {
                int x = min<int>(cnt[c], k);
                res[c] = x, k -= x;
            }
        }
        return res;
    }

    void applySorted(TreapNode t, const Count &cnt, int dir) {
        if (!t)return;
        ensurePower(t->sz);
        t->cnt = cnt;
        t->h = {};
        t->rh = {};
        int pref = 0;
        if (dir > 0) {
            for (int c = 0; c < ALPHA; ++c) {
                int k = cnt[c];
                if (!k)continue;
                int suff = t->sz - pref - k;
                for (int i = 0; i < M; ++i) {
                    t->h[c][i] = (int64_t) geo[k][i] * pw[suff][i] % mod[i];
                    t->rh[c][i] = (int64_t) geo[k][i] * pw[pref][i] % mod[i];
                }
                pref += k;
            }
        } else {
            for (int c = ALPHA - 1; c >= 0; --c) {
                int k = cnt[c];
                if (!k)continue;
                int suff = t->sz - pref - k;
                for (int i = 0; i < M; ++i) {
                    t->h[c][i] = (int64_t) geo[k][i] * pw[suff][i] % mod[i];
                    t->rh[c][i] = (int64_t) geo[k][i] * pw[pref][i] % mod[i];
                }
                pref += k;
            }
        }
        t->val = char('a' + kthChar(cnt, size(t->l), dir));
        resetMap(t->to);
        t->lazyMap = false;
        t->rev = false;
        t->lazySort = dir;
    }

    void pushSort(TreapNode t) {
        if (!t || !t->lazySort)return;
        int dir = t->lazySort, lsz = size(t->l);
        Count lc = takeFirst(t->cnt, lsz, dir), rem = t->cnt;
        for (int c = 0; c < ALPHA; ++c)rem[c] -= lc[c];
        int rootChar = kthChar(rem, 0, dir);
        rem[rootChar]--;
        applySorted(t->l, lc, dir);
        applySorted(t->r, rem, dir);
        t->val = char('a' + rootChar);
        t->lazySort = 0;
    }

    // Returns current forward hash in O(ALPHA * M).
    Hash hash(TreapNode t) {
        Hash res{};
        if (!t)return res;
        for (int c = 0; c < ALPHA; ++c)
            for (int i = 0; i < M; ++i)
                res[i] = (res[i] + (int64_t) (c + 1) * t->h[c][i]) % mod[i];
        return res;
    }

    // Returns current reverse hash in O(ALPHA * M).
    Hash reverseHash(TreapNode t) {
        Hash res{};
        if (!t)return res;
        for (int c = 0; c < ALPHA; ++c)
            for (int i = 0; i < M; ++i)
                res[i] = (res[i] + (int64_t) (c + 1) * t->rh[c][i]) % mod[i];
        return res;
    }

    // Applies any character mapping c -> p[c] in O(ALPHA * M).
    void applyMap(TreapNode t, const Mapping &p) {
        if (!t)return;
        if (t->lazySort)pushSort(t);
        array<Hash, ALPHA> nh{}, nrh{};
        Count nc{};
        for (int c = 0; c < ALPHA; ++c) {
            int d = p[c];
            nc[d] += t->cnt[c];
            for (int i = 0; i < M; ++i) {
                nh[d][i] = (nh[d][i] + t->h[c][i]) % mod[i];
                nrh[d][i] = (nrh[d][i] + t->rh[c][i]) % mod[i];
            }
        }
        t->cnt = nc;
        t->h = nh;
        t->rh = nrh;
        t->val = char('a' + p[t->val - 'a']);
        for (int c = 0; c < ALPHA; ++c)
            t->to[c] = p[t->to[c]];
        t->lazyMap = true;
    }

    void applyReverse(TreapNode t) {
        if (!t)return;
        if (t->lazySort)pushSort(t);
        swap(t->l, t->r);
        swap(t->h, t->rh);
        t->rev ^= 1;
    }

    void push(TreapNode t) {
        if (!t)return;
        if (t->lazySort)pushSort(t);
        if (t->lazyMap) {
            applyMap(t->l, t->to);
            applyMap(t->r, t->to);
            resetMap(t->to);
            t->lazyMap = false;
        }
        if (t->rev) {
            applyReverse(t->l);
            applyReverse(t->r);
            t->rev = false;
        }
    }

    // Recalculates per-character positional hashes in O(ALPHA * M).
    void pull(TreapNode t) {
        if (!t)return;
        push(t);
        int lsz = size(t->l), rsz = size(t->r);
        t->sz = lsz + rsz + 1;
        ensurePower(t->sz);
        t->cnt.fill(0);
        for (int c = 0; c < ALPHA; ++c) {
            t->cnt[c] = (t->l ? t->l->cnt[c] : 0) + (t->val - 'a' == c) + (t->r ? t->r->cnt[c] : 0);
            for (int i = 0; i < M; ++i) {
                int64_t lh = t->l ? t->l->h[c][i] : 0;
                int64_t rh = t->r ? t->r->h[c][i] : 0;
                int64_t lr = t->l ? t->l->rh[c][i] : 0;
                int64_t rr = t->r ? t->r->rh[c][i] : 0;
                t->h[c][i] = (lh * pw[rsz + 1][i] + (t->val - 'a' == c) * pw[rsz][i] + rh) % mod[i];
                t->rh[c][i] = (rr * pw[lsz + 1][i] + (t->val - 'a' == c) * pw[lsz][i] + lr) % mod[i];
            }
        }
    }

    void split(TreapNode t, TreapNode &l, TreapNode &r, int k) {
        if (!t)return void(l = r = nullptr);
        push(t);
        if (size(t->l) >= k)
            split(t->l, l, t->l, k), r = t;
        else
            split(t->r, t->r, r, k - size(t->l) - 1), l = t;
        pull(t);
    }

    TreapNode merge(TreapNode l, TreapNode r) {
        if (!l || !r)return l ? l : r;
        push(l);
        push(r);
        if (l->pri > r->pri) {
            l->r = merge(l->r, r);
            pull(l);
            return l;
        }
        r->l = merge(l, r->l);
        pull(r);
        return r;
    }

    TreapNode buildTree(const string &s) {
        if (s.empty())return nullptr;
        ensurePower(s.size());
        vector<TreapNode> st;
        TreapNode rt = nullptr;
        for (auto c: s) {
            auto cur = new Node(c);
            TreapNode last = nullptr;
            while (!st.empty() && st.back()->pri < cur->pri) {
                last = st.back();
                st.pop_back();
            }
            cur->l = last;
            if (!st.empty())st.back()->r = cur;
            else rt = cur;
            st.push_back(cur);
        }
        vector<TreapNode> ord{rt};
        for (int i = 0; i < (int) ord.size(); ++i) {
            if (ord[i]->l)ord.push_back(ord[i]->l);
            if (ord[i]->r)ord.push_back(ord[i]->r);
        }
        reverse(ord.begin(), ord.end());
        for (auto t: ord)pull(t);
        return rt;
    }

    void splitRange(int l, int r, TreapNode &a, TreapNode &b, TreapNode &c) {
        split(root, a, b, l);
        split(b, b, c, r - l + 1);
    }

    void join(TreapNode a, TreapNode b, TreapNode c) {
        root = merge(a, merge(b, c));
    }

    TreapNode kth(TreapNode t, int k) {
        while (t) {
            push(t);
            int lsz = size(t->l);
            if (k < lsz)t = t->l;
            else if (k == lsz)return t;
            else k -= lsz + 1, t = t->r;
        }
        return nullptr;
    }

    void destroy(TreapNode t) {
        if (!t)return;
        vector<TreapNode> st{t};
        while (!st.empty()) {
            auto v = st.back();
            st.pop_back();
            if (v->l)st.push_back(v->l);
            if (v->r)st.push_back(v->r);
            delete v;
        }
    }

    void collect(TreapNode t, string &s) {
        if (!t)return;
        push(t);
        collect(t->l, s);
        s += t->val;
        collect(t->r, s);
    }

public:
    static Mapping identityMap() {
        Mapping p{};
        for (int c = 0; c < ALPHA; ++c)p[c] = c;
        return p;
    }

    Treap() { ensurePower(1); }

    Treap(const string &s) { build(s); }

    Treap(const Treap &) = delete;

    Treap &operator=(const Treap &) = delete;

    Treap(Treap &&other) noexcept {
        root = other.root;
        other.root = nullptr;
        garbage.swap(other.garbage);
    }

    Treap &operator=(Treap &&other) noexcept {
        if (this == &other)return *this;
        clear();
        root = other.root;
        other.root = nullptr;
        garbage.swap(other.garbage);
        return *this;
    }

    ~Treap() { clear(); }

    void clear() {
        destroy(root);
        root = nullptr;
        for (auto t: garbage)destroy(t);
        garbage.clear();
    }

    void releaseCutMemory() {
        for (auto t: garbage)destroy(t);
        garbage.clear();
    }

    void build(const string &s) {
        clear();
        root = buildTree(s);
    }

    int size() { return size(root); }

    bool empty() { return !root; }

    void insert(int pos, char c) {
        ensurePower(size() + 1);
        TreapNode l, r, mid = new Node(c);
        split(root, l, r, pos);
        root = merge(l, merge(mid, r));
    }

    void insertString(int pos, const string &s) {
        if (s.empty())return;
        TreapNode l, r, mid = buildTree(s);
        split(root, l, r, pos);
        root = merge(l, merge(mid, r));
    }

    void pushBack(char c) { insert(size(), c); }

    void pushFront(char c) { insert(0, c); }

    void append(const string &s) { insertString(size(), s); }

    void prepend(const string &s) { insertString(0, s); }

    void erase(int l, int r) {
        TreapNode a, b, c;
        splitRange(l, r, a, b, c);
        root = merge(a, c);
        destroy(b);
    }

    void erase(int pos) { erase(pos, pos); }

    // O(log n) structural cut; removed memory is freed later.
    void cut(int l, int r) {
        TreapNode a, b, c;
        splitRange(l, r, a, b, c);
        root = merge(a, c);
        if (b)garbage.push_back(b);
    }

    void popBack() { erase(size() - 1); }

    void popFront() { erase(0); }

    void replaceRange(int l, int r, const string &s) {
        TreapNode a, b, c;
        splitRange(l, r, a, b, c);
        destroy(b);
        b = buildTree(s);
        join(a, b, c);
    }

    char get(int pos) { return kth(root, pos)->val; }

    void set(int pos, char c) { rangeAssign(pos, pos, c); }

    char front() { return get(0); }

    char back() { return get(size() - 1); }

    // Applies any mapping c -> p[c] to [l,r].
    void rangeMap(int l, int r, const Mapping &p) {
        TreapNode a, b, c;
        splitRange(l, r, a, b, c);
        applyMap(b, p);
        join(a, b, c);
    }

    // Cyclically shifts every character by k.
    void rangeShift(int l, int r, int k) {
        k = (k % ALPHA + ALPHA) % ALPHA;
        if (!k)return;
        auto p = identityMap();
        for (int c = 0; c < ALPHA; ++c)
            p[c] = (c + k) % ALPHA;
        rangeMap(l, r, p);
    }

    // Replaces every x with y and every y with x.
    void rangeSwapChars(int l, int r, char x, char y) {
        if (x == y)return;
        auto p = identityMap();
        swap(p[x - 'a'], p[y - 'a']);
        rangeMap(l, r, p);
    }

    // Replaces every occurrence of x with y.
    void rangeReplace(int l, int r, char x, char y) {
        if (x == y)return;
        auto p = identityMap();
        p[x - 'a'] = y - 'a';
        rangeMap(l, r, p);
    }

    // Replaces every character with c.
    void rangeAssign(int l, int r, char c) {
        Mapping p{};
        p.fill(c - 'a');
        rangeMap(l, r, p);
    }

    // Sorts [l,r] ascending or descending in O(ALPHA * M * log n) expected.
    void rangeSort(int l, int r, bool ascending = true) {
        TreapNode a, b, c;
        splitRange(l, r, a, b, c);
        Count cnt = b->cnt;
        applySorted(b, cnt, ascending ? 1 : -1);
        join(a, b, c);
    }

    // Sorts the whole string ascending or descending in O(ALPHA * M).
    void sortAll(bool ascending = true) {
        if (!root)return;
        Count cnt = root->cnt;
        applySorted(root, cnt, ascending ? 1 : -1);
    }

    void rangeReverse(int l, int r) {
        TreapNode a, b, c;
        splitRange(l, r, a, b, c);
        applyReverse(b);
        join(a, b, c);
    }

    void reverseAll() { applyReverse(root); }

    // Positive k rotates right, negative k rotates left.
    void rotate(int l, int r, int k) {
        int len = r - l + 1;
        k = (k % len + len) % len;
        if (!k)return;
        TreapNode a, b, c, x, y;
        splitRange(l, r, a, b, c);
        split(b, x, y, len - k);
        b = merge(y, x);
        join(a, b, c);
    }

    void rotateAll(int k) {
        if (size())rotate(0, size() - 1, k);
    }

    void cyclicShift(int l, int r, int k, bool left = false) {
        rotate(l, r, left ? -k : k);
    }

    // pos is an index in the remaining string after removing [l,r].
    void move(int l, int r, int pos) {
        TreapNode a, b, c, x, y;
        splitRange(l, r, a, b, c);
        root = merge(a, c);
        split(root, x, y, pos);
        root = merge(x, merge(b, y));
    }

    void moveToFront(int l, int r) { move(l, r, 0); }

    void moveToBack(int l, int r) {
        move(l, r, size() - (r - l + 1));
    }

    // Ranges must not overlap.
    void swapRanges(int l1, int r1, int l2, int r2) {
        if (l2 < l1) {
            swap(l1, l2);
            swap(r1, r2);
        }
        assert(r1 < l2);
        TreapNode a, b, c, d, e, t;
        split(root, a, t, l1);
        split(t, b, t, r1 - l1 + 1);
        split(t, c, t, l2 - r1 - 1);
        split(t, d, e, r2 - l2 + 1);
        root = merge(a, merge(d, merge(c, merge(b, e))));
    }

    void swapElements(int i, int j) {
        if (i != j)swapRanges(i, i, j, j);
    }

    Hash getHash(int l, int r) {
        TreapNode a, b, c;
        splitRange(l, r, a, b, c);
        Hash ans = hash(b);
        join(a, b, c);
        return ans;
    }

    Hash getReverseHash(int l, int r) {
        TreapNode a, b, c;
        splitRange(l, r, a, b, c);
        Hash ans = reverseHash(b);
        join(a, b, c);
        return ans;
    }

    Hash wholeHash() { return hash(root); }

    Hash wholeReverseHash() { return reverseHash(root); }

    Hash hashOf(const string &s) {
        Hash h{};
        for (auto c: s)
            for (int i = 0; i < M; ++i)
                h[i] = ((int64_t) h[i] * B[i] + c - 'a' + 1) % mod[i];
        return h;
    }

    bool equal(int l1, int r1, int l2, int r2) {
        return r1 - l1 == r2 - l2 && getHash(l1, r1) == getHash(l2, r2);
    }

    bool equalString(int l, int r, const string &s) {
        return r - l + 1 == (int) s.size() && getHash(l, r) == hashOf(s);
    }

    bool reverseEqual(int l1, int r1, int l2, int r2) {
        return r1 - l1 == r2 - l2 && getHash(l1, r1) == getReverseHash(l2, r2);
    }

    bool isPalindrome(int l, int r) {
        TreapNode a, b, c;
        splitRange(l, r, a, b, c);
        bool ans = hash(b) == reverseHash(b);
        join(a, b, c);
        return ans;
    }

    bool isPalindrome() { return wholeHash() == wholeReverseHash(); }

    int lcp(int l1, int r1, int l2, int r2) {
        int lo = 0, hi = min(r1 - l1 + 1, r2 - l2 + 1);
        while (lo < hi) {
            int mid = (lo + hi + 1) / 2;
            if (getHash(l1, l1 + mid - 1) == getHash(l2, l2 + mid - 1))
                lo = mid;
            else hi = mid - 1;
        }
        return lo;
    }

    int lcs(int l1, int r1, int l2, int r2) {
        int lo = 0, hi = min(r1 - l1 + 1, r2 - l2 + 1);
        while (lo < hi) {
            int mid = (lo + hi + 1) / 2;
            if (getHash(r1 - mid + 1, r1) == getHash(r2 - mid + 1, r2))
                lo = mid;
            else hi = mid - 1;
        }
        return lo;
    }

    int compare(int l1, int r1, int l2, int r2) {
        int n = r1 - l1 + 1, m = r2 - l2 + 1;
        int k = lcp(l1, r1, l2, r2);
        if (k == min(n, m))return (n > m) - (n < m);
        return get(l1 + k) > get(l2 + k) ? 1 : -1;
    }

    bool isPrefix(int l1, int r1, int l2, int r2) {
        int len = r1 - l1 + 1;
        return len <= r2 - l2 + 1 && equal(l1, r1, l2, l2 + len - 1);
    }

    bool isSuffix(int l1, int r1, int l2, int r2) {
        int len = r1 - l1 + 1;
        return len <= r2 - l2 + 1 && equal(l1, r1, r2 - len + 1, r2);
    }

    int longestOddPalindrome(int c) {
        int lo = 0, hi = min(c, size() - 1 - c);
        while (lo < hi) {
            int mid = (lo + hi + 1) / 2;
            if (isPalindrome(c - mid, c + mid))
                lo = mid;
            else hi = mid - 1;
        }
        return 2 * lo + 1;
    }

    int longestEvenPalindrome(int c) {
        if (c < 0 || c + 1 >= size())return 0;
        int lo = 0, hi = min(c + 1, size() - c - 1);
        while (lo < hi) {
            int mid = (lo + hi + 1) / 2;
            if (isPalindrome(c - mid + 1, c + mid))
                lo = mid;
            else hi = mid - 1;
        }
        return 2 * lo;
    }

    string substr(int l, int r) {
        TreapNode a, b, c;
        splitRange(l, r, a, b, c);
        string s;
        s.reserve(r - l + 1);
        collect(b, s);
        join(a, b, c);
        return s;
    }

    string toString() {
        string s;
        s.reserve(size());
        collect(root, s);
        return s;
    }

    void print() { cout << toString(); }
};

vector<int> linear_sieve(int n) {
    vector<int> lp(n + 1);
    vector<int> pr;
    for (int i = 2; i <= n; ++i) {
        if (lp[i] == 0) {
            lp[i] = i;
            pr.push_back(i);
        }
        for (int j = 0; i * pr[j] <= n; ++j) {
            lp[i * pr[j]] = pr[j];
            if (pr[j] == lp[i]) { break; }
        }
    }
    return lp;
}

void solve() {
    int n, q;
    string s;
    cin >> n >> q >> s;
    Treap T(s);
    vector<vector<int>> divs(n + 1);
    for (int i = 1; i <= n; ++i)
        for (int j = i; j <= n; j += i) divs[j].emplace_back(i);
    auto lp = linear_sieve(s.size());
    auto minPeriod = [&](int l, int r) -> long long {
        if ((l == r) || T.getHash(l, r - 1) == T.getHash(l + 1, r)) return 1;
        int len = (r - l + 1), ans = len;
        while (len > 1) {
            if (T.getHash(l, r - ans / lp[len]) == T.getHash(l + ans / lp[len], r))
                ans /= lp[len];
            len /= lp[len];
        }
        return ans;
    };
    set<H> st;
    while (q--) {
        int l, r;
        cin >> l >> r, --l, --r;
        int minP = minPeriod(l, r);
        auto hash = T.getHash(l, l + minP - 1);
        if (st.count(hash)) continue;
        st.insert(hash);
        T.rangeReverse(l, r);
    }
    T.print();
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
#include <bits/stdc++.h>

using namespace std;
#define int long long
const int M = 26;
mt19937_64 rnd(chrono::steady_clock::now().time_since_epoch().count());

// don't forget to update what you want only
struct Treap {
private:
    struct Node {
        Node *l = nullptr, *r = nullptr, *p = nullptr;
        uint64_t pri = rnd();
        int id = 0, val = 0, sz = 1, sum = 0, mn = 0, mx = 0;
        int mul = 1, add = 0, dif = 0;
        bool rev = false, good = true, perm = false;
        array<int, M> frq{}, to{};

        Node(int _val, int _id) : id(_id), val(_val), sum(_val), mn(_val), mx(_val) {
            for (int i = 0; i < M; ++i)
                to[i] = i;
            frq[val] = 1;
        }
    };

    using TreapNode = Node *;
    TreapNode root = nullptr;
    vector<TreapNode> byId{nullptr};

    int size(TreapNode t) { return t ? t->sz : 0; }

    int sum(TreapNode t) { return t ? t->sum : 0; }

    int mn(TreapNode t) { return t ? t->mn : LLONG_MAX; }

    int mx(TreapNode t) { return t ? t->mx : LLONG_MIN; }

    void pull(TreapNode t) {
        if (!t)return;
        t->sz = size(t->l) + size(t->r) + 1;
        t->sum = sum(t->l) + t->val + sum(t->r);
        t->good = (!t->l or t->l->good) and (!t->r or t->r->good);
        if (t->good) {
            t->mn = min(t->val, min(mn(t->l), mn(t->r)));
            t->mx = max(t->val, max(mx(t->l), mx(t->r)));
        }

        t->frq.fill(0);
        if (t->l)
            for (int i = 0; i < M; ++i)
                t->frq[i] += t->l->frq[i];
        t->frq[t->val]++;
        if (t->r)
            for (int i = 0; i < M; ++i)
                t->frq[i] += t->r->frq[i];
        if (t->l)t->l->p = t;
        if (t->r)t->r->p = t;
    }

    void applyTag(TreapNode t, int a, int b, int d = 0) {
        if (!t)return;
        int n = t->sz, pos = size(t->l);
        t->val = a * t->val + b + d * pos;
        t->sum = a * t->sum + b * n + d * n * (n - 1) / 2;
        if (!a) {
            int x = b, y = b + d * (n - 1);
            t->mn = min(x, y);
            t->mx = max(x, y);
            t->good = true;
        } else if (!d and t->good) {
            int oldMn = t->mn, oldMx = t->mx;
            if (a > 0)t->mn = a * oldMn + b, t->mx = a * oldMx + b;
            else t->mn = a * oldMx + b, t->mx = a * oldMn + b;
        } else if (d)t->good = false;
        t->mul = a * t->mul;
        t->add = a * t->add + b;
        t->dif = a * t->dif + d;
    }

    void applyReverse(TreapNode t) {
        if (!t)return;
        swap(t->l, t->r);
        t->add += t->dif * (t->sz - 1);
        t->dif = -t->dif;
        t->rev ^= 1;
    }

    void applyPerm(TreapNode t, const array<int, M> &p) {
        if (!t)return;
        array<int, M> nf{};
        for (int i = 0; i < M; ++i)
            nf[p[i]] += t->frq[i];
        t->frq = nf;
        t->val = p[t->val];
        t->sum = 0;
        t->mn = LLONG_MAX;
        t->mx = LLONG_MIN;
        for (int i = 0; i < M; ++i) {
            t->sum += i * t->frq[i];
            if (t->frq[i]) {
                t->mn = min(t->mn, i);
                t->mx = max(t->mx, i);
            }
        }
        for (int i = 0; i < M; ++i)
            t->to[i] = p[t->to[i]];
        t->good = true;
        t->perm = true;
    }

    void push(TreapNode t) {
        if (!t)return;
        if (t->rev) {
            applyReverse(t->l);
            applyReverse(t->r);
            t->rev = false;
        }

        if (t->perm) {
            applyPerm(t->l, t->to);
            applyPerm(t->r, t->to);

            iota(t->to.begin(), t->to.end(), 0);
            t->perm = false;
        }
        if (t->mul != 1 or t->add or t->dif) {
            int shift = size(t->l) + 1;
            applyTag(t->l, t->mul, t->add, t->dif);
            applyTag(t->r, t->mul, t->add + t->dif * shift, t->dif);
            t->mul = 1;
            t->add = t->dif = 0;
        }
    }

    // O(n)
    void normalize(TreapNode t) {
        if (!t or t->good)return;
        push(t);
        normalize(t->l);
        normalize(t->r);
        pull(t);
    }

    void split(TreapNode t, TreapNode &l, TreapNode &r, int k) {
        if (!t)return void(l = r = nullptr);
        push(t);
        if (size(t->l) >= k) split(t->l, l, t->l, k), r = t;
        else split(t->r, t->r, r, k - size(t->l) - 1), l = t;
        pull(t);
        if (l)l->p = nullptr;
        if (r)r->p = nullptr;
    }

    TreapNode merge(TreapNode l, TreapNode r) {
        if (!l or !r) {
            auto t = l ? l : r;
            if (t)t->p = nullptr;
            return t;
        }
        push(l);
        push(r);
        if (l->pri > r->pri) {
            l->r = merge(l->r, r);
            pull(l);
            l->p = nullptr;
            return l;
        }
        r->l = merge(l, r->l);
        pull(r);
        r->p = nullptr;
        return r;
    }

    TreapNode makeNode(int val) {
        int id = byId.size();
        auto t = new Node(val, id);
        byId.push_back(t);
        return t;
    }

    void destroy(TreapNode t) {
        if (!t)return;
        vector<TreapNode> st{t};
        while (!st.empty()) {
            auto v = st.back();
            st.pop_back();
            if (v->l)st.push_back(v->l);
            if (v->r)st.push_back(v->r);
            byId[v->id] = nullptr;
            delete v;
        }
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
            int left = size(t->l);
            if (k < left)t = t->l;
            else if (k == left)return t;
            else k -= left + 1, t = t->r;
        }
        return nullptr;
    }

    int firstGE(TreapNode t, int x) {
        if (!t)return -1;
        normalize(t);
        if (t->mx < x)return -1;
        push(t);
        if (t->l and t->l->mx >= x)return firstGE(t->l, x);
        if (t->val >= x)return size(t->l);
        int res = firstGE(t->r, x);
        return res == -1 ? -1 : size(t->l) + 1 + res;
    }

    int firstLE(TreapNode t, int x) {
        if (!t)return -1;
        normalize(t);
        if (t->mn > x)return -1;
        push(t);
        if (t->l and t->l->mn <= x)return firstLE(t->l, x);
        if (t->val <= x)return size(t->l);
        int res = firstLE(t->r, x);
        return res == -1 ? -1 : size(t->l) + 1 + res;
    }

    int lastGE(TreapNode t, int x) {
        if (!t)return -1;
        normalize(t);
        if (t->mx < x)return -1;
        push(t);
        if (t->r and t->r->mx >= x) {
            int res = lastGE(t->r, x);
            return size(t->l) + 1 + res;
        }
        if (t->val >= x)return size(t->l);
        return lastGE(t->l, x);
    }

    int lastLE(TreapNode t, int x) {
        if (!t)return -1;
        normalize(t);
        if (t->mn > x)return -1;
        push(t);
        if (t->r and t->r->mn <= x) {
            int res = lastLE(t->r, x);
            return size(t->l) + 1 + res;
        }
        if (t->val <= x)return size(t->l);
        return lastLE(t->l, x);
    }

    void pushPath(TreapNode t) {
        vector<TreapNode> path;
        for (auto v = t; v; v = v->p)
            path.push_back(v);
        reverse(path.begin(), path.end());
        for (auto v: path)
            push(v);
    }

    void collect(TreapNode t, vector<int> &a) {
        if (!t)return;
        push(t);
        collect(t->l, a);
        a.push_back(t->val);
        collect(t->r, a);
    }

public:
    struct Info {
        int sum = 0, mn = LLONG_MAX, mx = LLONG_MIN;
    };

    Treap() = default;

    Treap(const vector<int> &a) { build(a); }

    ~Treap() { clear(); }

    // Clears the treap in O(n).
    void clear() {
        destroy(root);
        root = nullptr;
        byId.assign(1, nullptr);
    }

    // Builds from an array in O(n).
    void build(const vector<int> &a) {
        clear();
        vector<TreapNode> st;
        for (auto x: a) {
            auto cur = makeNode(x);
            TreapNode last = nullptr;
            while (!st.empty() and st.back()->pri < cur->pri) {
                last = st.back();
                st.pop_back();
            }
            cur->l = last;
            if (last)last->p = cur;
            if (!st.empty()) {
                st.back()->r = cur;
                cur->p = st.back();
            } else root = cur;
            st.push_back(cur);
        }
        if (!root)return;
        vector<TreapNode> ord{root};
        for (int i = 0; i < ord.size(); ++i) {
            if (ord[i]->l)ord.push_back(ord[i]->l);
            if (ord[i]->r)ord.push_back(ord[i]->r);
        }
        reverse(ord.begin(), ord.end());
        for (auto t: ord)
            pull(t);
        root->p = nullptr;
    }

    // Returns the number of elements in O(1).
    int size() { return size(root); }

    // Checks whether the treap is empty in O(1).
    bool empty() { return !root; }

    // Inserts val before pos and returns its stable id in O(log n) expected.
    int insert(int pos, int val) {
        TreapNode l, r;
        split(root, l, r, pos);
        auto mid = makeNode(val);
        root = merge(l, merge(mid, r));
        return mid->id;
    }

    int pushBack(int val) { return insert(size(), val); }

    int pushFront(int val) { return insert(0, val); }

    // Erases [l, r] in O(log n + removed) expected.
    void erase(int l, int r) {
        TreapNode a, b, c;
        splitRange(l, r, a, b, c);
        destroy(b);
        root = merge(a, c);
    }

    void erase(int pos) { erase(pos, pos); }

    void popBack() { erase(size() - 1); }

    void popFront() { erase(0); }

    // Returns a[pos] in O(log n) expected.
    int get(int pos) { return kth(root, pos)->val; }

    // Changes a[pos] to val in O(log n) expected.
    void set(int pos, int val) { rangeAssign(pos, pos, val); }

    int front() { return get(0); }

    int back() { return get(size() - 1); }

    int idAt(int pos) { return kth(root, pos)->id; }

    // Returns {sum, min, max} on [l, r].
    Info query(int l, int r) {
        TreapNode a, b, c;
        splitRange(l, r, a, b, c);
        normalize(b);
        Info ans{b->sum, b->mn, b->mx};
        join(a, b, c);
        return ans;
    }

    // Returns the sum of [l, r] in O(log n) expected.
    int rangeSum(int l, int r) {
        TreapNode a, b, c;
        splitRange(l, r, a, b, c);
        int ans = b->sum;
        join(a, b, c);
        return ans;
    }

    array<int, M> rangeFrq(int l, int r) {
        TreapNode a, b, c;
        splitRange(l, r, a, b, c);
        array<int, M> ans = b->frq;
        join(a, b, c);
        return ans;
    }

    int rangeMin(int l, int r) { return query(l, r).mn; }

    int rangeMax(int l, int r) { return query(l, r).mx; }

    int totalSum() { return sum(root); }

    // Applies a[i] = mul * a[i] + first + (i-l) * diff on [l, r].
    void rangeTransform(int l, int r, int mul, int first, int diff = 0) {
        TreapNode a, b, c;
        splitRange(l, r, a, b, c);
        applyTag(b, mul, first, diff);
        join(a, b, c);
    }

    // Adds x to every element in [l, r].
    void rangeAdd(int l, int r, int x) { rangeTransform(l, r, 1, x); }

    // Multiplies every element in [l, r] by x.
    void rangeMul(int l, int r, int x) { rangeTransform(l, r, x, 0); }

    // Assigns every element in [l, r] to x.
    void rangeAssign(int l, int r, int x) { rangeTransform(l, r, 0, x); }

    // Adds first, first+d, first+2d, ... to [l, r].
    void rangeAPAdd(int l, int r, int first, int d) {
        rangeTransform(l, r, 1, first, d);
    }

    // Assigns [l, r] to first, first+d, first+2d, ...
    void rangeAPAssign(int l, int r, int first, int d) {
        rangeTransform(l, r, 0, first, d);
    }

    // Reverses [l, r] in O(log n) expected.
    void rangeReverse(int l, int r) {
        TreapNode a, b, c;
        splitRange(l, r, a, b, c);
        applyReverse(b);
        join(a, b, c);
    }

    void reverseAll() { applyReverse(root); }

    // Rotates [l, r] by k; positive right and negative left.
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
        if (size())
            rotate(0, size() - 1, k);
    }

    // Removes [l, r] and inserts it before pos in the remaining array.
    void move(int l, int r, int pos) {
        TreapNode a, b, c, x, y;
        splitRange(l, r, a, b, c);
        root = merge(a, c);
        split(root, x, y, pos);
        root = merge(x, merge(b, y));
    }

    void moveToFront(int l, int r) { move(l, r, 0); }

    void moveToBack(int l, int r) { move(l, r, size() - (r - l + 1)); }

    void applySwap(TreapNode t, int a, int b) {
        if (!t or a == b)return;
        array<int, M> p{};
        iota(p.begin(), p.end(), 0);
        swap(p[a], p[b]);
        applyPerm(t, p);
    }

    void rangeSwapValues(int l, int r, int a, int b) {
        TreapNode x, y, z;
        splitRange(l, r, x, y, z);
        applySwap(y, a, b);
        join(x, y, z);
    }

    // Swaps two non-overlapping ranges.
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
        if (i == j)return;
        swapRanges(i, i, j, j);
    }

    // Returns the first index in [l, r] with a[i] >= x, or -1.
    int firstAtLeast(int l, int r, int x) {
        TreapNode a, b, c;
        splitRange(l, r, a, b, c);
        int ans = firstGE(b, x);
        join(a, b, c);
        return ans == -1 ? -1 : l + ans;
    }

    // Returns the first index in [l, r] with a[i] <= x, or -1.
    int firstAtMost(int l, int r, int x) {
        TreapNode a, b, c;
        splitRange(l, r, a, b, c);
        int ans = firstLE(b, x);
        join(a, b, c);
        return ans == -1 ? -1 : l + ans;
    }

    // Returns the last index in [l, r] with a[i] >= x, or -1.
    int lastAtLeast(int l, int r, int x) {
        TreapNode a, b, c;
        splitRange(l, r, a, b, c);
        int ans = lastGE(b, x);
        join(a, b, c);
        return ans == -1 ? -1 : l + ans;
    }

    // Returns the last index in [l, r] with a[i] <= x, or -1.
    int lastAtMost(int l, int r, int x) {
        TreapNode a, b, c;
        splitRange(l, r, a, b, c);
        int ans = lastLE(b, x);
        join(a, b, c);
        return ans == -1 ? -1 : l + ans;
    }

    // Checks whether an id still exists.
    bool alive(int id) {
        return 0 < id and id < byId.size() and byId[id];
    }

    // Returns the current index of an id, or -1 if erased.
    int getPosById(int id) {
        if (!alive(id))return -1;
        auto t = byId[id];
        pushPath(t);
        int pos = size(t->l);
        while (t->p) {
            auto p = t->p;
            if (t == p->r)
                pos += size(p->l) + 1;
            t = p;
        }
        return pos;
    }

    int getById(int id) {
        if (!alive(id))return 0;
        pushPath(byId[id]);
        return byId[id]->val;
    }

    void setById(int id, int val) {
        int pos = getPosById(id);
        if (~pos)set(pos, val);
    }

    void eraseById(int id) {
        int pos = getPosById(id);
        if (~pos)erase(pos);
    }

    // Returns the whole array in O(n).
    vector<int> toVector() {
        vector<int> a;
        a.reserve(size());
        collect(root, a);
        return a;
    }

    void print() {
        for (auto x: toVector())
            cout << x << ' ';
        cout << '\n';
    }
};

const int mod = 1e9 + 7;

int exp(int base, int pow) {
    if (pow < 0)return 0;
    int res = 1;
    while (pow) {
        if (pow & 1) res = (res * base) % mod;
        base = (base * base) % mod, pow >>= 1;
    }
    return res;
}

void solve() {
    int n, q;
    string s;
    cin >> n >> q >> s;
    vector<int> v;
    for (auto c: s)
        v.push_back(c - 'a');

    vector<int> fact(n + q + 1, 1), inv(n + q + 1, 1);
    for (int i = 1; i <= n + q; ++i) {
        fact[i] = fact[i - 1] * i % mod;
        inv[i] = exp(fact[i], mod - 2);
    }
    Treap T(v);
    while (q--) {
        int t, l, r, idx;
        char a, b;
        cin >> t;
        if (t == 1) {
            cin >> l >> r;
            --l, --r;
            auto frq = T.rangeFrq(l, r);
            int answer = 1, cnt = 0, sum = 0;
            for (int i = 0; i < M; ++i) {
                cnt += frq[i] & 1;
                sum += frq[i] / 2;
                answer = answer * inv[frq[i] / 2] % mod;
            }
            if (cnt > 1)
                answer = 0;
            answer = answer * fact[sum] % mod;
            cout << answer << '\n';
        } else if (t == 2) {
            cin >> a >> idx;
            --idx;
            T.insert(idx, a - 'a');
        } else if (t == 3) {
            cin >> idx;
            --idx;
            T.erase(idx);
        } else if (t == 4) {
            cin >> a >> b >> l >> r;
            --l, --r;
            T.rangeSwapValues(l, r, a - 'a', b - 'a');
        } else {
            cin >> l >> r;
            --l, --r;
            T.rangeReverse(l, r);
        }
    }
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


// merge
struct Info {
    int sum = 0, prefix = INT_MIN, suffix = INT_MIN, ans = INT_MIN;
};

Info mergeInfo(const Info &a, const Info &b) {
    if (a.ans == INT_MIN)return b;
    if (b.ans == INT_MIN)return a;
    Info c;
    c.sum = a.sum + b.sum;
    c.prefix = max(a.prefix, a.sum + b.prefix);
    c.suffix = max(b.suffix, b.sum + a.suffix);
    c.ans = max({a.ans, b.ans, a.suffix + b.prefix});
    return c;
}

void pull(TreapNode t) {
    if (!t)return;
    t->sz = size(t->l) + size(t->r) + 1;

    Info cur{t->val, t->val, t->val, t->val};

    if (t->l) {
        Info lef{t->l->sum, t->l->prefix, t->l->suffix, t->l->ans};
        cur = mergeInfo(lef, cur);
    }

    if (t->r) {
        Info rig{t->r->sum, t->r->prefix, t->r->suffix, t->r->ans};
        cur = mergeInfo(cur, rig);
    }

    t->sum = cur.sum;
    t->prefix = cur.prefix;
    t->suffix = cur.suffix;
    t->ans = cur.ans;

    if (t->l)t->l->p = t;
    if (t->r)t->r->p = t;
}
#include<bits/stdc++.h>
using namespace std;
#define int long long

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
const int N = 2e5 + 5, inf = 1e18, M = 1;
const int mod[M] = {1000000009}, B[M] = {351};
int power[M][N];

struct Node {
    char val;
    int size = 1, priority = rng();
    array<int, M> hash{}, rev_hash{};
    bool reverse = false, replace_flag = false;
    char replace_val = 0;
    Node *left = nullptr, *right = nullptr;

    Node(char _val) : val(_val) {
        int x = _val;
        for (int i = 0; i < M; ++i) hash[i] = rev_hash[i] = x;
    }
};

using TreapNode = Node *;

struct Treap {
    TreapNode root = nullptr;

    ~Treap() { clear(root); }

    void clear(TreapNode t) {
        if (!t) return;
        clear(t->left);
        clear(t->right);
        delete t;
    }

    int size(TreapNode t) { return t ? t->size : 0; }

    array<int, M> get_hash(TreapNode t) { return t ? t->hash : array < int, M > {}; }

    array<int, M> get_rev_hash(TreapNode t) { return t ? t->rev_hash : array < int, M > {}; }

    void apply_replace(TreapNode t) {
        if (!t || !t->replace_flag) return;
        propagate(t->left);
        propagate(t->right);
        int c = t->replace_val;
        for (int i = 0; i < M; ++i) {
            int mod_val = mod[i];
            t->hash[i] = t->rev_hash[i] =
                    (c * (power[i][t->size] - 1 + mod_val) % mod_val * ((mod_val + 1) / 2)) %
                    mod_val;
        }
        t->val = t->replace_val;
        if (t->left) {
            t->left->replace_flag = true;
            t->left->replace_val = t->replace_val;
        }
        if (t->right) {
            t->right->replace_flag = true;
            t->right->replace_val = t->replace_val;
        }
        t->replace_flag = false;
    }

    void propagate(TreapNode t) {
        if (!t) return;
        apply_replace(t);
        if (t->reverse) {
            swap(t->left, t->right);
            swap(t->hash, t->rev_hash);
            if (t->left) t->left->reverse ^= true;
            if (t->right) t->right->reverse ^= true;
            t->reverse = false;
        }
    }

    void update(TreapNode t) {
        if (!t) return;
        propagate(t->left);
        propagate(t->right);
        int lsz = size(t->left), rsz = size(t->right);
        t->size = lsz + rsz + 1;
        for (int i = 0; i < M; ++i) {
            int mod_val = mod[i];
            int x = t->val;
            int lh = t->left ? t->left->hash[i] : 0;
            int rh = t->right ? t->right->hash[i] : 0;
            int lr = t->left ? t->left->rev_hash[i] : 0;
            int rr = t->right ? t->right->rev_hash[i] : 0;
            t->hash[i] =
                    (lh + x * power[i][lsz] % mod_val + rh * power[i][lsz + 1] % mod_val) % mod_val;
            t->rev_hash[i] =
                    (rr + x * power[i][rsz] % mod_val + lr * power[i][rsz + 1] % mod_val) % mod_val;
        }
    }

    void split(TreapNode t, TreapNode &l, TreapNode &r, int k, int add = 0) {
        if (!t) return void(l = r = nullptr);
        propagate(t);
        int idx = add + size(t->left);
        if (idx <= k) split(t->right, t->right, r, k, idx + 1), l = t;
        else split(t->left, l, t->left, k, add), r = t;
        update(t);
    }

    void merge(TreapNode &t, TreapNode l, TreapNode r) {
        propagate(l);
        propagate(r);
        if (!l || !r) t = l ? l : r;
        else if (l->priority > r->priority) merge(l->right, l->right, r), t = l;
        else merge(r->left, l, r->left), t = r;
        update(t);
    }

    void insert(int pos, char c) {
        TreapNode L, R;
        TreapNode mid = new Node(c);
        split(root, L, R, pos - 1);
        merge(L, L, mid);
        merge(root, L, R);
    }

    void delete_at(int pos) {
        if (pos < 0 || pos >= size(root)) return;
        TreapNode L, Mid, R;
        split(root, L, R, pos - 1);  // L = [0..pos-1], R = [pos..end]
        split(R, Mid, R, 0);         // Mid = [pos], R = [pos+1..end]
        delete Mid;
        merge(root, L, R);
    }

    void cyclic_shift(int l, int r, int k, bool left_shift = false) {
        if (l > r || l < 0 || r >= size(root)) return;
        k %= (r - l + 1);
        if (k == 0) return;
        TreapNode L, R, Mid, F, S;
        split(root, L, R, l - 1);
        split(R, Mid, R, r - l);
        if (!left_shift) {
            split(Mid, F, S, (r - l + 1) - k);
            merge(Mid, S, F);
        } else {
            split(Mid, F, S, k);
            merge(Mid, S, F);
        }
        merge(R, Mid, R);
        merge(root, L, R);
    }

    void cut_insert(int i, int j, int k) {
        TreapNode L, Mid, R, T;
        split(root, L, R, i - 1);
        split(R, Mid, R, j - i);
        merge(T, L, R);
        split(T, L, R, k - 1);
        merge(L, L, Mid);
        merge(root, L, R);
    }

    void reverse(int l, int r) {
        TreapNode L, R, Mid;
        split(root, L, R, l - 1);
        split(R, Mid, R, r - l);
        if (Mid) Mid->reverse ^= true;
        merge(R, Mid, R);
        merge(root, L, R);
    }

    array<int, M> getHash(int l, int r) {
        if (l > r || l < 0 || r >= size(root)) return array < int, M > {};
        TreapNode L, Mid, R;
        split(root, L, R, l - 1);  // L = [0..l-1], R = [l..end]
        split(R, Mid, R, r - l);   // Mid = [l..r], R = [r+1..end]
        propagate(Mid);
        array<int, M> res = get_hash(Mid);
        merge(R, Mid, R);
        merge(root, L, R);
        return res;
    }

    array<int, M> getReverseHash(int l, int r) {
        if (l > r || l < 0 || r >= size(root)) return array < int, M > {};
        TreapNode L, Mid, R;
        split(root, L, R, l - 1);
        split(R, Mid, R, r - l);
        propagate(Mid);
        array<int, M> res = get_rev_hash(Mid);
        merge(R, Mid, R);
        merge(root, L, R);
        return res;
    }

    void print(TreapNode t) {
        if (!t) return;
        propagate(t);
        print(t->left);
        cout << t->val;
        print(t->right);
    }

    bool is_palindrome(int l, int r) {
        TreapNode L, R, Mid;
        split(root, L, R, l - 1);
        split(R, Mid, R, r - l);
        propagate(Mid);
        bool ok = true;
        for (int i = 0; i < M; ++i) ok &= (Mid->hash[i] == Mid->rev_hash[i]);
        merge(R, Mid, R);
        merge(root, L, R);
        return ok;
    }
};

void precompute() {
    for (int i = 0; i < M; ++i) {
        power[i][0] = 1;
        for (int j = 1; j < N; ++j) power[i][j] = power[i][j - 1] * B[i] % mod[i];
    }
}

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
    Treap treap;
    for (int i = 0; i < n; ++i) treap.insert(i, s[i]);
    vector <vector<int>> divs(n + 1);
    for (int i = 1; i <= n; ++i)
        for (int j = i; j <= n; j += i) divs[j].emplace_back(i);
    auto lp = linear_sieve(s.size());
    auto minPeriod = [&](int l, int r) -> long long {
        if ((l == r) || treap.getHash(l, r - 1) == treap.getHash(l + 1, r)) return 1;
        int len = (r - l + 1), ans = len;
        while (len > 1) {
            if (treap.getHash(l, r - ans / lp[len]) == treap.getHash(l + ans / lp[len], r))
                ans /= lp[len];
            len /= lp[len];
        }
        return ans;
    };
    set <array<int, M>> st;
    while (q--) {
        int l, r;
        cin >> l >> r, --l, --r;
        int minP = minPeriod(l, r);
        auto hash = treap.getHash(l, l + minP - 1);
        if (st.count(hash)) continue;
        st.insert(hash);
        treap.reverse(l, r);
    }
    treap.print(treap.root);
}
//  precompute();
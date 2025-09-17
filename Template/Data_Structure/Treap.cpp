#include<bits/stdc++.h>
using namespace std;
#define int long long

mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());

// Beware!!!here Treap is 0-indexed
struct Node {
    int val = 0, size = 0, priority = 0, lazy = 0, max_val = 0;
    int min_val = 0, replace = 0, sum = 0;
    bool replace_flag = false, reverse = false;
    Node *left = nullptr, *right = nullptr, *parent = nullptr;

    Node() = default;

    explicit Node(int _val) {
        val = sum = max_val = min_val = _val;
        size = 1;
        priority = rnd();
        left = right = parent = nullptr;
    }
};

typedef Node *TreapNode;

struct Treap {
    TreapNode root{};
    map<int, TreapNode> position;  // positions of all the values
    bool exist = false;

    void clear() {
        delete_nodes(root);
        root = nullptr;
        position.clear();
        exist = false;
    }

    Treap() { clear(); }

    static int size(TreapNode t) { return t ? t->size : 0; }

    static void update_size(TreapNode &t) {
        if (t) t->size = size(t->left) + size(t->right) + 1;
    }

    static void update_parent(TreapNode &t) {
        if (!t) return;
        if (t->left) t->left->parent = t;
        if (t->right) t->right->parent = t;
    }

    static void update_sum(TreapNode &t) {
        if (!t or !t->lazy) return;
        t->sum += t->lazy * size(t);
        t->val += t->lazy;
        t->max_val += t->lazy;
        t->min_val += t->lazy;
        if (t->left) t->left->lazy += t->lazy;
        if (t->right) t->right->lazy += t->lazy;
        t->lazy = 0;
    }

    // replace update
    static void apply_replace(TreapNode &t) {
        if (!t or !t->replace_flag) return;
        t->val = t->max_val = t->min_val = t->replace;
        t->sum = t->val * size(t);
        if (t->left) {
            t->left->replace = t->replace;
            t->left->replace_flag = true;
        }
        if (t->right) {
            t->right->replace = t->replace;
            t->right->replace_flag = true;
        }
        t->replace_flag = false;
        t->replace = 0;
    }

    // reverse update
    static void apply_reverse(TreapNode &t) {
        if (!t or !t->reverse) return;
        t->reverse = false;
        swap(t->left, t->right);
        if (t->left) t->left->reverse ^= true;
        if (t->right) t->right->reverse ^= true;
    }

    // reset the value of current node assuming it now
    // represents a single element of the array
    static void reset(TreapNode &t) {
        if (!t) return;
        t->sum = t->val;
        t->max_val = t->val;
        t->min_val = t->val;
    }

    // perform all operations
    void operation(TreapNode &t) {
        if (!t) return;
        apply_reverse(t);
        apply_replace(t);
        update_sum(t);
        recalculate(t);
    }

    // split node t in l and r by key k
    void split(TreapNode t, TreapNode &l, TreapNode &r, int k, int add = 0) {
        if (t == nullptr) {
            l = nullptr;
            r = nullptr;
            return;
        }
        operation(t);
        int idx = add + size(t->left);
        if (t->left) t->left->parent = nullptr;
        if (t->right) t->right->parent = nullptr;
        if (idx <= k) split(t->right, t->right, r, k, idx + 1), l = t;
        else split(t->left, l, t->left, k, add), r = t;
        update_parent(t);
        update_size(t);
        operation(t);
    }

    // merge node l with r in t
    void merge(TreapNode &t, TreapNode l, TreapNode r) {
        operation(l);
        operation(r);
        if (!l) {
            t = r;
            return;
        }
        if (!r) {
            t = l;
            return;
        }
        if (l->priority > r->priority) merge(l->right, l->right, r), t = l;
        else merge(r->left, l, r->left), t = r;
        update_parent(t);
        update_size(t);
        operation(t);
    }

    void recalculate(TreapNode &t) {
        if (!t) return;
        t->size = 1 + size(t->left) + size(t->right);
        int lsum = t->left ? t->left->sum : 0;
        int rsum = t->right ? t->right->sum : 0;
        int lsize = size(t->left);
        if (lsize % 2 == 0) t->sum = lsum + t->val - rsum;
        else t->sum = lsum - t->val + rsum;
    }

    // returns index of node curr
    int get_pos(TreapNode curr, TreapNode son = nullptr) {
        bool exists = true;
        if (curr == nullptr) {
            exists = false;
            return 0;
        }
        if (!son) {
            if (curr == root) return size(curr->left);
            else return size(curr->left) + get_pos(curr->parent, curr);
        }
        if (curr == root) {
            if (son == curr->left) return 0;
            else return size(curr->left) + 1;
        }
        if (curr->left == son) return get_pos(curr->parent, curr);
        else return get_pos(curr->parent, curr) + size(curr->left) + 1;
    }

    void delete_nodes(TreapNode t) {
        if (!t) return;
        delete_nodes(t->left);
        delete_nodes(t->right);
        position.erase(t->val);
        delete t;
    }

    // insert val in position a[pos]
    void insert(int pos, int val) {
        if (root == nullptr) {
            auto to_add = new Node(val);
            root = to_add;
            position[val] = root;
            return;
        }
        TreapNode l, r, mid;
        mid = new Node(val);
        position[val] = mid;
        split(root, l, r, pos - 1);
        merge(l, l, mid);
        merge(root, l, r);
    }

    // erase from qL to qR indexes
    void erase(int qL, int qR) {
        TreapNode l, r, mid;
        split(root, l, r, qL - 1);
        split(r, mid, r, qR - qL);
        delete_nodes(mid);
        merge(root, l, r);
    }

    // returns answer for corresponding types of query [sum, max, min]
    int query(int qL, int qR) {
        TreapNode l, r, mid;
        split(root, l, r, qL - 1);
        split(r, mid, r, qR - qL);
        recalculate(root);
        int answer = mid->sum;
        merge(r, mid, r);
        merge(root, l, r);
        recalculate(root);
        return answer;
    }

    // add val in all the values from a[qL] to a[qR] positions
    void update(int qL, int qR, int val) {
        TreapNode l, r, mid;
        split(root, l, r, qL - 1);
        split(r, mid, r, qR - qL);
        mid->lazy += val;
        merge(r, mid, r);
        merge(root, l, r);
    }

    // reverse all the values from qL to qR
    void reverse(int qL, int qR) {
        TreapNode l, r, mid;
        split(root, l, r, qL - 1);
        split(r, mid, r, qR - qL);
        mid->reverse ^= 1;
        merge(r, mid, r);
        merge(root, l, r);
    }

    // replace all the values from a[qL] to a[qR] by v
    void replace(int qL, int qR, int v) {
        TreapNode l, r, mid;
        split(root, l, r, qL - 1);
        split(r, mid, r, qR - qL);
        mid->replace_flag = true;
        mid->replace = v;
        merge(r, mid, r);
        merge(root, l, r);
    }

    // it will cyclic right shift the array k times
    void cyclic_shift(int qL, int qR, int k, bool left_shift = false) {
        if (qL == qR) return;
        k %= (qR - qL + 1);
        TreapNode l, r, mid, fh, sh;
        split(root, l, r, qL - 1);
        split(r, mid, r, qR - qL);
        if (!left_shift) split(mid, fh, sh, (qR - qL + 1) - k - 1);
        else split(mid, fh, sh, k - 1);
        merge(mid, sh, fh);
        merge(r, mid, r);
        merge(root, l, r);
    }

    // returns index of the value
    int get_pos(int value) {
        if (position.find(value) == position.end()) return -1;
        int x = get_pos(position[value]);
        return x;
    }

    //  access index in the array
    int get_val(int pos) { return query(pos, pos); }

    int size() { return size(root); }

    bool find(int val) { return position.count(val) > 0; }

    void print(TreapNode t) {
        if (!t) return;
        propagate(t);
        print(t->left);
        cout << t->val;
        print(t->right);
    }
};

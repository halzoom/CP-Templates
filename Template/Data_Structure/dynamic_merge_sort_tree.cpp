#include<bits/stdc++.h>
using namespace std;
#define int long long

/*
 * You have an array a of size N and two types of query to process.
 * 1. change the value at position x to v.
 * 2. count number of element less than k from l to r
 * */
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace __gnu_pbds;
using namespace std;
template<class T>
using ordered_set =
        tree<T, null_type, less_equal<T>, rb_tree_tag, tree_order_statistics_node_update>;

// order_of_key(k): Gives the count of elements smaller than k. - O(log n)
// find_by_order(k): Returns the iterator for the kth element (use k = 0 for the
// first element). - O(log n)0
//  9
struct Node {
    ordered_set<int> v;

    Node() {};

    Node(int x) { v.insert(x); };

    void erase(int x) { v.erase(v.find_by_order(v.order_of_key(x))); }
};

struct MergeTree {
#define LNodeIDX (2 * node + 1)
#define RNodeIDX (2 * node + 2)
#define mid ((l + r) / 2)
    vector<Node> Tree;
    int SegSize = 1;
    Node defVal = 0;

    // 7
    Node Merge(Node &a, Node &b) {
        Node res;
        for (auto i: a.v) res.v.insert(i);
        for (auto i: b.v) res.v.insert(i);
        return res;
    }

    // 9
    void build(int l, int r, int node, vector<int> &a) {
        if (l == r) {
            if (l < a.size()) Tree[node] = a[l];
            return;
        }
        build(l, mid, LNodeIDX, a);
        build(mid + 1, r, RNodeIDX, a);
        Tree[node] = Merge(Tree[LNodeIDX], Tree[RNodeIDX]);
    }

    // 11
    void update(int l, int r, int node, int idx, int oldV,
                int newV) {  // O(log^2(n))
        Tree[node].erase(oldV);
        Tree[node].v.insert(newV);
        if (l == r) return;
        if (idx <= mid) update(l, mid, LNodeIDX, idx, oldV, newV);
        else update(mid + 1, r, RNodeIDX, idx, oldV, newV);
    }

    // 11
    int query(int l, int r, int node, int Lx, int Rx, int v) {  // O(log^2(n))
        if (Lx > r or Rx < l) return 0;
        if (Lx <= l and r <= Rx) return (int) Tree[node].v.order_of_key(v + 1);
        int L = query(l, mid, LNodeIDX, Lx, Rx, v);
        int R = query(mid + 1, r, RNodeIDX, Lx, Rx, v);
        return L + R;
    }

public:
    // 5
    MergeTree(vector<int> &a) {
        int n = (int) a.size();
        while (SegSize < n) SegSize *= 2;
        Tree.assign(2 * SegSize, defVal);
        build(0, SegSize - 1, 0, a);
    }

    // 3
    void update(int idx, int oldV, int newV) { update(0, SegSize - 1, 0, idx, oldV, newV); }

    // 3
    int query(int l, int r, int v) { return query(0, SegSize - 1, 0, l, r, v); }
};

// 24
signed main() {
    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    MergeTree tree(a);
    while (q--) {
        char t;
        int l, r;
        cin >> t >> l >> r, --l;
        if (t == '!') {
            tree.update(l, a[l], r);
            a[l] = r;
        } else {
            int v;
            cin >> v;
            --r;
            cout << tree.query(l, r, v) << '\n';
        }
    }
    return 0;
}

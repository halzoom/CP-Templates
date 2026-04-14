#include<bits/stdc++.h>

using namespace std;
#define int long long

/*
 * Your task is to maintain an array of n values and efficiently process the
 * following types of queries:
 * 1. Increase each value in range [a,b] by x.
 * 2. Set each value in range [a,b] to x.
 * 3. Calculate the sum of values in range [a,b].
 * */
struct Node {
    int v = 0, lazyAdd = 0, lazyAssign = 0;
    int isLazyAdd = 0, isLazyAssign = 0;

    Node() {};

    Node(int x) : v(x) {};

    void add(int val, int l, int r) {
        v += val * (r - l + 1);
        lazyAdd += val;
        isLazyAdd = 1;
    }

    void assign(int val, int l, int r) {
        v = val * (r - l + 1);
        lazyAssign = val;
        isLazyAssign = 1;
        lazyAdd = 0;
        isLazyAdd = 0;
    }
};

struct LazySegmentTree {
#define LNodeIDX (2 * node + 1)
#define RNodeIDX (2 * node + 2)
#define mid ((l + r) / 2)
private:
    vector<Node> Tree;
    int SegSize = 1;
    Node defVal = 0;

    Node merge(Node &a, Node &b) {
        Node res;
        res.v = a.v + b.v;
        return res;
    }

    void build(int l, int r, int node, vector<int> &a) {
        if (l == r) {
            if (l < a.size()) Tree[node] = Node(a[l]);
            return;
        }
        build(l, mid, LNodeIDX, a);
        build(mid + 1, r, RNodeIDX, a);
        Tree[node] = merge(Tree[LNodeIDX], Tree[RNodeIDX]);
    }

    void propagate(int node, int l, int r) {
        if (l == r) return;
        if (Tree[node].isLazyAssign) {
            Tree[LNodeIDX].assign(Tree[node].lazyAssign, l, mid);
            Tree[RNodeIDX].assign(Tree[node].lazyAssign, mid + 1, r);
        }
        if (Tree[node].isLazyAdd) {
            Tree[LNodeIDX].add(Tree[node].lazyAdd, l, mid);
            Tree[RNodeIDX].add(Tree[node].lazyAdd, mid + 1, r);
        }
        Tree[node].isLazyAdd = Tree[node].isLazyAssign = 0;
        Tree[node].lazyAdd = Tree[node].lazyAssign = 0;
    }

    void update(int l, int r, int node, int Lx, int Rx, int val, int t) {
        propagate(node, l, r);
        if (Lx > r or Rx < l) return;
        if (Lx <= l and r <= Rx) {
            if (t == 1) {
                Tree[node].isLazyAdd = 1;
                Tree[node].add(val, l, r);
            } else {
                Tree[node].isLazyAssign = 1;
                Tree[node].assign(val, l, r);
            }
            return;
        }
        update(l, mid, LNodeIDX, Lx, Rx, val, t);
        update(mid + 1, r, RNodeIDX, Lx, Rx, val, t);
        Tree[node] = merge(Tree[LNodeIDX], Tree[RNodeIDX]);
    }

    Node query(int l, int r, int node, int Lx, int Rx) {
        propagate(node, l, r);
        if (Lx > r or Rx < l) return defVal;
        if (Lx <= l and r <= Rx) return Tree[node];
        Node L = query(l, mid, LNodeIDX, Lx, Rx);
        Node R = query(mid + 1, r, RNodeIDX, Lx, Rx);
        return merge(L, R);
    }

public:
    LazySegmentTree(int n) {
        while (SegSize < n) SegSize *= 2;
        Tree.assign(2 * SegSize, defVal);
    }

    void build(vector<int> &a) { build(0, SegSize - 1, 0, a); }

    void update(int l, int r, int val, int t) { update(0, SegSize - 1, 0, l, r, val, t); }

    Node query(int l, int r) { return query(0, SegSize - 1, 0, l, r); }
};
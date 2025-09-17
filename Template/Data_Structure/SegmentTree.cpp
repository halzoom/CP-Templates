#include<bits/stdc++.h>
using namespace std;
#define int long long

/*
 * There is an array consisting of n integers. Some values of the array will be
 * updated, and after each update, your task is to report the maximum subarray
 * sum in the array.
 * */
struct Node {
    int Suffix = 0, Prefix = 0, maxV = 0, Sum = 0;

    Node() {};

    Node(int v) {
        Sum = v;
        Suffix = Prefix = maxV = max(v, 0LL);
    };
};

struct SegmentTree {
#define LNodeIDX (2 * node + 1)
#define RNodeIDX (2 * node + 2)
#define mid ((l + r) / 2)
private:
    vector <Node> Seg;
    int SegSize = 1;
    Node difVal;

    Node Merge(Node a, Node b) {
        Node res;
        res.Sum = a.Sum + b.Sum;
        res.Prefix = max(a.Prefix, a.Sum + b.Prefix);
        res.Suffix = max(b.Suffix, b.Sum + a.Suffix);
        res.maxV = max({res.Sum, a.maxV, b.maxV, a.Suffix + b.Prefix});
        return res;
    }

    void build(int l, int r, int node, vector<int> &a) {
        if (l == r) {
            if (r < a.size()) Seg[node] = Node(a[r]);
            return;
        }
        build(l, mid, LNodeIDX, a);
        build(mid + 1, r, RNodeIDX, a);
        Seg[node] = Merge(Seg[LNodeIDX], Seg[RNodeIDX]);
    }

    void update(int l, int r, int node, int idx, int Value) {
        if (l == r) {
            Seg[node] = Node(Value);
            return;
        }
        if (mid >= idx) update(l, mid, LNodeIDX, idx, Value);
        else update(mid + 1, r, RNodeIDX, idx, Value);
        Seg[node] = Merge(Seg[LNodeIDX], Seg[RNodeIDX]);
    }

    Node query(int l, int r, int node, int Lx, int Rx) {
        if (l > Rx or r < Lx) return difVal;
        if (l >= Lx and r <= Rx) return Seg[node];
        Node Left = query(l, mid, LNodeIDX, Lx, Rx);
        Node Right = query(mid + 1, r, RNodeIDX, Lx, Rx);
        return Merge(Left, Right);
    }

public:
    SegmentTree(vector<int> &a) {
        int n = (int) a.size();
        SegSize = 1;
        while (SegSize < n) SegSize *= 2;
        Seg.assign(2 * SegSize, difVal);
        build(0, SegSize - 1, 0, a);
    }

    void update(int idx, int Value) { update(0, SegSize - 1, 0, idx, Value); }

    Node query(int l, int r) { return query(0, SegSize - 1, 0, l, r); }

#undef LNodeIDX
#undef RNodeIDX
#undef mid
};

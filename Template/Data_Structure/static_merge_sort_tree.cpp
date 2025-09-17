#include<bits/stdc++.h>
using namespace std;
#define int long long

/*
 * You have an array a of size n and q queries
 * for each query calculate number of elements grater than k from l to r
 * */
struct Node {
    vector<int> v;

    Node() {};

    Node(int x) { v.push_back(x); };
};

struct MergeTree {
#define LNodeIDX (2 * node + 1)
#define RNodeIDX (2 * node + 2)
#define mid ((l + r) / 2)
#define all(v) v.begin(), v.end()
    vector <Node> Tree;
    int SegSize = 1;
    Node defVal = 0;

    Node Merge(Node &a, Node &b) {
        Node res;
        res.v.assign(a.v.size() + b.v.size(), {});
        merge(all(a.v), all(b.v), res.v.begin());
        return res;
    }

    void build(int l, int r, int node, vector<int> &a) {
        if (l == r) {
            if (l < a.size()) Tree[node] = a[l];
            return;
        }
        build(l, mid, LNodeIDX, a);
        build(mid + 1, r, RNodeIDX, a);
        Tree[node] = Merge(Tree[LNodeIDX], Tree[RNodeIDX]);
    }

    int query(int l, int r, int node, int Lx, int Rx, int k) {
        if (Lx > r or Rx < l) return 0;
        if (Lx <= l and r <= Rx) {
            int g =
                    upper_bound(Tree[node].v.begin(), Tree[node].v.end(), k) - Tree[node].v.begin();
            return Tree[node].v.size() - g;
        }
        int L = query(l, mid, LNodeIDX, Lx, Rx, k);
        int R = query(mid + 1, r, RNodeIDX, Lx, Rx, k);
        return L + R;
    }

public:
    MergeTree(vector<int> &a) {
        int n = (int) a.size();
        while (SegSize < n) SegSize *= 2;
        Tree.assign(2 * SegSize, defVal);
        build(0, SegSize - 1, 0, a);
    }

    int query(int l, int r, int k) { return query(0, SegSize - 1, 0, l, r, k); }
};
#include<bits/stdc++.h>
using namespace std;
#define int long long

// Given n ranges, your task is to count for each range how many other ranges it contains and how many other ranges contain it.
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace __gnu_pbds;
template<class T> using ordered_set =
        tree<T, null_type, less_equal < T>, rb_tree_tag, tree_order_statistics_node_update>;

// order_of_key(k): Gives the count of elements smaller than k. - O(log n)
// find_by_order(k): Returns the iterator for the kth element (use k = 0 for the first element). - O(log n)0
struct Range {
    int l, r, idx;

    bool operator<(Range &other) {
        if (other.l == l) return r > other.r;
        return l < other.l;
    }
};

void solve() {
    int n;
    cin >> n;
    vector <Range> ranges(n);
    for (int i = 0; i < n; ++i) {
        cin >> ranges[i].l >> ranges[i].r;
        ranges[i].idx = i;
    }
    sort(ranges.begin(), ranges.end());
    vector<int> contains(n), contained(n);
    ordered_set<int> maxR, minR;
    for (int i = 0; i < n; ++i) {
        contains[ranges[i].idx] = maxR.size() - maxR.order_of_key(ranges[i].r);
        maxR.insert(ranges[i].r);
    }
    for (int i = n - 1; ~i; --i) {
        contained[ranges[i].idx] = minR.order_of_key(ranges[i].r + 1);
        minR.insert(ranges[i].r);
    }
    for (auto i: contained) cout << i << ' ';
    cout << '\n';
    for (auto i: contains) cout << i << ' ';
}
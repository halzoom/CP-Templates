#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace __gnu_pbds;
using namespace std;

template<class T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
// order_of_key(k): Gives the count of elements smaller than k. - O(log n)
// find_by_order(k): Returns the iterator for the kth element (use k = 0 for the
// first element). - O(log n)0
const int inf = 1e9;


void solve() {
    int n;
    cin >> n;
    vector<int> a(n + 1);
    vector<int> prefix(n + 1), suffix(n + 1);
    for (int i = 1; i <= n; ++i)
        cin >> a[i];
    prefix = suffix = a;
    for (int i = 2; i <= n; ++i)
        prefix[i] = min(prefix[i], prefix[i - 1]);
    for (int i = n - 1; i; --i)
        suffix[i] = max(suffix[i], suffix[i + 1]);


    for (int i = 1; i < n; ++i) {
        if (prefix[i] > suffix[i + 1])
            return void(cout << "NO\n");
    }
    cout << "YES\n";
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int test = 1;
    cin >> test;

    for (int i = 1; i <= test; ++i) {
        solve();
    }
    return 0;
}
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace __gnu_pbds;
using namespace std;
#define int long long
template<class T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
// order_of_key(k): Gives the count of elements smaller than k. - O(log n)
// find_by_order(k): Returns the iterator for the kth element (use k = 0 for the
// first element). - O(log n)0

void solve() {
    int n, m;
    cin >> n >> m;
    ordered_set<int> st;
    for (int i = 1; i <= n; i++)
        st.insert(i);
    int pos = 0;
    for (int i = 1; i <= n; i++) {
        pos = (pos + m) % (n - i + 1);
        auto it = st.find_by_order(pos);
        cout << *it << " ";
        st.erase(it);
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
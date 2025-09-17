#include<bits/stdc++.h>
using namespace std;
#define int long long

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace __gnu_pbds;
template<class T>
using ordered_set =
        tree<T, null_type, less < T>, rb_tree_tag, tree_order_statistics_node_update>;
// order_of_key(k): Gives the count of elements smaller than k. - O(log n)
// find_by_order(k): Returns the iterator for the kth element (use k = 0 for the
// first element). - O(log n)0
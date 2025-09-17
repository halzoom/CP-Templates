#include<bits/stdc++.h>
using namespace std;
#define int long long

/*
 * For each k=1,...,N, solve the following problem: A has N - k+1 (contiguous)
 * subarrays of length k. Take the maximum of each of them, and output the sum
 * of these maxima.
 * */
// #include "sparse_table.h"
void solve() {
    int n;
    cin >> n;
    vector <pair<int, int>> a(n + 1);
    for (int i = 1; i <= n; ++i) cin >> a[i].first, a[i].second = i;
    SparseTable sp(a, [&](pair<int, int> x, pair<int, int> y) { return max(x, y); });
    vector<int> answer(n + 2);
    function<int(int, int)> go = [&](int l, int r) -> int {
        if (l > r) return 0;
        auto [M, idx] = sp.get(l, r);
        int L = go(l, idx - 1);
        int R = go(idx + 1, r);
        if (L > R) swap(L, R);
        for (int i = 0; i <= L; ++i) {
            answer[i + 1] += M;
            answer[i + R + 2] -= M;
        }
        return L + R + 1;
    };
    go(1, n);
    for (int i = 1; i <= n; ++i) answer[i] += answer[i - 1];
    for (int i = 1; i <= n; ++i) cout << answer[i] << '\n';
}
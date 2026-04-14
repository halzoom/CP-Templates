#include <bits/stdc++.h>

using namespace std;
#define int long long

struct DSU {
    vector<int> parent, size;

    DSU(int n) {
        parent.assign(n + 1, {});
        size.assign(n + 1, 1);
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (x == parent[x]) return x;
        return parent[x] = find(parent[x]);
    }

    bool union_set(int a, int b) {
        a = find(a), b = find(b);
        if (a == b) return false;
        if (size[a] < size[b]) swap(a, b);
        parent[b] = a;
        size[a] += size[b];
        return true;
    }

    bool isConnected(int a, int b) { return find(a) == find(b); }
};


void solve() {
    int n, m, q;
    cin >> n >> m >> q;
    vector<array<int, 3>> edges(m);
    for (int i = 0; i < m; ++i)
        cin >> edges[i][0] >> edges[i][1] >> edges[i][2];

    sort(edges.begin(), edges.end(), [&](array<int, 3> &A, array<int, 3> &B) {
        return A[2] > B[2];
    });
    vector<int> left(q, 0), right(q, m - 1), answer(q);
    vector<array<int, 2>> query(q);
    vector<vector<int>> has(m);
    for (int i = 0; i < q; ++i)
        cin >> query[i][0] >> query[i][1];

    auto add = [&]() {
        for (int i = 0; i < q; ++i)
            if (left[i] <= right[i])
                has[(left[i] + right[i]) / 2].emplace_back(i);
    };
    int k = 23;
    while (k--) {
        add();
        DSU T(n);
        for (int mid = 0; mid < m; ++mid) {
            T.union_set(edges[mid][0], edges[mid][1]);
            for (auto j: has[mid]) {
                if (T.isConnected(query[j][0], query[j][1]))
                    answer[j] = edges[mid][2], right[j] = mid - 1;
                else
                    left[j] = mid + 1;
            }
            has[mid].clear();
        }
    }
    for (int i = 0; i < q; ++i)
        cout << answer[i] << '\n';
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
#include <bits/stdc++.h>

using namespace std;
#define int long long

// O(V^3)
const int inf = 2e15;

struct Hungarian {
    int n, m;
    vector<int> u, v, p, way;
    vector<vector<int>> A;

    Hungarian(int n, int m) :
            n(n), m(m), u(n + 1), v(m + 1), p(m + 1),
            way(m + 1), A(n + 1, vector(m + 1, inf)) {}

    void addEdge(int x, int y, int w) { A[x][y] = min(A[x][y], w); }

    array<int, 2> build() {
        for (int i = 1; i <= n; ++i) {
            p[0] = i;
            int j0 = 0;
            vector minv(m + 1, inf);
            vector used(m + 1, false);

            do {
                used[j0] = true;
                int i0 = p[j0], delta = inf, j1 = 0;
                for (int j = 1; j <= m; ++j)
                    if (!used[j]) {
                        int cur = A[i0][j] - u[i0] - v[j];
                        if (cur < minv[j]) minv[j] = cur, way[j] = j0;
                        if (minv[j] < delta) delta = minv[j], j1 = j;
                    }

                if (delta == inf) {
                    j0 = 0;
                    break;
                }

                for (int j = 0; j <= m; ++j)
                    if (used[j]) u[p[j]] += delta, v[j] -= delta;
                    else minv[j] -= delta;
                j0 = j1;
            } while (p[j0] != 0);


            do {
                int j1 = way[j0];
                p[j0] = p[j1];
                j0 = j1;
            } while (j0);
        }

        int match = 0, cost = 0;
        for (int j = 1; j <= m; ++j)
            if (p[j] && A[p[j]][j] < inf) match++, cost += A[p[j]][j];

        cout << cost << '\n';
        for (int j = 1; j <= m; ++j)
            cout << p[j] << ' ' << j << '\n';

        return {match, cost};
    }
};

void solve() {
    int n;
    cin >> n;

    Hungarian T(n, n);
    for (int i = 1; i <= n; ++i) {
        for (int j = 1, w; j <= n; ++j) {
            cin >> w;
            T.addEdge(i, j, w);
        }
    }

    T.build();
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
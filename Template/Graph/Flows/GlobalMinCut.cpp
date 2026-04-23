#include <bits/stdc++.h>

using namespace std;
#define int long long
const int inf = 1e18;
// Stoer–Wagner global min‐cut on an undirected weighted graph.
// n = number of vertices(0 indexed), weight[u][v] = weight of edge u–v (0 if none).
// Returns the weight of the minimum cut separating the vertices into two nonempty sets.
int stoer_wagner(int n, vector<vector<int>> &weight) {
    vector<bool> alive(n, true);
    vector<int> best(n), dist(n);
    int min_cut = inf;

    for (int phase = n; phase > 1; --phase) {
        fill(dist.begin(), dist.end(), 0);
        vector<bool> added(n, false);
        int prev = -1;
        for (int i = 0; i < phase; ++i) {
            int pick = -1;
            for (int j = 0; j < n; ++j) {
                if (alive[j] && !added[j] && (pick == -1 || dist[j] > dist[pick]))
                    pick = j;
            }
            added[pick] = true;
            if (i == phase - 1) {
                if (dist[pick] < min_cut)
                    min_cut = dist[pick];
                for (int v = 0; v < n; ++v) {
                    weight[prev][v] += weight[pick][v];
                    weight[v][prev] = weight[prev][v];
                }
                alive[pick] = false;
            } else {
                prev = pick;
                for (int v = 0; v < n; ++v) {
                    if (alive[v] && !added[v])
                        dist[v] += weight[pick][v];
                }
            }
        }
    }
    return min_cut;
}

pair<int, vector<int>> globalMinCut(vector<vector<int>> mat) {
    pair<int, vector<int>> best = {inf, {}};
    int n = mat.size();
    vector<vector<int>> co(n);
    for (int i = 0; i < n; i++) co[i] = {i};

    for (int ph = 1; ph < n; ph++) {
        vector<int> w = mat[0];
        size_t s = 0, t = 0;
        for (int it = 0; it < n - ph; it++) {
            w[t] = -inf;
            s = t, t = max_element(w.begin(), w.end()) - w.begin();
            for (int i = 0; i < n; i++) w[i] += mat[t][i];
        }
        best = min(best, {w[t] - mat[t][t], co[t]});

        co[s].insert(co[s].end(), co[t].begin(), co[t].end());
        for (int i = 0; i < n; i++) mat[s][i] += mat[t][i];
        for (int i = 0; i < n; i++) mat[i][s] = mat[s][i];
        mat[0][t] = -inf;
    }
    return best;
}

void solve() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> mat(n, vector<int>(n));
    for (int i = 0, u, v, w; i < m; ++i) {
        cin >> u >> v >> w, --u, --v;
        mat[u][v] = mat[v][u] = w;
    }

    cout << stoer_wagner(n, mat) << '\n';
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
#ifdef HALZOOM
    freopen("Input.txt", "r", stdin);
    freopen("Output.txt", "w", stdout);
#endif

    int test = 1;
    cin >> test;

    for (int i = 1; i <= test; ++i) {
        cout << "Case #" << i << ": ";
        solve();
    }
    return 0;
}
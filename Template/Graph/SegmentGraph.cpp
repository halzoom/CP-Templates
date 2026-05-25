#include <bits/stdc++.h>

using namespace std;
#define int long long
const int inf = 1e18;

struct SegmentGraph {
    int n, size;
    vector<vector<pair<int, int>>> adj;

    SegmentGraph(int _n) : n(_n) {
        size = 1;
        while (size < n) size <<= 1;
        adj.resize(4 * size + 5);
        build(1, 1, size);
    }

    void add_edge(int u, int v, int w) {
        adj[size + u - 1].emplace_back(size + v - 1, w);
    }

    void add_edge_to_range(int u, int l, int r, int w) {
        if (l > r) return;
        add_edge_to_range(u, 1, 1, size, l, r, w);
    }

    void add_edge_from_range(int u, int l, int r, int w) {
        if (l > r) return;
        add_edge_from_range(u, 1, 1, size, l, r, w);
    }

    vector<int> dijkstra(int s) {
        vector<int> dist(adj.size(), inf);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
        s = size + s - 1;
        dist[s] = 0;
        pq.emplace(0, s);
        while (!pq.empty()) {
            auto [weight, u] = pq.top();
            pq.pop();

            if (weight > dist[u]) continue;
            for (auto [v, w]: adj[u]) {
                if (dist[v] > weight + w) {
                    dist[v] = weight + w;
                    pq.emplace(dist[v], v);
                }
            }
        }
        vector<int> res(n + 1);
        for (int i = 1; i <= n; ++i) {
            int leaf = size + i - 1;
            res[i] = (dist[leaf] >= inf ? -1 : dist[leaf]);
        }
        return res;
    }

    void build(int node, int lx, int rx) {
        if (lx == rx) {
            if (lx <= n) {
                adj[node].emplace_back(node + 2 * size, 0);
                adj[node + 2 * size].emplace_back(node, 0);
            }
            return;
        }

        int mid = lx + (rx - lx) / 2;
        int left = 2 * node;
        int right = 2 * node + 1;

        adj[node].emplace_back(left, 0);
        adj[node].emplace_back(right, 0);

        adj[left + 2 * size].emplace_back(node + 2 * size, 0);
        adj[right + 2 * size].emplace_back(node + 2 * size, 0);

        build(left, lx, mid);
        build(right, mid + 1, rx);
    }

    void add_edge_to_range(int u, int node, int lx, int rx, int ql, int qr, int w) {
        if (ql <= lx and rx <= qr) {
            adj[size + u - 1].emplace_back(node, w);
            return;
        }
        int mid = lx + (rx - lx) / 2;
        if (ql <= mid) add_edge_to_range(u, 2 * node, lx, mid, ql, qr, w);
        if (qr > mid) add_edge_to_range(u, 2 * node + 1, mid + 1, rx, ql, qr, w);
    }

    void add_edge_from_range(int u, int node, int lx, int rx, int ql, int qr, int w) {
        if (ql <= lx and rx <= qr) {
            adj[node + 2 * size].emplace_back(size + u - 1, w);
            return;
        }
        int mid = lx + (rx - lx) / 2;
        if (ql <= mid) add_edge_from_range(u, 2 * node, lx, mid, ql, qr, w);
        if (qr > mid) add_edge_from_range(u, 2 * node + 1, mid + 1, rx, ql, qr, w);
    }
};

void solve() {
    int n, q, s;
    cin >> n >> q >> s;
    SegmentGraph T(n);
    for (int i = 0, t; i < q; ++i) {
        cin >> t;
        if (t == 1) {
            int u, v, w;
            cin >> u >> v >> w;
            T.add_edge(u, v, w);
        } else if (t == 2) {
            int v, l, r, w;
            cin >> v >> l >> r >> w;
            T.add_edge_to_range(v, l, r, w);
        } else {
            int v, l, r, w;
            cin >> v >> l >> r >> w;
            T.add_edge_from_range(v, l, r, w);
        }
    }

    auto answer = T.dijkstra(s);
    for (int i = 1; i <= n; ++i)
        cout << answer[i] << ' ';
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
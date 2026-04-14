#include <bits/stdc++.h>

using namespace std;
#define int long long

const long long inf = 1e18;

vector<int> Dijkstra(int start, vector<vector<pair<int, int>>> &adj) {
    int n = adj.size();
    vector<int> dist(n, inf);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    dist[start] = 0;
    pq.push({0, start});
    while (!pq.empty()) {
        auto [W, u] = pq.top();
        pq.pop();
        if (dist[u] < W) continue;
        for (auto [node, Weight]: adj[u]) {
            int newW = W + Weight;
            if (newW < dist[node]) {
                dist[node] = newW;
                pq.push({newW, node});
            }
        }
    }
    return dist;
}

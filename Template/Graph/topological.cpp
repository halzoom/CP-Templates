#include <bits/stdc++.h>

using namespace std;

vector<int> topological(vector<vector<int>> &adj) {
    int n = adj.size() - 1;
    vector<int> in(n + 1), topo;
    for (int u = 1; u <= n; ++u)
        for (auto v: adj[u])
            in[v]++;
    queue<int> q;
    for (int i = 1; i <= n; ++i)
        if (in[i] == 0)
            q.emplace(i), topo.emplace_back(i);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (auto v: adj[u])
            if (--in[v] == 0)
                q.push(v), topo.emplace_back(v);
    }
    return topo;
}
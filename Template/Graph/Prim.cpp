#include<bits/stdc++.h>

using namespace std;
#define int long long

int Prim(int start, vector<vector<pair<int, int>>> &adj) {
    int n = (int) adj.size();
    vector<bool> inMST(n);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    int sum = 0;
    pq.emplace(0, start);
    while (!pq.empty()) {
        auto [w, u] = pq.top();
        pq.pop();

        if (inMST[u]) continue;
        inMST[u] = true;
        sum += w;
        for (auto [weight, v]: adj[u]) {
            if (inMST[v]) continue;
            pq.emplace(weight, v);
        }
    }
    return sum;
}

int Prim(int start, vector<vector<pair<int, int>>> &adj) {
    int n = (int) adj.size();
    vector<bool> inMST(n);
    priority_queue<array<int, 3>, vector<array<int, 3>>, greater<>> pq;
    vector<vector<pair<int, int>>> MST(n + 1);
    int sum = 0;
    pq.push({0, start, 0});
    while (!pq.empty()) {
        auto [w, u, p] = pq.top();
        pq.pop();

        if (inMST[u]) continue;
        if (p) {
            MST[u].emplace_back(p, w);
            MST[p].emplace_back(u, w);
        }
        inMST[u] = true;
        sum += w;
        for (auto [v, weight]: adj[u]) {
            if (inMST[v]) continue;
            pq.push({weight, v, u});
        }
    }
    adj = MST;
    return sum;
}

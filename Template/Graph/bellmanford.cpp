#include<bits/stdc++.h>
using namespace std;
#define int long long

const ll oo = 1e18;
struct edge {
    ll x, y, cost;
};
vector <edge> edges;
vector <ll> dis;
vector<bool> in_negative_cycle;
vector <ll> v;

void bellman_ford(ll u) {
    dis.assign(n + 1, oo);
    in_negative_cycle.assign(n + 1, false);
    dis[u] = 0;
    for (int i = 0; i < n - 1; i++) {
        for (auto &e: edges) {
            if (dis[e.x] != oo && dis[e.x] + e.cost < dis[e.y]) {
                dis[e.y] = dis[e.x] + e.cost;
            }
        }
    }
    for (auto &e: edges) {
        if (dis[e.x] != oo && dis[e.x] + e.cost < dis[e.y]) { in_negative_cycle[e.y] = true; }
    }
    queue <ll> q;
    for (int i = 1; i <= n; i++) {
        if (in_negative_cycle[i]) q.push(i);
    }
    while (!q.empty()) {
        ll x = q.front();
        q.pop();
        for (auto e: edges) {
            if (e.x == x && !in_negative_cycle[e.y]) {
                in_negative_cycle[e.y] = true;
                q.push(e.y);
            }
        }
    }
}
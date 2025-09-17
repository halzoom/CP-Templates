#include<bits/stdc++.h>
using namespace std;
#define int long long

struct edge {
    ll to, cost;
};
vector <ll> dis;
vector <vector<edge>> adj;

void BFS_0_1(ll n) {
    dis[n] = 0;
    deque <ll> d;
    d.push_back(n);
    while (!d.empty()) {
        ll u = d.front();
        d.pop_front();
        for (auto &x: adj[u]) {
            if (dis[u] + x.cost < dis[x.to]) {
                dis[x.to] = dis[u] + x.cost;
                if (x.cost == 1) d.push_back(x.to);
                else d.push_front(x.to);
            }
        }
    }
}
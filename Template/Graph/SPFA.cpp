#include<bits/stdc++.h>
using namespace std;
#define int long long

const ll oo = 1e18;
struct edge {
    ll to, cost;
};
vector <vector<edge>> adj;
vector <ll> dis;
vector <ll> cnt;
vector<bool> in;

bool SPFA(ll u) {
    dis.assign(n + 1, oo);
    cnt.assign(n + 1, 0);
    in.assign(n + 1, false);
    queue <ll> q;
    q.push(u);
    dis[u] = 0;
    in[u] = true;
    while (!q.empty()) {
        ll x = q.front();
        q.pop();
        in[x] = false;
        for (auto &e: adj[x]) {
            if (dis[e.to] > dis[x] + e.cost) {
                dis[e.to] = dis[x] + e.cost;
                if (!in[e.to]) {
                    q.push(e.to);
                    in[e.to] = true;
                    cnt[e.to]++;
                    if (cnt[e.to] > n) return false;
                }
            }
        }
    }
    return true;
}

#include<bits/stdc++.h>
using namespace std;
#define int long long

vector <vector<ll>> dis;

void floyd_warshall() {
    for (int i = 1; i <= n; i++) dis[i][i] = 0;
    for (int k = 1; k <= n; k++) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (dis[i][k] < oo && dis[k][j] < oo)
                    dis[i][j] = dis[j][i] = min(dis[i][j], dis[i][k] + dis[k][j]);
            }
        }
    }
}
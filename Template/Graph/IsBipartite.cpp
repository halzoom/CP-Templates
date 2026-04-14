#include<bits/stdc++.h>
using namespace std;
#define int long long

int isBipartite(int n, vector <vector<int>> &graph) {
    int cnt = 0;
    vector<int> color(n + 1, -1);
    for (int start = 1; start <= n; ++start) {
        if (color[start] == -1) {
            cnt++;
            queue<int> q;
            q.push(start);
            color[start] = 0;
            while (!q.empty()) {
                int node = q.front();
                q.pop();
                for (int neighbor: graph[node]) {
                    if (color[neighbor] == -1) {
                        color[neighbor] = 1 - color[node];
                        q.push(neighbor);
                    } else if (color[neighbor] == color[node]) {
                        return 0;
                    }
                }
            }
        }
    }
    return cnt;
}

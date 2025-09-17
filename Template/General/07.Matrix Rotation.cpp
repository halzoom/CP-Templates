#include<bits/stdc++.h>
using namespace std;
#define int long long

// 7
vector <vector<int>> rotateClockWise(vector <vector<int>> &g) {
    int n = g.size(), m = g[0].size();
    vector res(m, vector<int>(n));
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j) res[i][j] = g[n - j - 1][i];
    return res;
}

// 7
vector <vector<int>> rotateCounterClockwise(vector <vector<int>> &g) {
    if (g.empty() || g[0].empty()) return {};  // Handle empty matrix
    int n = g.size(), m = g[0].size();
    vector <vector<int>> res(m, vector<int>(n));
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j) res[i][j] = g[j][m - i - 1];
    return res;
}
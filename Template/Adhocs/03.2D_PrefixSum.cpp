#include<bits/stdc++.h>
using namespace std;
#define int long long

// 11
void PrefixSum_2D(int n, int m, vector <vector<int>> &Prefix) {
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) { Prefix[i][j] += Prefix[i][j - 1]; }
    }
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) { Prefix[j][i] += Prefix[j - 1][i]; }
    }
}

int getSum(int i, int j, int x, int y) {
    return Prefix[x][y] - Prefix[i - 1][y] - Prefix[x][j - 1] + Prefix[i - 1][j - 1];
}

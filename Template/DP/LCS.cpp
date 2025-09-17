#include <bits/stdc++.h>

using namespace std;
#define int long long
const int N = 1e5 + 5;
int n, m ,  mem[N][N];
string s, t;
vector<vector<int>> nxt;
vector<vector<bool>> vis;
int dp(int idx, int sz) {
    if (!sz) return -1;
    if (idx < 0)return n;

    int &res = mem[idx][sz];
    if (vis[idx][sz])return res;
    vis[idx][sz] = true;
    int leave = dp(idx - 1, sz);
    int take = dp(idx - 1, sz - 1);
    take = nxt[take + 1][t[idx] - 'a'];
    return res = min(take, leave);
}

void solve() {
    // min(n * n , m * m)
    // dp[idx][sz] min IDX in s when match string of size sz
    cin >> s >> t;
    n = s.size();
    m = t.size();
    vis.assign(m + 1, vector<bool>(m + 1, false));
    nxt.assign(n + 2, vector<int>(26, n));
    for (int i = n - 1; i >= 0; --i) {
        nxt[i] = nxt[i + 1];
        nxt[i][s[i] - 'a'] = i;
    }
    for (int i = m; i >= 0; --i) {
        if (dp(m - 1, i) < n) {
            cout << i << '\n';
            return;
        }
    }
}
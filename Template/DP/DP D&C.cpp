#include <bits/stdc++.h>

using namespace std;

const int N = 35005, inf = 1e9;
int dp[N][2], it = 1, L = 1, R = 0, sum = 0;
int a[N];
deque<int> idx[N];

void add(int i) {
    int v = a[i];
    if (!idx[v].empty())
        sum -= idx[v].back() - idx[v].front();

    if (idx[v].empty() or idx[v].front() > i)idx[v].emplace_front(i);
    else idx[v].emplace_back(i);

    sum += idx[v].back() - idx[v].front();
}

void rem(int i) {
    int v = a[i];
    sum -= idx[v].back() - idx[v].front();

    if (idx[v].back() == i)idx[v].pop_back();
    else idx[v].pop_front();

    if (!idx[v].empty())
        sum += idx[v].back() - idx[v].front();
}

void move(int l, int r) {
    while (R < r) add(++R);
    while (L > l) add(--L);
    while (R > r) rem(R--);
    while (L < l) rem(L++);
}

void go(int l, int r, int lx, int rx) {
    if (l > r) return;

    int m = (l + r) / 2, opt = 1;
    dp[m][it] = inf;
    for (int i = lx; i <= min(rx, m); ++i) {
        move(i, m);
        int curr = dp[i - 1][it ^ 1] + sum;
        if (curr < dp[m][it])
            dp[m][it] = curr, opt = i;
    }
    go(l, m - 1, lx, opt);
    go(m + 1, r, opt, rx);
}

// check this
// a < b < c < d
// maximization cost(a,c)+cost(b,d) >= cost(a,d)+cost(b,c)
// minimization cost(a,c)+cost(b,d) <= cost(a,d)+cost(b,c)
void solve() {
    int n, k;
    cin >> n >> k;
    for (int i = 1; i <= n; ++i)
        cin >> a[i];

    memset(dp, '?', sizeof dp);
    it = 1;
    dp[0][0] = 0;
    for (int i = 1; i <= k; ++i, it ^= 1) {
        go(1, n, 1, n);
    }

    cout << dp[n][k & 1];
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
#ifdef HALZOOM
    freopen("Input.txt", "r", stdin);
    freopen("Output.txt", "w", stdout);
#endif

    int test = 1;
//    cin >> test;

    for (int i = 1; i <= test; ++i) {
        solve();
    }
    return 0;
}
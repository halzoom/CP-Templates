#include <bits/stdc++.h>

using namespace std;
#define int long long
const int mod = 1e9 + 7, N = 1e6 + 6;
vector<vector<int>> dp;
int m;


struct CartesianTree {
    int n, root;
    vector<int> par, lf, rt;
    CartesianTree() {};
    CartesianTree(const vector<int> &a) {
        n = a.size() - 1;
        par.assign(n + 1, {});
        lf.assign(n + 1, {});
        rt.assign(n + 1, {});
        auto CMP = [&](int i, int j) {
            return a[i] < a[j]; // max
            return a[i] > a[j]; // min
        };
        stack<int> st;
        for (int i = 1; i <= n; i++) {
            int last = 0;
            while (!st.empty() and CMP(st.top(), i))
                last = st.top(), st.pop();

            if (!st.empty()) {
                rt[st.top()] = i;
                par[i] = st.top();
            }

            if (last) {
                lf[i] = last;
                par[last] = i;
            }
            st.push(i);
        }

        root = 0;
        for (int i = 1; i <= n; i++)
            if (par[i] == 0) root = i;
    }
} T;

void go(int u) {
    if (T.lf[u] == 0 and T.rt[u] == 0) {
        dp[u].assign(m + 1, 1);
        dp[u][0] = 0;
        return;
    }
    if (T.lf[u]) go(T.lf[u]);
    if (T.rt[u]) go(T.rt[u]);
    int sumL = T.lf[u] ? 0 : 1, sumR = T.rt[u] ? 0 : 1;
    for (int i = 1; i <= m; ++i) {
        sumR = (sumR + dp[T.rt[u]][i]) % mod;
        sumL = (sumL + dp[T.lf[u]][i - 1]) % mod;
        dp[u][i] = (dp[u][i] + sumL * sumR) % mod;
    }
}

void solve() {
    int n;
    cin >> n >> m;
    dp.assign(n + 1, vector<int>(m + 1));
    T.lf.assign(n + 1, {});
    vector<int> a(n + 1);
    for (int i = 1; i <= n; ++i)
        cin >> a[i];

    T = CartesianTree(a);
    go(T.root);
    int answer = 0;
    for (int i = 1; i <= m; ++i)
        answer = (answer + dp[T.root][i]) % mod;
    cout << answer << '\n';
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
#ifdef HALZOOM
    freopen("Input.txt", "r", stdin);
    freopen("Output.txt", "w", stdout);
#endif

    int test = 1;
    cin >> test;

    for (int i = 1; i <= test; ++i) {
        solve();
    }
    return 0;
}
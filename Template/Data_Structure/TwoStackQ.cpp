#include <bits/stdc++.h>

using namespace std;
#define int long long
const int inf = 1e18;

struct TwoStackQ {
    struct Node {
        int mx = -inf, mn = inf, val;

        Node() : val(0) {}

        Node(int x) : mx(x), mn(x), val(x) {}
    };

    stack<Node> a, b;

    int size() { return a.size() + b.size(); }

    void mrg(Node &a, Node &b) {
        a.mn = min(a.mn, b.mn);
        a.mx = max(a.mx, b.mx);
    }

    void push(int val) {
        auto nd = Node(val);
        if (!a.empty()) mrg(nd, a.top());
        a.push(nd);
    }

    void move() {
        while (!a.empty()) {
            auto nd = Node(a.top().val);
            if (!b.empty()) mrg(nd, b.top());
            b.push(nd), a.pop();
        }
    }

    Node get() {
        Node res;
        if (!b.empty()) mrg(res, b.top());
        if (!a.empty()) mrg(res, a.top());
        return res;
    }

    void pop() {
        if (b.empty()) move();
        if (!b.empty()) b.pop();
    }
};

void solve() {
    int n, m, d;
    cin >> n >> m >> d;
    vector<int> dp(n + 1);
    int t = 1;
    for (int i = 0, a, b, ti; i < m; ++i) {
        cin >> a >> b >> ti;
        TwoStackQ T;
        vector<int> cur_dp(n + 1, -inf);
        int k = min(n, d * (ti - t)), r = 1, l = 1;
        for (int j = 1; j <= n; ++j) {
            while (r <= n and r <= j + k)
                T.push(dp[r++]);
            while (l < j - k)
                T.pop(), ++l;
            cur_dp[j] = b - abs(a - j) + T.get().mx;
        }

        swap(dp, cur_dp);
        t = ti;
    }
    cout << *max_element(dp.begin(), dp.end()) << '\n';
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
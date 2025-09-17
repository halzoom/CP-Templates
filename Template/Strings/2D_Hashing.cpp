#include<bits/stdc++.h>
using namespace std;
#define int long long

// Find all occurrence of a 2d word in a 2d word.
const int N = 2005, M = 2005;
int baseX, baseY, mod, powX[N], powY[M];

struct Hashing {
    vector <vector<int>> hash;
    int n, m;

    // 19
    void build() {
        if (mod) return;
        mt19937 rng(chrono::system_clock::now().time_since_epoch().count());
        auto rnd = [&](int a, int b) { return a + rng() % (b - a + 1); };
        auto check = [&](int x) {
            for (int i = 2; i * i <= x; ++i)
                if (x % i == 0) return true;
            return false;
        };
        baseX = rnd(130, 500);
        baseY = rnd(130, 500);
        mod = rnd(1e9, 2e9);
        while (check(mod)) mod--;
        powX[0] = powY[0] = 1;
        for (int i = 0; i <= 2000; i++) powX[i + 1] = 1LL * powX[i] * baseX % mod;
        for (int i = 0; i <= 2000; i++) powY[i + 1] = 1LL * powY[i] * baseY % mod;
    }

    // 20
    Hashing(vector <string> &s) {
        build();
        n = (int) s.size(), m = (int) s[0].size();
        hash.assign(n + 1, vector<int>(m + 1, 0));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) { hash[i + 1][j + 1] = s[i][j]; }
        }
        for (int i = 0; i <= n; i++) {
            for (int j = 0; j < m; j++) {
                hash[i][j + 1] = (hash[i][j + 1] + 1LL * hash[i][j] * baseY % mod) % mod;
            }
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j <= m; j++) {
                hash[i + 1][j] = (hash[i + 1][j] + 1LL * hash[i][j] * baseX % mod) % mod;
            }
        }
    }

    // 9
    int get_hash(int x1, int y1, int x2, int y2) {  // 1-indexed
        assert(1 <= x1 && x1 <= x2 && x2 <= n);
        assert(1 <= y1 && y1 <= y2 && y2 <= m);
        x1--;
        y1--;
        int dx = x2 - x1, dy = y2 - y1;
        return (1LL * (hash[x2][y2] - 1LL * hash[x2][y1] * powY[dy] % mod + mod) % mod -
                1LL * (hash[x1][y2] - 1LL * hash[x1][y1] * powY[dy] % mod + mod) % mod *
                powX[dx] % mod +
                mod) %
               mod;
    }

    // 3
    int get_hash() { return get_hash(1, 1, n, m); }
};

// 9
void PartialSum_in2D(int x1, int y1, int x2, int y2, vector <vector<int>> &Prefix) {
    if (x1 > x2) swap(x1, x2);
    if (y1 > y2) swap(y1, y2);
    Prefix[x1][y1]++;
    Prefix[x2 + 1][y1]--;
    Prefix[x1][y2 + 1]--;
    Prefix[x2 + 1][y2 + 1]++;
}

// 9
void PrefixSum_2D(int n, int m, vector <vector<int>> &Prefix) {
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= m; ++j) Prefix[i][j] += Prefix[i][j - 1];
    for (int i = 1; i <= m; ++i)
        for (int j = 1; j <= n; ++j) Prefix[j][i] += Prefix[j - 1][i];
}

// 30
signed main() {
    int n, m;
    cin >> n >> m;
    vector <string> word(n);
    for (int i = 0; i < n; ++i) cin >> word[i];
    int X, Y;
    cin >> X >> Y;
    vector <string> grid(X);
    for (int i = 0; i < X; ++i) cin >> grid[i];
    Hashing hash(word);
    int pattern = hash.get_hash();
    hash = Hashing(grid);
    vector <vector<int>> res(X + 2, vector<int>(Y + 2));
    for (int i = 1; i + n - 1 <= X; ++i)
        for (int j = 1; j + m - 1 <= Y; ++j)
            if (pattern == hash.get_hash(i, j, i + n - 1, j + m - 1))
                PartialSum_in2D(i, j, i + n - 1, j + m - 1, res);
    PrefixSum_2D(X, Y, res);
    for (int i = 1; i <= X; ++i) {
        for (int j = 1; j <= Y; ++j) cout << (res[i][j] ? grid[i - 1][j - 1] : '.');
        cout << '\n';
    }
    return 0;
}
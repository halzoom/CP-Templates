#include <bits/stdc++.h>

using namespace std;
#define int long long

struct BIT2D {
    int n;
    vector<vector<int>> bit;

    BIT2D(int _n) {
        n = _n;
        bit.assign(n + 1, vector<int>(n + 1, 0));
    }

    void update(int x, int y, int val) {
        for (int i = x; i <= n; i += i & -i) {
            for (int j = y; j <= n; j += j & -j) {
                bit[i][j] += val;
            }
        }
    }

    int get(int x, int y) {
        int sum = 0;
        for (int i = x; i > 0; i -= i & -i)
            for (int j = y; j > 0; j -= j & -j)
                sum += bit[i][j];
        return sum;
    }

    int get(int x1, int y1, int x2, int y2) {
        return get(x2, y2) - get(x1 - 1, y2) - get(x2, y1 - 1) + get(x1 - 1, y1 - 1);
    }
};

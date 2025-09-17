#include<bits/stdc++.h>

using namespace std;
#define int long long

template<typename T, class CMP = function<T(const T &, const T &)>>
class SparseTable2D {
public:
    int n = 0, m = 0, Kx = 0, Ky = 0;
    vector<vector<vector<vector<T>>>> sp;
    CMP func;

    SparseTable2D(const vector<vector<T>> &mat, CMP f) : func(f) {
        n = mat.size();
        m = mat[0].size();
        Kx = 32 - __builtin_clz(n);
        Ky = 32 - __builtin_clz(m);

        sp.assign(Kx, vector<vector<vector<T>>>(Ky));
        for (int kx = 0; kx < Kx; ++kx) {
            for (int ky = 0; ky < Ky; ++ky) {
                int rows = n - (1 << kx) + 1;
                int cols = m - (1 << ky) + 1;
                if (rows <= 0 || cols <= 0) continue;
                sp[kx][ky].assign(rows, vector<T>(cols));
                if (kx == 0 and ky == 0) {
                    for (int i = 0; i < n; ++i)
                        for (int j = 0; j < m; ++j)
                            sp[0][0][i][j] = mat[i][j];
                } else if (kx == 0) {
                    for (int i = 0; i < rows; ++i) {
                        for (int j = 0; j < cols; ++j) {
                            sp[0][ky][i][j] =
                                    func(sp[0][ky - 1][i][j],
                                         sp[0][ky - 1][i][j + (1 << (ky - 1))]);
                        }
                    }
                } else {
                    for (int i = 0; i < rows; ++i) {
                        for (int j = 0; j < cols; ++j) {
                            sp[kx][ky][i][j] =
                                    func(sp[kx - 1][ky][i][j],
                                         sp[kx - 1][ky][i + (1 << (kx - 1))][j]);
                        }
                    }
                }
            }
        }
    }

    T query(int x1, int y1, int x2, int y2) { // overlapping
        if (x1 > x2)swap(x1, x2);
        if (y1 > y2)swap(y1, y2);
        int lx = x2 - x1 + 1;
        int ly = y2 - y1 + 1;
        int kx = 31 - __builtin_clz(lx), ky = 31 - __builtin_clz(ly);
        int ix2 = x2 - (1 << kx) + 1, jy2 = y2 - (1 << ky) + 1;

        T &a = sp[kx][ky][x1][y1];
        T &b = sp[kx][ky][ix2][y1];
        T &c = sp[kx][ky][x1][jy2];
        T &d = sp[kx][ky][ix2][jy2];
        return func(func(a, b), func(c, d));
    }

    T _query(int x1, int y1, int x2, int y2) { // no overlapping
        if (x1 > x2)swap(x1, x2);
        if (y1 > y2)swap(y1, y2);
        int lx = x2 - x1 + 1;
        int ly = y2 - y1 + 1;
        int kx = 31 - __builtin_clz(lx), ky = 31 - __builtin_clz(ly);
        int hx = 1 << kx, hy = 1 << ky;

        T &a = sp[kx][ky][x1][y1];
        T &b = sp[kx][ky][x2 - hx + 1][y1];
        T &c = sp[kx][ky][x1][y2 - hy + 1];
        T &d = sp[kx][ky][x2 - hx + 1][y2 - hy + 1];
        return func(func(a, b), func(c, d));
    }
};
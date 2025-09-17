#include <bits/stdc++.h>

using namespace std;

#define int long long

const int limit = 1e6 + 1;
using cd = complex<double>;
const double PI = acos(-1);

// Fast Fourier Transform
void fft(vector<cd> &a, bool invert) {
    int n = a.size();
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }

    for (int len = 2; len <= n; len <<= 1) {
        double ang = 2 * PI / len * (invert ? -1 : 1);
        cd wlen(cos(ang), sin(ang));
        for (int i = 0; i < n; i += len) {
            cd w(1);
            for (int j = 0; j < len / 2; j++) {
                cd u = a[i + j], v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }

    if (invert) {
        for (cd &x: a) x /= n;
    }
}

// Polynomial multiplication using FFT
vector<int> multiply(const vector<int> &a, const vector<int> &b) {
    vector<cd> fa(a.begin(), a.end()), fb(b.begin(), b.end());
    int n = 1;
    while (n < (int) (a.size() + b.size())) n <<= 1;
    fa.resize(n);
    fb.resize(n);

    fft(fa, false);
    fft(fb, false);
    for (int i = 0; i < n; i++) fa[i] *= fb[i];
    fft(fa, true);

    vector<int> result(n);
    for (int i = 0; i < n; i++) {
        result[i] = llround(fa[i].real());
    }

    result.resize(min((int) (a.size() + b.size()), limit));
    return result;
}

// Polynomial exponentiation with a limit on size
vector<int> poly_pow(vector<int> poly, int p, int limit = 1e9) {
    vector<int> ans{1};
    while (p) {
        if (p & 1) ans = multiply(ans, poly);
        poly = multiply(poly, poly);
        ans.resize(limit + 1);
        poly.resize(limit + 1);
        p >>= 1;
    }
    return ans;
}

// Alternative polynomial exponentiation
vector<int> power(vector<int> &a, int b) {
    vector<int> res = {1};
    while (b) {
        if (b & 1) res = multiply(res, a);
        a = multiply(a, a);
        b >>= 1;
    }
    return res;
}

// Multiply multiple polynomials using a priority queue (optimal order)
vector<int> multiply_moreThanOne(vector<vector<int>> polys) {
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    for (int i = 0; i < polys.size(); ++i) {
        pq.emplace(polys[i].size(), i);
    }

    while (pq.size() > 1) {
        int a = pq.top().second;
        pq.pop();
        int b = pq.top().second;
        pq.pop();
        auto res = multiply(polys[a], polys[b]);
        polys[a] = res;
        pq.emplace(res.size(), a);
    }

    return polys[pq.top().second];
}

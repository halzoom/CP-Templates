#include <bits/stdc++.h>

using namespace std;
const int lg = 32;

struct Basis {
    int size = 0;
    vector<int> basis, lst;

    Basis() {
        size = 0;
        basis.assign(lg, {});
        lst.assign(lg, -1);
    };

    bool on(int n, int bit) {
        return n >> bit & 1;
    }

    bool insert(int n, int idx) {
        for (int i = lg - 1; ~i; --i) {
            if (!on(n, i))continue;
            if (lst[i] < idx) {
                if (lst[i] == -1)size++;
                swap(basis[i], n);
                swap(idx, lst[i]);
            }
            n ^= basis[i];
        }
        return false;
    }

    int max_xor(int idx) {
        int answer = 0;
        for (int i = lg - 1; ~i; --i) {
            if (!basis[i] or lst[i] < idx or on(answer, i))continue;
            answer ^= basis[i];
        }
        return answer;
    }
};

void solve() {
    int n;
    cin >> n;
    vector<Basis> T(n + 1);
    Basis add;
    for (int i = 1, x; i <= n; ++i) {
        cin >> x;
        add.insert(x, i);
        T[i] = add;
    }
    int q;
    cin >> q;
    while (q--) {
        int l, r;
        cin >> l >> r;
        cout << T[r].max_xor(l) << '\n';
    }
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
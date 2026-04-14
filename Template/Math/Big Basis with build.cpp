#include <bits/stdc++.h>
using namespace std;
const int lg = 2000, M = 2000;
typedef bitset<M> bigInt;

struct Basis {
    int size = 0;
    vector<bigInt> basis, idx;
    vector<int> vis;

    Basis() {
        size = 0;
        basis.assign(lg, {});
        vis.assign(lg, {});
        idx.assign(lg, {});
    };

    bool on(bigInt n, int bit) {
        return n[bit];
    }

    pair<bool, bigInt> can(bigInt n) {
        bigInt usedIndices;
        for (int i = lg - 1; i >= 0; --i) {
            if (vis[i] && on(n, i)) {
                n ^= basis[i];
                usedIndices ^= idx[i];
            }
        }
        return {n == 0, usedIndices};
    }

    bool insert(bigInt n, int id) {
        bigInt add;
        add.set(id);

        for (int i = lg - 1; i >= 0; --i) {
            if (!on(n, i)) continue;
            if (!vis[i]) {
                vis[i] = 1;
                basis[i] = n;
                idx[i] = add;
                size++;
                return true;
            }
            n ^= basis[i];
            add ^= idx[i];
        }
        return false;
    }
};

bigInt stringToBitset(string s) {
    if (s == "0") return {};

    vector<uint32_t> chunks;
    for (int i = s.size(); i > 0; i -= 9) {
        if (i < 9) chunks.push_back(stoi(s.substr(0, i)));
        else chunks.push_back(stoi(s.substr(i - 9, 9)));
    }

    bigInt res;
    int bitPos = 0;
    while (!chunks.empty()) {
        uint64_t rem = 0;
        for (int i = (int) chunks.size() - 1; i >= 0; i--) {
            uint64_t cur = chunks[i] + rem * 1000000000ULL;
            chunks[i] = cur >> 1;
            rem = cur & 1;
        }

        if (rem) res.set(bitPos);
        bitPos++;
        while (!chunks.empty() && chunks.back() == 0) chunks.pop_back();
    }
    return res;
}

void solve() {
    int n;
    cin >> n;
    Basis T;
    for (int x = 0; x < n; ++x) {
        string m;
        cin >> m;
        bigInt add = stringToBitset(m);
        auto [ok, idx] = T.can(add);
        vector<int> answer;
        for (int i = 0; i < M; ++i)
            if (idx[i])
                answer.emplace_back(i);
        if (ok) {
            cout << answer.size() << ' ';
            for (auto i: answer)
                cout << i << ' ';
            cout << '\n';
        } else
            cout << 0 << '\n';
        T.insert(add, x);
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
#include <bits/stdc++.h>

using namespace std;
#define int long long
const int lg = 32;

struct Basis {
    int size = 0;
    vector<int> basis;

    Basis() {
        size = 0;
        basis.assign(lg, {});
    };

    bool on(int n, int bit) {
        return n >> bit & 1;
    }

    bool insert(int n) {
        for (int i = lg - 1; ~i; --i) {
            if (!on(n, i))continue;
            if (!basis[i]) {
                basis[i] = n;
                size++;
                return true;
            }
            n ^= basis[i];
        }
        return false;
    }

    bool can(int n) {
        for (int i = lg - 1; ~i; --i)
            if (basis[i] and on(n, i))
                n ^= basis[i];
        return n == 0;
    }

    int max_xor() {
        int answer = 0;
        for (int i = lg - 1; ~i; --i) {
            if (!basis[i] or on(answer, i))continue;
            answer ^= basis[i];
        }
        return answer;
    }

    int min_xor(int n) {
        for (int i = lg - 1; ~i; --i) {
            if (on(n, i) and basis[i])
                n ^= basis[i];
        }
        return n;
    }

    int Kth(int k) {
        int answer = 0, cnt = size;
        for (int i = lg - 1; ~i; --i) {
            if (!basis[i])continue;
            --cnt;
            if (on(answer, i)) {
                if ((1LL << cnt) >= k)answer ^= basis[i];
                else k -= 1LL << cnt;
            } else if ((1LL << cnt) < k) {
                answer ^= basis[i];
                k -= 1LL << cnt;
            }
        }
        return answer;
    }

    void andALL(int x) {
        vector<int> new_Biz;
        for (int i = lg - 1; i >= 0; --i) {
            basis[i] = (basis[i] & x);
            if (basis[i]) new_Biz.push_back(basis[i]);
            basis[i] = 0;
        }
        for (int i: new_Biz)
            insert(i);
    }

    bool merge(Basis &w) {
        bool repeat = false;
        for (auto biz: w.basis)
            if (biz and !insert(biz))
                repeat = true;
        return repeat;
    }
};
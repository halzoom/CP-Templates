#include <bits/stdc++.h>

using namespace std;

long long rnd(long long a, long long b) {
    if (a > b) return 0;
    return a + rand() % (b - a + 1);
}

void gen() {
    cout << 1 << '\n';

    int n = rnd(1e3, 1e5);

    cout << n << '\n';
    for (int i = 0; i < n; ++i) {
        int l = rnd(1, 1e9);
        int r = rnd(1, 1e9);
        if (l > r)swap(l, r);
        cout << l << ' ' << r << '\n';
    }

}

int32_t main(int argc, char *argv[]) {

    gen();

    return 0;
}

#include <bits/stdc++.h>

using namespace std;
mt19937 rng(chrono::system_clock::now().time_since_epoch().count());

int rnd(int a, int b) {
    if (a > b) return 0;
    return a + rng() % (b - a + 1);
}
int main(int argc, char *argv[]) {

    int maxT = 4000, maxS = 4000, maxK = 4000, sum = 5000;
    int n = rnd(1, maxK);
    cout << n << endl;
    for (int i = n; i; --i) {
        int m = rnd(1, min(maxS, sum - i - 1));
        sum -= m;
        for (int j = 0; j < m; ++j)
            cout << char('A' + rnd(0, 25));

        if (i > 1)
            cout << ' ';
        else
            cout << endl;
    }
    int T = rnd(1, min(maxT, sum));
    for (int i = 1; i <= T; ++i)
        cout << char('A' + rnd(0, 25));

    cout << endl;
}
#include <bits/stdc++.h>

using namespace std;
mt19937 rng(chrono::system_clock::now().time_since_epoch().count());

int rnd(int a, int b) {
    if (a > b) return 0;
    return a + rng() % (b - a + 1);
}

int main() {
    int n = rnd(1, 1000);
    vector<int> a(n);
    iota(a.begin(), a.end(), 1);
    shuffle(a.begin(), a.end(), rng);
    cout << 1 << '\n' << n << '\n';
    for(auto i : a)
        cout << i << ' ';
    return 0;
}
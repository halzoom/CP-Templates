#include<bits/stdc++.h>
using namespace std;
#define int long long

//5
mt19937 rng(chrono::system_clock::now().time_since_epoch().count());

int rnd(int a, int b) {
    if (a > b) return 0;
    return a + rng() % (b - a + 1);
}
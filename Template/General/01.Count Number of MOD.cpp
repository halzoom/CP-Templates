#include<bits/stdc++.h>
using namespace std;
#define int long long

// 5
int count_number_of_mod(int l, int r, int x, int mod) {
    // count number of y such that y%n = x , for y from l to r
    if (x >= mod) return 0;
    return (r - x + mod) / mod - (l - 1 - x + mod) / mod;
}
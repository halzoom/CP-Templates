#include<bits/stdc++.h>
using namespace std;
#define int long long

// 7
//  int how many ways you can get a permutation of size n such that pi != i
vector<int> Derangement(int n) {
    vector<int> D(n + 1, 0);
    D[2] = 1;
    for (int i = 3; i <= n; ++i) { D[i] = (i - 1) * (D[i - 1] + D[i - 2]) % mod; }
    return D;
}
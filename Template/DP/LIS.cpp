#include<bits/stdc++.h>
using namespace std;
#define int long long

// 12
//  s -> input array , LIS-> answer of lis ending at position i
void LIS(const vector<int> &S, vector<int> &LIS) {
    vector<int> L(S.size());
    int lisCount = 0;
    for (size_t i = 0; i < S.size(); ++i) {
        // if you need equal values change it to upper_bound
        int pos = lower_bound(L.begin(), L.begin() + lisCount, S[i]) - L.begin();
        L[pos] = S[i];
        if (pos == lisCount) ++lisCount;
        LIS[i] = pos + 1;
    }
}
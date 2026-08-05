#include <bits/stdc++.h>

using namespace std;
#define int long long

vector<int> LIS(vector<int> &a) {
    int n = a.size(), len = 0;
    vector<int> lis(n), res(n);
    for (int i = 0; i < n; ++i) {
        int pos = lower_bound(lis.begin(), lis.begin() + len, a[i]) - lis.begin();
        lis[pos] = a[i];
        if (pos == len)len++;
        res[i] = pos + 1;
    }
    return res;
}

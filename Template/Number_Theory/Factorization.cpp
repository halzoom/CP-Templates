#include<bits/stdc++.h>
using namespace std;
#define int long long

vector<int> Factorize(int n) {
    vector<int> a;
    for (int i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            a.push_back(i);
            if (i * i != n) a.push_back(n / i);
        }
    }
    sort(a.begin(), a.end());
    return a;
}
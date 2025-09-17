#include<bits/stdc++.h>
using namespace std;
#define int long long

vector <pair<int, int>> PrimeFact(int n) {
    vector <pair<int, int>> a;
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            int cnt = 0;
            while (n % i == 0) {
                cnt++;
                n /= i;
            }
            a.emplace_back(i, cnt);
        }
    }
    if (n > 1) a.emplace_back(n, 1);
    return a;
}
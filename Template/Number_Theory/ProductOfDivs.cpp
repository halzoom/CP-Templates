#include<bits/stdc++.h>
using namespace std;
#define int long long

int ProdOfDivs_FromPrimes(vector <pair<int, int>> primes) {
    int res = 1, d = 1;
    for (auto [pr, cnt]: primes) {
        int v = exp(pr, cnt * (cnt + 1) / 2);
        res = exp(res, cnt + 1) * exp(v, d) % mod;
        d = d * (cnt + 1) % (mod - 1);
    }
    return res;
}
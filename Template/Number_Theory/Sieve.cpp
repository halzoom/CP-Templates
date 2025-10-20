#include<bits/stdc++.h>
using namespace std;
#define int long long

vector<int> Sieve(int n) {
    vector<int> sieve(n + 1, -1);
    for (int i = 2; i <= n; ++i) {
        if (sieve[i] == -1)
            for (int j = i; j <= n; j += i)
                sieve[j] = i;
    }
    return sieve;
}
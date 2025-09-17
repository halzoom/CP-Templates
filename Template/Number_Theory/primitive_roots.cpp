#include<bits/stdc++.h>
using namespace std;
#define int long long

int exp(int Base, int Power, int n) {
    int Result = 1;
    while (Power) {
        if (Power & 1) Result = (Result * Base) % n;
        Base = (Base * Base) % n, Power >>= 1;
    }
    return Result;
}

int phi(int n) {
    int result = n;
    for (int p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            result -= result / p;
        }
    }
    if (n > 1) result -= result / n;
    return result;
}

vector<int> PrimeFact(int n) {
    vector<int> a;
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            int cnt = 0;
            while (n % i == 0) cnt++, n /= i;
            a.emplace_back(i);
        }
    }
    if (n > 1) a.emplace_back(n);
    return a;
}

bool is_primitive_root(int g, int n, int phi_n, const vector<int> &factors) {
    for (auto factor: factors) {
        if (exp(g, phi_n / factor, n) == 1) return false;
    }
    return true;
}

vector<int> get_primitive_roots(int n) {
    // any prime number has phi(n - 1) primitive root
    vector<int> roots;
    int phi_n = phi(n);
    vector<int> factors = PrimeFact(phi_n);
    for (int g = 2; g < n; ++g) {
        if (gcd(g, n) == 1 && is_primitive_root(g, n, phi_n, factors)) { roots.push_back(g); }
    }
    return roots;
}

void solve() {
    auto x = get_primitive_roots(100057);
    for (auto i: x) cout << i << ' ';
}

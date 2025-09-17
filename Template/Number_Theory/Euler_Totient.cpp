#include<bits/stdc++.h>
using namespace std;
#define int long long


/*
 * some facts
 * 1. \phi (p^k) = p^k - p^(k-1) where p is prime
 * 2. \phi (ab) = \phi (a) \phi (b) where a and b are coprime
 * form 1 and 2 -> 3
 * 3. \phi (n)= p1^(k1-1)*(p1 - 1) * p2^(k2-1)*(p2 - 1) * p3^(k3-1)*(p3 - 1)
 * ......
 * 4. \sum (d|n) \phi (d) = n
 */
vector<int> compute_phi(int n) {
    // \sum (d|n) \phi (d) = n
    // using the fact that sum of phi[divisors of n] = n
    vector<int> phi(n + 1);
    for (int i = 1; i <= n; i++) {
        phi[i] += i;
        for (int j = 2 * i; j <= n; j += i) { phi[j] -= phi[i]; }
    }
    return phi;
}

vector<int> linear_phi(int n) {
    vector<int> lp(n + 1);
    vector<int> pr;
    vector<int> phi(n + 1);
    phi[1] = 1;
    for (int i = 2; i < n; ++i) {
        if (!lp[i]) {
            pr.push_back(i);
            phi[i] = i - 1;  // i is pr
        }
        for (int j = 0; j < pr.size() && i * pr[j] <= n; ++j) {
            lp[i * pr[j]] = true;
            if (i % pr[j] == 0) {
                phi[i * pr[j]] = phi[i] * pr[j];  // pr[j] divides i
                break;
            } else {
                phi[i * pr[j]] = phi[i] * phi[pr[j]];  // pr[j] does not divide i
            }
        }
    }
    return phi;
}

int phi(int n) {
    vector <pair<int, int>> divisors = PrimeFact(n);
    // pairs {prime number, exponent}
    int ans = 1;
    for (auto [prime, exp]: divisors) {
        int power = 1;
        for (int i = 1; i < exp; i++) { power *= prime; }
        ans *= (power * prime - power);  // (p^exp - p^{exp-1})
    }
    return ans;
}

int phi(int n) {
    int result = n == 1 ? 0 : n;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            while (n % i == 0) n /= i;
            result -= result / i;
        }
    }
    if (n > 1) result -= result / n;
    return result;
}
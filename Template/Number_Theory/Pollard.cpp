#include <bits/stdc++.h>

using namespace std;
#define int long long

using ul = uint64_t;
using u128 = __uint128_t;

class Pollard {
    mt19937_64 rng{(ul) chrono::steady_clock::now().time_since_epoch().count()};

public:
    map<ul, int> cnt_primes;
    vector<ul> primes, divisors;

    // Multiplies a and b modulo mod safely in O(1).
    ul modMul(ul a, ul b, ul mod) {
        return (u128) a * b % mod;
    }

    ul modPow(ul a, ul b, ul mod) {
        ul res = 1;
        while (b) {
            if (b & 1) res = modMul(res, a, mod);
            a = modMul(a, a, mod), b >>= 1;
        }
        return res;
    }

    // Calculates (x*x+c) modulo mod safely in O(1).
    ul rhoNext(ul x, ul c, ul mod) {
        return ((u128) modMul(x, x, mod) + c) % mod;
    }

    // Checks whether n is prime deterministically for 64-bit integers in O(log n).
    bool isPrime(ul n) {
        if (n < 2) return false;
        for (ul p: {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL}) {
            if (n % p == 0) return n == p;
        }
        ul d = n - 1, s = 0;
        while (!(d & 1)) {
            d >>= 1;
            s++;
        }
        for (ul a: {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL}) {
            if (a % n == 0) continue;
            ul x = modPow(a % n, d, n);
            if (x == 1 or x == n - 1) continue;
            bool composite = true;

            for (ul r = 1; r < s; r++) {
                x = modMul(x, x, n);
                if (x == n - 1) {
                    composite = false;
                    break;
                }
            }
            if (composite) return false;
        }
        return true;
    }

    // Finds a non-trivial factor of composite n in expected O(sqrt(p)), where p is its smallest prime factor.
    ul pollard(ul n) {
        if (n % 2 == 0) return 2;
        if (n % 3 == 0) return 3;
        while (true) {
            ul y = uniform_int_distribution<ul>(2, n - 2)(rng);
            ul c = uniform_int_distribution<ul>(1, n - 1)(rng);
            ul m = 128;
            ul g = 1, r = 1;
            ul x = 0, ys = 0;
            while (g == 1) {
                x = y;
                for (ul i = 0; i < r; i++)
                    y = rhoNext(y, c, n);

                ul q = 1;
                for (ul k = 0; k < r && g == 1; k += m) {
                    ys = y;
                    for (ul i = 0; i < min(m, r - k); i++) {
                        y = rhoNext(y, c, n);
                        ul dif = x > y ? x - y : y - x;
                        q = modMul(q, dif, n);
                    }
                    g = gcd(q, n);
                }
                r <<= 1;
            }

            if (g == n) {
                do {
                    ys = rhoNext(ys, c, n);
                    ul dif = x > ys ? x - ys : ys - x;
                    g = gcd(dif, n);
                } while (g == 1);
            }
            if (g != n) return g;
        }
    }

    // Recursively factorizes n and stores its prime exponents in expected about O(n^(1/4)) for hard 64-bit inputs.
    void factorRec(ul n) {
        if (n == 1) return;

        if (isPrime(n)) {
            cnt_primes[n]++;
            return;
        }

        ul d = pollard(n);

        factorRec(d);
        factorRec(n / d);
    }

    // Clears previous data and factorizes n in expected about O(n^(1/4)) for hard 64-bit inputs.
    void factorize(ul n) {
        cnt_primes.clear();

        for (ul p: {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL}) {
            while (n % p == 0) {
                cnt_primes[p]++;
                n /= p;
            }
        }
        factorRec(n);
    }

    // Generates all divisors from the prime factorization in O(tau(n) * omega(n)).
    void divisorsRec(ul cur, int i) {
        if (i == (int) primes.size()) {
            divisors.push_back(cur);
            return;
        }

        ul p = primes[i];
        int e = cnt_primes[p];
        for (int j = 0; j <= e; j++) {
            divisorsRec(cur, i + 1);
            if (j < e)
                cur *= p;
        }
    }

    // Factorizes n and stores all its divisors sorted in factorization time plus O(tau(n) log tau(n)).
    void calcDivisors(ul n) {
        factorize(n);

        primes.clear();
        divisors.clear();

        for (auto [p, e]: cnt_primes)
            primes.push_back(p);

        divisorsRec(1, 0);
        sort(divisors.begin(), divisors.end());
    }

    // Returns the number of positive divisors of n in factorization time plus O(omega(n)).
    ul numberOfDivisors(ul n) {
        factorize(n);

        ul ans = 1;

        for (auto [p, e]: cnt_primes)
            ans *= e + 1;

        return ans;
    }

    // Returns the number of distinct prime factors of n in factorization time.
    int numberOfDistinctPrimeFactors(ul n) {
        factorize(n);
        return cnt_primes.size();
    }

    // Returns the total number of prime factors with multiplicity in factorization time plus O(omega(n)).
    int numberOfPrimeFactors(ul n) {
        factorize(n);

        int ans = 0;

        for (auto [p, e]: cnt_primes)
            ans += e;

        return ans;
    }
} pollard;

void solve() {
    ul n;
    cin >> n;

    cout << pollard.numberOfDivisors(n) << '\n';
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
#ifdef HALZOOM
    freopen("Input.txt", "r", stdin);
    freopen("Output.txt", "w", stdout);
#endif

    int test = 1;
//    cin >> test;

    for (int i = 1; i <= test; ++i) {
        solve();
    }

    return 0;
}
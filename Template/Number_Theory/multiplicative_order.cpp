// is the smallest positive integer x such that a^x = 1 mod m
int multiplicative_order(int a, int m) {
    if (gcd(a, m) != 1) return -1;
    vector<int> primes = primeFact(ord);
    int ord = phi(m);
    for (auto p: primes) {
        while (ord % p == 0 && exp(a, ord / p, m) == 1)
            ord /= p;
    }
    return ord;
}
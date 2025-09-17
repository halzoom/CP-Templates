#include<bits/stdc++.h>
using namespace std;
#define int long long

// 11
ll calc_ones(ll a, ll bit) {
    ++bit;
    ll ones = a / (1ll << bit) * (1ll << (bit - 1));
    if (a % (1ll << bit) >= (1ll << (bit - 1)))
        ones += a % (1ll << bit) - (1ll << (bit - 1)) + 1;
    return ones;
}
// calc how many bit number (bit) appear in range (1, a)
// if you want range [l, r]  = calc_ones(r, bit_number) - calc(l - 1,
// bit_number) now you can get xor , or  , and easily  you have the number of
// this bit appear in or it's enough for this bit to appear at least once to
// consider it in your answer in and it should appear (r - l + 1) , in xor
// should appear odd times
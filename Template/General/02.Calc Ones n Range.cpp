#include<bits/stdc++.h>

using namespace std;
#define int long long

// 11
int calc_ones(int a, int bit) {
    ++bit;
    int ones = a / (1ll << bit) * (1ll << (bit - 1));
    if (a % (1ll << bit) >= (1ll << (bit - 1)))
        ones += a % (1ll << bit) - (1ll << (bit - 1)) + 1;
    return ones;
}

int get(int n, int m) { // submasks of m less than n
    if (n < 0)return 0;
    int answer = 0;
    for (int bit = 30; bit >= 0; --bit) {
        int nBit = n >> bit & 1;
        int mBit = m >> bit & 1;
        if (nBit == 0)
            continue;
        int all = (1ll << bit) - 1;
        int cnt = __builtin_popcount(m & all);
        if (mBit == 1) {
            answer += 1 << cnt;
        } else {
            answer += 1 << cnt;
            return answer;
        }
    }
    answer++;
    return answer;
}
// calc how many bit number (bit) appear in range (1, a)
// if you want range [l, r]  = calc_ones(r, bit_number) - calc(l - 1,
// bit_number) now you can get xor , or  , and easily  you have the number of
// this bit appear in or it's enough for this bit to appear at least once to
// consider it in your answer in and it should appear (r - l + 1) , in xor
// should appear odd times
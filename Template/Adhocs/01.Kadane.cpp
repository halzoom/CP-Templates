#include<bits/stdc++.h>
using namespace std;
#define int long long

// 10
int kadane(const vector<int> &nums) {
    int maxSoFar = INT_MIN;
    int maxEndingHere = 0;
    for (int num: nums) {
        maxEndingHere += num;
        if (maxSoFar < maxEndingHere) maxSoFar = maxEndingHere;
        if (maxEndingHere < 0) maxEndingHere = 0;
    }
    return maxSoFar;
}
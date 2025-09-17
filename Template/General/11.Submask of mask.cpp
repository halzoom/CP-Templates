#include<bits/stdc++.h>

using namespace std;

int main() {
    for (int mask = 0; mask < 1 << n; ++mask) {
        for (int b = (b - mask) & mask; b; b = (b - mask) & mask)
            res = max(res, go(mask ^ b) + cost[b]);

        int p = (1 << n) - 1 & ~mask, nxt = 0;
        do {
            if (nxt >> 1 & mask or nxt << 1 & mask)continue;
            if (nxt << 1 & nxt or nxt >> 1 & nxt)continue;
            valid[mask].emplace_back(nxt);
        } while (nxt = (nxt - p) & p);
    }
    return 0;
}
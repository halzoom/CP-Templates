#include<bits/stdc++.h>
using namespace std;
#define int long long

//8
void PartialSum_in2D(int x1, int y1, int x2, int y2, vector <vector<int>> &Prefix) {
    if (x1 > x2) swap(x1, x2);
    if (y1 > y2) swap(y1, y2);
    Prefix[x1][y1]++;
    Prefix[x2 + 1][y1]--;
    Prefix[x1][y2 + 1]--;
    Prefix[x2 + 1][y2 + 1]++;
}
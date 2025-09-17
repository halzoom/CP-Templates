#include<bits/stdc++.h>
using namespace std;
#define int long long

// 9
vector<int> nextSmaller(vector<int> &a) {
    int n = a.size();
    vector<int> result(n, n), st;
    for (int i = 0; i < n; ++i) {
        while (!st.empty() && a[st.back()] > a[i]) result[st.back()] = i, st.pop_back();
        st.push_back(i);
    }
    return result;
}
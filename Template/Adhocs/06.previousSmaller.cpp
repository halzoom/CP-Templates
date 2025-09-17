#include<bits/stdc++.h>
using namespace std;
#define int long long

// 10
vector<int> previousSmaller(vector<int> &a) {
    int n = a.size();
    vector<int> result(n, -1), st;
    for (int i = 0; i < n; ++i) {
        while (!st.empty() && a[st.back()] >= a[i]) st.pop_back();
        if (!st.empty()) result[i] = st.back();
        st.push_back(i);
    }
    return result;
}
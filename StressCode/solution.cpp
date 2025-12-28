#include <bits/stdc++.h>


using namespace std;

typedef long long ll;
typedef long double ld;
#define int long long
#define endl "\n"
#define Endl "\n"
//-------------------\\



void solve() {
    int n; cin >> n;
    int arr[n]; for (int i=0;i<n;i++) cin >> arr[i];

    SegTree st(n + 2);
    st.set(arr[0],arr[0]+1,1);
    for (int i=0;i<n;i++) {
        bool covered = st.get(arr[i],n+1);
        if (covered) {
            st.set(arr[i],n+1,1);
            st.set(arr[i],arr[i]+1,0);
        }
        else {
            cout << "NO" << endl;
            return;
        }
    }
    cout << "YES" << endl;
}


signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    cout << fixed << setprecision(9);
    int t = 1;
    cin >> t;
    while (t--) solve();
    return 0;
}

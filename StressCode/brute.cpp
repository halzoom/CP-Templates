#include<bits/stdc++.h>

using namespace std;

struct apos {
    long long a;
    long long b;

    friend bool operator<(apos a, apos b) {
        return a.a > b.a;
    }
} ap[200000];

int main() {
    ios::sync_with_stdio(false), cin.tie(0);
    int T, n, i;
    long long l, r, ans, cans, mx;
    for (cin >> T; T > 0; T--) {
        cin >> n;
        ans = 0;
        for (i = 0; i < n; i++) {
            cin >> l >> r;
            ans += r - l;
            ans -= l;
            ap[i].a = r + l;
            ap[i].b = l;
        }
        sort(ap, ap + n);
        if (n % 2 == 0) {
            for (i = 0; i < n / 2; i++)ans += ap[i].a;
            cout << ans << '\n';
            continue;
        }
        cans = 0;
        mx = 0;
        for (i = 0; i < n / 2; i++)cans += ap[i].a;
        for (i = 0; i < n; i++) {
            if (i < n / 2)mx = max(mx, cans - ap[i].a + ap[i].b + ap[n / 2].a);
            else mx = max(mx, cans + ap[i].b);
        }
        cout << ans + mx << '\n';
    }
    return 0;
}
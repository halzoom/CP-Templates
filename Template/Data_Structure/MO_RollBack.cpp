#include<bits/stdc++.h>
using namespace std;
#define int long long

/*
 * You have an array a of size n and q queries , for each query
 * calculate the maximum distance between two same numbers from l to r
 * */
struct MO_RollBack {
    vector<int> v, L, R;
    int n = 0, B = 0, ans = 0;

    MO_RollBack(vector<int> &a) {
        v = a;
        n = (int) a.size();
        B = (int) sqrt(n) + 2;
        L.assign(n + 1, -1);
        R.assign(n + 1, -1);
    }

    void add(int idx) {
        int x = v[idx];
        if (L[x] + R[x] == -2) {
            L[x] = R[x] = idx;
        } else if (R[x] < idx) {
            R[x] = idx;
            ans = max(ans, R[x] - L[x]);
        } else {
            L[x] = idx;
            ans = max(ans, R[x] - L[x]);
        }
    }

    int get() { return ans; }

    void Process(vector <array<int, 3>> query[], vector<int> &res) {
        for (int i = 0; i < B; ++i) {  // answer every block
            if (query[i].empty()) continue;
            sort(query[i].begin(), query[i].end());
            L.assign(n + 1, -1);
            R.assign(n + 1, -1);
            ans = 0;
            int l = (i + 1) * B - 1, r = l;
            add(r);
            for (const auto [rq, lq, idx]: query[i]) {
                while (r < rq) {
                    ++r;
                    add(r);
                }
                int tmp_ans = ans;              // save current answer
                vector <array<int, 3>> updates;  // save all changes to rollback
                while (lq < l) {
                    --l;
                    updates.push_back({v[l], L[v[l]], R[v[l]]});
                    add(l);
                }
                res[idx] = get();
                ans = tmp_ans;  // return to the last results
                reverse(updates.begin(),
                        updates.end());  // start from the last change to the first change
                for (auto [x, lst_L, lst_R]: updates) {
                    L[x] = lst_L;
                    R[x] = lst_R;
                }
                l = (i + 1) * B - 1;
            }
        }
    }
};

int main() {
    int n, m, q;
    cin >> n >> m >> q;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) { cin >> a[i]; }
    int B = sqrt(n) + 2;
    MO_RollBack mo(a);
    vector<int> res(q);
    vector <array<int, 3>> query[B];
    for (int i = 0; i < q; ++i) {
        int l, r;
        cin >> l >> r;
        --l, --r;
        if (r - l + 1 < B) {
            for (int j = l; j <= r; ++j) {
                mo.L[a[j]] = -1;
                mo.R[a[j]] = -1;
            }
            for (int j = l; j <= r; ++j) mo.add(j);
            res[i] = mo.get();
            mo.ans = 0;
            continue;
        }
        query[l / B].push_back({r, l, i});
    }
    mo.Process(query, res);
    for (auto i: res) cout << i << '\n';
    return 0;
}

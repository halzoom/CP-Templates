#include "testlib.h"
#include <bits/stdc++.h>

using ll = long long;
using namespace std;
array<int, 2> N, Q, K, T;

void gen() {
    int n = rnd.next(N[0], N[1]);
    int q = rnd.next(Q[0], Q[1]);
    int k = rnd.next(K[0], K[1]);
    cout << n << ' ' << q << ' ' << k << endl;
    for(int i = 0 ; i < n ; ++i){
        int ai = rnd.next(1 , k);
        cout << ai << " \n"[i == n - 1];
    }
    for (int i = 0; i < q; ++i) {
        int t = rnd.next(T[0], T[1]);
        if (t == T[1] and n > 20) {
            int idx = rnd.next(1, n);
            int x = rnd.next(1, k);
            cout << 1 << ' ' << idx << ' ' << x;
        } else {
            int l = rnd.next(1, n);
            int r = rnd.next(1, n);
            if (l > r)swap(l, r);
            int x = rnd.next(0 , k);
            cout << 2 << ' ' << l << ' ' << r << ' ' << x;
        }
        cout << endl;
    }
}

int32_t main(int argc, char *argv[]) {
    registerGen(argc, argv, 1);
    rnd.setSeed(opt<int>("S"));
    N = {opt<int>("mn_N"), opt<int>("mx_N")};
    Q = {opt<int>("mn_Q"), opt<int>("mx_Q")};
    K = {opt<int>("mn_K"), opt<int>("mx_K")};
    T = {opt<int>("mn_T"), opt<int>("mx_T")};

    gen();
    return 0;
}


//#include <bits/stdc++.h>
//#include "testlib.h"
//
//using namespace std;
//
//int main(int argc, char *argv[]) {
//
//    registerGen(argc, argv, 0);
//    int maxN = 100'000, maxM = 100'000, maxQ = 100'000, sum = 1000000;
//    int n = rnd.next(maxN, maxN);
//    int m = rnd.next(maxM, maxM);
//    int q = rnd.next(maxQ, maxQ);
//    vector<int> movies(m);
//    iota(movies.begin(), movies.end(), 1);
//    shuffle(movies.begin(), movies.end());
//    cout << n << ' ' << m << ' ' << q << endl;
//    for (int i = n; i; --i) {
//        int x = rnd.next(1, min(m, sum - i));
//        cout << x << ' ';
//        sum -= x;
//        for (int j = 0; j < x; ++j) {
//            int movie = rnd.next(0, m - j - 1);
//            cout << movie << " \n"[j == x - 1];
//            swap(movies[movie], movies[m - j - 1]);
//        }
//    }
//    int sumK = 1e6;
//    for (int i = q; i ; --i) {
//        int x = rnd.next(1 , n);
//        int k = rnd.next(1 , sumK - i);
//        sumK -= k;
//        cout << x << ' ' << k << " \n"[i == 1];
//    }
//}
### Generator Template #1
```cpp
#include "testlib.h"
#include <bits/stdc++.h>

using ll = long long;
using namespace std;
array<int, 2> N, M;

void gen() {
    int n = rnd.next(N[0], N[1]);
    int m = rnd.next(M[0], M[1]);
    cout << n << ' ' << m << endl;
}

int32_t main(int argc, char *argv[]) {
    registerGen(argc, argv, 1);
    rnd.setSeed(opt<int>("S"));
    N = {opt<int>("mn_N"), opt<int>("mx_N")};
    M = {opt<int>("mn_M"), opt<int>("mx_M")};

    gen();

    return 0;
}
```

----
### Generator Template #2
```cpp
#include <bits/stdc++.h>
#include "testlib.h"

using namespace std;

int main(int argc, char *argv[]) {

    registerGen(argc, argv, 0);
    int maxN = 100'000, maxM = 100'000, maxQ = 100'000, sum = 1000000;
    int n = rnd.next(maxN, maxN);
    int m = rnd.next(maxM, maxM);
    int q = rnd.next(maxQ, maxQ);
    vector<int> movies(m);
    iota(movies.begin(), movies.end(), 1);
    shuffle(movies.begin(), movies.end());
    cout << n << ' ' << m << ' ' << q << endl;
    for (int i = n; i; --i) {
        int x = rnd.next(1, min(m, sum - i));
        cout << x << ' ';
        sum -= x;
        for (int j = 0; j < x; ++j) {
            int movie = rnd.next(0, m - j - 1);
            cout << movie << " \n"[j == x - 1];
            swap(movies[movie], movies[m - j - 1]);
        }
    }
    int sumK = 1e6;
    for (int i = q; i ; --i) {
        int x = rnd.next(1 , n);
        int k = rnd.next(1 , sumK - i);
        sumK -= k;
        cout << x << ' ' << k << " \n"[i == 1];
    }
}
```

----



### To run with FreeMaker

```shell
<#list 2..10 as i>
    gen -mn_N 1 -mx_N 10 -mn_q 1 -mx_q 5 -S ${i} > $
</#list>
````
```shell
<#list 11..19 as i>
    gen -mn_N 100 -mx_N 1000 -mn_q 10 -mx_q 5000 -S ${i} > $
</#list>
```
```shell
<#list 20..49 as i>
    gen -mn_N 10000 -mx_N 10000 -mn_q 10000 -mx_q 10000 -S ${i} > $
</#list>
```
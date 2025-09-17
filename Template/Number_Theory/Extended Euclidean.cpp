#include<bits/stdc++.h>
using namespace std;
#define int long long

/*
 * http://e-maxx.ru/algo/diofant_2_equation&usg=ALkJrhhAzF9yCVA7pOjdWhVRIFdPsBlzmA
 */
int extended_gcd(int a, int b, int &x, int &y) {
    if (a == 0) {
        x = 0;
        y = 1;
        return b;
    }
    int x1, y1;
    int g = extended_gcd(b % a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
    return g;
}

bool find_any_solution(int a, int b, int c, int &x0, int &y0, int &g) {
    g = extended_gcd(abs(a), abs(b), x0, y0);
    if (c % g != 0) return false;
    x0 *= c / g;
    y0 *= c / g;
    if (a < 0) x0 *= -1;
    if (b < 0) y0 *= -1;
    return true;
}

void shift_solution(int &x, int &y, int a, int b, int cnt) {
    x += cnt * b;
    y -= cnt * a;
    // ax + by = c
    // x += k * b / g
    // y -= k * a / g
}

int find_all_solutions(int a, int b, int c, int minx, int maxx, int miny, int maxy) {
    int x, y, g;
    if (!find_any_solution(a, b, c, x, y, g)) return 0;
    a /= g;
    b /= g;
    int sign_a = a > 0 ? +1 : -1;
    int sign_b = b > 0 ? +1 : -1;
    shift_solution(x, y, a, b, (minx - x) / b);
    if (x < minx) shift_solution(x, y, a, b, sign_b);
    if (x > maxx) return 0;
    int lx1 = x;
    shift_solution(x, y, a, b, (maxx - x) / b);
    if (x > maxx) shift_solution(x, y, a, b, -sign_b);
    int rx1 = x;
    shift_solution(x, y, a, b, -(miny - y) / a);
    if (y < miny) shift_solution(x, y, a, b, -sign_a);
    if (y > maxy) return 0;
    int lx2 = x;
    shift_solution(x, y, a, b, -(maxy - y) / a);
    if (y > maxy) shift_solution(x, y, a, b, sign_a);
    int rx2 = x;
    if (lx2 > rx2) swap(lx2, rx2);
    int lx = max(lx1, lx2);
    int rx = min(rx1, rx2);
    //    for (int curx = lx; curx <= rx; curx += abs(b)) {
    //        int cury = (c - a * curx) / b;
    //        if (cury >= miny && cury <= maxy)
    //            solutions.emplace_back(curx, cury);
    //    }
    return (rx - lx) / abs(b) + 1;
}

bool findFastMinSumSolution(int a, int b, int c, int &minX, int &minY) {
    int x0, y0, g;
    if (!find_any_solution(a, b, c, x0, y0, g))
        return false;

    int dx = b / g;
    int dy = a / g;

    // Bounds for k to keep x and y non-negative
    int k_min = (int) ceil((double) (-x0) / dx);
    int k_max = (int) floor((double) (y0) / dy);

    if (k_min > k_max)
        return false;

    // delta tells how x+y changes with k
    int delta = dx - dy , k_opt;
    if (delta > 0)
        k_opt = k_min;
    else if (delta < 0)
        k_opt = k_max;
    else
        k_opt = k_min; // any k in range gives same sum

    minX = x0 + k_opt * dx;
    minY = y0 - k_opt * dy;
    return true;
}
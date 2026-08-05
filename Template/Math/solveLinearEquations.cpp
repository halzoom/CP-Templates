#include <bits/stdc++.h>

using namespace std;

#define int long long

const int inf = 1e18;
const double EPS = 1e-10;

/*
    { inf,  inf} -> infinitely many solutions
    {-inf, -inf} -> no solution
    otherwise    -> unique double solution
*/
pair<double, double> solveLinearEquations(double a1, double b1, double c1, double a2, double b2, double c2) {
    double determinant = a1 * b2 - a2 * b1;

    if (fabs(determinant) < EPS) {
        bool firstInvalid = fabs(a1) < EPS && fabs(b1) < EPS && fabs(c1) >= EPS;
        bool secondInvalid = fabs(a2) < EPS && fabs(b2) < EPS && fabs(c2) >= EPS;
        if (firstInvalid || secondInvalid)
            return {-inf, -inf};

        if (fabs(a1 * c2 - a2 * c1) < EPS && fabs(b1 * c2 - b2 * c1) < EPS)
            return {inf, inf};

        return {-inf, -inf};
    }

    double x = (b2 * c1 - b1 * c2) / determinant;
    double y = (a1 * c2 - a2 * c1) / determinant;

    return {x, y};
}

/*
    { inf,  inf} -> infinitely many solutions
    {-inf, -inf} -> no real solution
    {-inf,  inf} -> unique real solution, but it is not integer
    otherwise    -> unique integer solution
*/
pair<int, int> solveLinearEquations(int a1, int b1, int c1, int a2, int b2, int c2) {
    __int128 determinant =
            (__int128) a1 * b2 - (__int128) a2 * b1;

    if (determinant == 0) {
        bool firstInvalid = (a1 == 0 && b1 == 0 && c1 != 0);
        bool secondInvalid = (a2 == 0 && b2 == 0 && c2 != 0);

        if (firstInvalid || secondInvalid)
            return {-inf, -inf};

        __int128 check1 = (__int128) a1 * c2 - (__int128) a2 * c1;

        __int128 check2 = (__int128) b1 * c2 - (__int128) b2 * c1;

        if (check1 == 0 && check2 == 0)
            return {inf, inf};

        return {-inf, -inf};
    }
    __int128 xNumerator =
            (__int128) b2 * c1 - (__int128) b1 * c2;

    __int128 yNumerator =
            (__int128) a1 * c2 - (__int128) a2 * c1;

    if (xNumerator % determinant != 0 ||
        yNumerator % determinant != 0)
        return {-inf, inf};

    __int128 x = xNumerator / determinant;
    __int128 y = yNumerator / determinant;

    if (x < -inf || x > inf || y < -inf || y > inf)
        return {-inf, -inf};

    return {(int) x, (int) y};
}
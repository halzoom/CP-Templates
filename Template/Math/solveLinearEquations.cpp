#include<bits/stdc++.h>
using namespace std;
#define int long long

const int inf = 1e18;

pair<int, int> solveLinearEquations(double a1, double b1, double c1, double a2, double b2,
                                    double c2) {
    double determinant = a1 * b2 - a2 * b1;
    pair<int, int> result{};
    if (fabs(determinant) < 1e-10) {
        if (fabs(a1 * c2 - a2 * c1) < 1e-10 && fabs(b1 * c2 - b2 * c1) < 1e-10)
            result = {inf, inf};       // infinite number of solutions
        else result = {-inf, -inf};  // no solution
    } else {
        double x = (b2 * c1 - b1 * c2) / determinant;
        double y = (a1 * c2 - a2 * c1) / determinant;
        result = {x, y};
    }
    return result;
}

pair<int, int> solveLinearEquations(int a1, int b1, int c1, int a2, int b2, int c2) {
    __int128 determinant = (__int128) a1 * b2 - (__int128) a2 * b1;
    __int128 x = ((__int128) b2 * c1 - (__int128) b1 * c2) / determinant;
    __int128 y = ((__int128) a1 * c2 - (__int128) a2 * c1) / determinant;
    return {x, y};
}

pair<int, int> solveLinearEquations(int a1, int b1, int c1, int a2, int b2, int c2) {
    __int128 determinant = (__int128) a1 * b2 - (__int128) a2 * b1;
    if (determinant == 0) return {0, 0};  // No unique solution
    __int128 x = ((__int128) b2 * c1 - (__int128) b1 * c2);
    __int128 y = ((__int128) a1 * c2 - (__int128) a2 * c1);
    if (x % determinant != 0 || y % determinant != 0) return {-1, -1};  // No value solution
    x /= determinant;
    y /= determinant;
    return {(int) x, (int) y};
}
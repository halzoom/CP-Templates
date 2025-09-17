#include<bits/stdc++.h>
using namespace std;
#define int long long

#define double long double  // ************
const double EPS = 1e-9;

pair <complex<double>, complex<double>> solveQuadratic(double a, double b, double c) {
    complex<double> discriminant = b * b - 4.0 * a * c;
    complex<double> sqrt_discriminant = sqrt(discriminant);
    complex<double> root1 = (-b + sqrt_discriminant) / (2.0 * a);
    complex<double> root2 = (-b - sqrt_discriminant) / (2.0 * a);
    return {root1, root2};
}
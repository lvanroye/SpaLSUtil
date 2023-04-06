#include <iostream>
#include "node.hpp"
using namespace SpaLS;
int main()
{
    Sym a("a");
    Sym b("b");
    Sym c("c");
    Sym x("x");
    Sym y("y");

    auto expr = (a + b + 1) * (x + y) + 5 * x;
    auto terms = GetTerms(expr);
    for (auto term : terms)
        cout << term << endl;
    auto coeffs = GetCoefficients({expr}, {x, y});
    // coefficients
    cout << "coefficients: "<< endl;
    for (auto coeff : coeffs.at(0))
        cout << coeff << endl;
    return 0;
    // linear_system.add_equation(-X.at(k+1) + A.at(K) * X.at(k) + B.at(k) * U.at(k), 0);
};

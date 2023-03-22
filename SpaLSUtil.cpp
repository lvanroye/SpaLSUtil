#include <iostream>
#include "node.hpp"
using namespace SpaLS;
int main()
{
    Sym x("x");
    Sym y("y");
    Sym a("a");
    auto expr = x + y + a*y;
    cout << expr << endl;
    auto terms = GetTerms(expr);
    // print the terms
    for(auto term : terms)
    {
        cout << term << endl;
    }
    // print the coefficients
    auto coefficients = GetCoefficients(expr, {x, y});
    for(auto coefficient : coefficients)
    {
        cout << coefficient << endl;
    }
    return 0;
};

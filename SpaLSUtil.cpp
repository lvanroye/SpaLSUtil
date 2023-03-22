#include <iostream>
#include "node.hpp"
using namespace SpaLS;
int main()
{
    Sym x("x");
    Sym y("y");
    Sym a("a");
    Sym b("b");
    auto expr1 = (a+b);
    auto expr = expr1*x + Const(5)*y + expr1*a*y + a*b*x;
    cout << "expr:" << endl;
    auto coefficients = GetCoefficients(expr, {x, y});
    Function testf({a, b}, Const(3)*a + Const(2)*b);
    cout << testf.Eval({1, 2}) << endl;
    return 0;
};


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
    cout << expr << endl;
    // print the coefficients
    cout << "coefficients:" << endl;
    auto coefficients = GetCoefficients(expr, {x, y});
    for(auto coefficient : coefficients)
    {
        cout << coefficient << endl;
    }
    // order depth first
    cout << "order depth first:" << endl;
    vector<Expression> ordered_expr;
    OrderDepthFirstRecurse(expr, ordered_expr);
    for(auto expr : ordered_expr)
    {
        cout << expr << endl;
    }
    return 0;
};


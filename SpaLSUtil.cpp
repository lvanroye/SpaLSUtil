#include <iostream>
#include "node.hpp"
using namespace SpaLS;
int main()
{
    Sym x("x");
    Sym y("y");
    Sym a("a");
    Sym b("b");
    auto expr = Const(5)*y + a*y + a*b*x;
    auto coeffs = GetCoefficients(expr, {x, y});
    Function coeffs_f({a,b}, coeffs);
    auto res = coeffs_f.Eval({3,4});
    // print res
    for(auto& r : res)
        std::cout << r << std::endl;
    return 0;
};


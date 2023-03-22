#include <iostream>
#include "node.hpp"
using namespace SpaLS;
int main()
{
    Sym x("x");
    Sym y("y");
    Sym z("z");
    Sym a("a");
    Sym b("b");
    Sym c("c");
    auto expr = Const(5)*y + a*y + a*b*x + c*z;
    auto coeffs = GetCoefficients(expr, {x, y, z});
    Function coeffs_f({a,b,c}, coeffs);
    auto res = coeffs_f.Eval({3,4,420});
    // print res
    for(auto& r : res)
        std::cout << r << std::endl;
    return 0;
};


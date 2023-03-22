#include <iostream>
#include "node.hpp"
using namespace SpaLS;
int main()
{
    SymMatrix xm("x", 3, 1);
    auto expr = Eye(3) * xm;
    auto vec_expr = vec(expr);
    for (auto& e : vec_expr)
        std::cout << e << std::endl;
    auto coeffs = GetCoefficients(vec_expr, vec(xm));
    // print out the coefficients
    for(auto row : coeffs)
    {
        for(auto col : row)
        {
            std::cout << col << " ";
        }
        std::cout << std::endl;
    };



    return 0;
};


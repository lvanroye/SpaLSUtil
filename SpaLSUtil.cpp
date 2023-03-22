#include <iostream>
#include "node.hpp"
using namespace SpaLS;
int main()
{
    SymMatrix xm("x", 3, 1);
    SymMatrix params("p", 1, 1);
    auto expr = Eye(3) * xm;
    expr(0, 0) = expr(0, 0) + params(0, 0) * xm(0, 0);
    auto vec_expr = vec(expr);
    for (auto &e : vec_expr)
        std::cout << e << std::endl;
    auto coeffs = GetCoefficients(vec_expr, vec(xm));
    // print out the coefficients
    for (auto row : coeffs)
    {
        for (auto col : row)
        {
            std::cout << col << " ";
        }
        std::cout << std::endl;
    };
    return 0;
    // linear_system.add_equation(-X.at(k+1) + A.at(K) * X.at(k) + B.at(k) * U.at(k), 0);
};

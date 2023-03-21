#include <iostream>
#include "node.hpp"
using namespace SpaLS;
int main()
{
    Sym x("x");
    Sym y("y");
    auto expr = x + y;
    GetCoeff(expr,x);
    cout << expr << endl;
    return 0;
};

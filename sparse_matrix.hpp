#ifndef SPARSEMATRIXINCLUDED
#define SPARSEMATRIXINCLUDED
#include "node.hpp"
namespace SpaLS
{
    class SparseMatrix
    {
    public:
        SymMatrix variable(int size)
        {
            SymMatrix sym("x", size, 1);
            variables.push_back(sym);
            return sym;
        }
        void parameter(int m, int n)
        {
            SymMatrix sym("p", m, n);
            variables.push_back(sym);
        }
        // set_value(variable, value)
        void add_equation(const Expression &lhs, const Expression &rhs)
        {
            equations.push_back(lhs);
            rhss.push_back(rhs);
        }
        vector<Matrix> variables;
        vector<Expression> equations;
        vector<Expression> rhss;
    };
}

#endif // SPARSEMATRIXINCLUDED
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
            auto symvec = vec(sym);
            variables.insert(variables.end(), symvec.begin(), symvec.end());
            return sym;
        }
        // parameter(m, n)
        // set_value(variable, value)
        void add_equation(const Expression &lhs, const Expression &rhs)
        {
            equations.push_back(lhs);
            rhss.push_back(rhs);
        }
        // get_coefficients()
        // eval_triplets()
        vector<Expression> variables;
        vector<Expression> equations;
        vector<Expression> rhss;
    };
}

#endif // SPARSEMATRIXINCLUDED
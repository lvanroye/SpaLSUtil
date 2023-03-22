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
        // add_equation(lhs, rhs)
        // get_coefficients()
        // eval_triplets()
        vector<Sym> variables;
    };
}

#endif // SPARSEMATRIXINCLUDED
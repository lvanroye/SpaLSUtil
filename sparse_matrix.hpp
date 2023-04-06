#ifndef SPARSEMATRIXINCLUDED
#define SPARSEMATRIXINCLUDED
#include "node.hpp"
namespace SpaLS
{
    struct Index
    {
        int ai = 0;
        int aj = 0;
    };

    struct ValueMatrixAbstract
    {
        virtual int n_rows() const = 0;
        virtual int n_cols() const = 0;
        virtual double eval(const int ai, const int aj) const = 0;
    };

    class SparseMatrix
    {
    public:
        SymMatrix variable(int size)
        {
            SymMatrix sym("x", size, 1);
            variables.push_back(sym);
            dirty = true;
            return sym;
        }
        void parameter(int m, int n)
        {
            SymMatrix sym("p", m, n);
            parameters.push_back(sym);
            dirty = true;
        }
        // set_value(variable, value)
        void add_equation(const Matrix &lhs, const Matrix &rhs)
        {
            equations.push_back(lhs);
            rhss.push_back(rhs);
            dirty = true;
        }
        vector<Expression> get_coeffs()
        {
            if (dirty)
                make_clean();
            return coeffs;
        }
        vector<Index> get_sparsity()
        {
            if (dirty)
                make_clean();
            return sparsity;
        }
        // vector<double> Eval(const vector<double> &solution, const vector<Expression> &expression)
        // {
        // }
        // vector<double> EvalCoeffs()
        // {
        // }
        vector<Matrix> variables;
        vector<Matrix> parameters;
        vector<ValueMatrixAbstract> parameters_vals;
        vector<Matrix> equations;
        vector<Matrix> rhss;
        vector<Index> sparsity;
        vector<Expression> coeffs;
        Function coeffs_f; // computes coefficients from parameters

    private:
        void make_clean()
        {
            sparsity.resize(0);
            // coeffs.resize(0);
            auto eq_vec = vec(equations);
            auto var_vec = vec(variables);
            auto all_coeffs = GetCoefficients(eq_vec, var_vec);
            const int n_eq = equations.size();
            const int n_var = variables.size();
            // go through all equations
            for (int eq_i = 0; eq_i < n_eq; eq_i++)
            {
                // go through all variables
                for (int var_i = 0; var_i < n_var; var_i++)
                {
                    // check if coefficient is zero
                    if (all_coeffs.at(eq_i).at(var_i) == Zero())
                    {
                        continue;
                    }
                    else
                    {
                        sparsity.push_back({eq_i, var_i});
                        coeffs.push_back(all_coeffs.at(eq_i).at(var_i));
                    }
                }
            }
            // initialize the function that computes the coefficients
            coeffs_f = Function(vec(parameters), coeffs);
            dirty = false;
        }
        bool dirty = true;
    };
}

#endif // SPARSEMATRIXINCLUDED
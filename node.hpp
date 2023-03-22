#ifndef NODEINCLUDED
#define NODEINCLUDED

#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include <type_traits>
#include <algorithm>
using namespace std;
namespace SpaLS
{
    class Node
    {
    public:
        // Node();
        // ~Node();
        virtual ostream &print(ostream &os) { return os << "Node"; };
        virtual bool equals(const Node &node) const { return this == &node; };
    };
    class SymNode : public Node
    {
    public:
        SymNode(const string &name) : name(name){};
        const string name;
        virtual ostream &print(ostream &os) { return os << name; };
    };
    class ConstNode : public Node
    {
    public:
        ConstNode(const double value) : value(value){};
        const double value;
        virtual ostream &print(ostream &os) { return os << value; };
    };
    class ZeroNode : public Node
    {
    public:
        ZeroNode(){};
        virtual ostream &print(ostream &os) { return os << "00"; };
        virtual bool equals(const Node &node) const
        {
            // check if node is of ZeroNode type
            auto zero_node = dynamic_cast<const ZeroNode *>(&node);
            if (zero_node)
            {
                return true;
            }
            else
            {
                return false;
            }
        };
    };

    class Expression : public shared_ptr<Node>
    {
    public:
        Expression(const shared_ptr<Node> &ptr) : shared_ptr<Node>(ptr){};
        template <typename Derived>
        static Expression make_new(const Derived &node)
        {
            return Expression(make_shared<Derived>(node));
        }
        friend ostream &operator<<(ostream &os, const Expression &expr) { return expr->print(os); };
        bool operator==(const Expression &expr) const { return this->get()->equals(*expr.get()); };
    };

    class TwoNode : public Node
    {
    public:
        TwoNode(const Expression &expr1, const Expression &expr2) : expr1(expr1), expr2(expr2){};
        const Expression expr1;
        const Expression expr2;
    };

    class PlusNode : public TwoNode
    {
    public:
        PlusNode(const Expression &expr1, const Expression &expr2) : TwoNode(expr1, expr2){};
        virtual ostream &print(ostream &os) override { return os << "(" << expr1 << "+" << expr2 << ")"; };
    };
    class MultNode : public TwoNode
    {
    public:
        MultNode(const Expression &expr1, const Expression &expr2) : TwoNode(expr1, expr2){};
        virtual ostream &print(ostream &os) override { return os << "(" << expr1 << "*" << expr2 << ")"; };
    };

    class Sym : public Expression
    {
    public:
        Sym(const string &name) : Expression(Expression::make_new(SymNode(name))){};
    };
    class Const : public Expression
    {
    public:
        Const(const double value) : Expression(Expression::make_new(ConstNode(value))){};
    };
    class Zero : public Expression
    {
    public:
        Zero() : Expression(Expression::make_new(ZeroNode())){};
    };

    Expression operator+(const Expression &expr1, const Expression &expr2)
    {
        // simplificiations
        if (expr1 == Zero())
        {
            return expr2;
        }
        if (expr2 == Zero())
        {
            return expr1;
        }
        return Expression::make_new(PlusNode(expr1, expr2));
    }

    Expression operator*(const Expression &expr1, const Expression &expr2)
    {
        return Expression::make_new(MultNode(expr1, expr2));
    }

    vector<Expression> GetTerms(const Expression &expr)
    {
        vector<Expression> terms;
        auto plus_node = dynamic_pointer_cast<PlusNode>(expr);
        if (plus_node)
        {
            auto expr1 = plus_node->expr1;
            auto expr2 = plus_node->expr2;
            auto terms1 = GetTerms(expr1);
            auto terms2 = GetTerms(expr2);
            terms.insert(terms.end(), terms1.begin(), terms1.end());
            terms.insert(terms.end(), terms2.begin(), terms2.end());
        }
        else
        {
            terms.push_back(expr);
        }
        return terms;
    };

    vector<Expression> GetCoefficients(const Expression &expr, const vector<Sym> &sym_vec)
    {
        vector<Expression> coefficients(sym_vec.size(), Zero());
        // get the terms of the expression
        vector<Expression> terms = GetTerms(expr);
        // iterate over all symbols
        for (int i = 0; i < sym_vec.size(); i++)
        {
            auto sym = sym_vec.at(i);
            // iterate over all terms
            for (auto term : terms)
            {
                // check if the term contains the symbol
                auto mult_node = dynamic_pointer_cast<MultNode>(term);
                if (mult_node)
                {
                    auto expr1 = mult_node->expr1;
                    auto expr2 = mult_node->expr2;
                    if (expr1 == sym)
                    {
                        coefficients.at(i) = coefficients.at(i) + expr2;
                    }
                    else if (expr2 == sym)
                    {
                        coefficients.at(i) = coefficients.at(i) + expr1;
                    }
                }
                else if (term == sym)
                {
                    coefficients.at(i) = coefficients.at(i) + Const(1.0);
                }
                else
                {
                    // runtime error
                    runtime_error("Error in GetCoefficients");
                }
            }
        }

        return coefficients;
    };

    void OrderDepthFirstRecurse(const Expression expr, vector<Expression> &result)
    {
        // check if expr is already in result
        if (find(result.begin(), result.end(), expr) != result.end())
        {
            return;
        }
        // check if expression is a TwoNode
        auto two_node = dynamic_pointer_cast<TwoNode>(expr);
        if (two_node)
        {
            auto expr1 = two_node->expr1;
            auto expr2 = two_node->expr2;
            OrderDepthFirstRecurse(expr1, result);
            OrderDepthFirstRecurse(expr2, result);
        }
        result.push_back(expr);
    };
    class Function
    {
        enum Instruction
        {
            INPUT,
            OUTPUT,
            PLUS,
            MULT,
            CONST
        };
        struct AlgEl
        {
            Instruction ins;
            int arg1;
            int arg2;
            double val;
        };
        Function(vector<Expression> input, Expression output)
        {
            vector<Expression> ordered_expression;
            OrderDepthFirstRecurse(output, ordered_expression);
            algorithm.reserve(ordered_expression.size());
            work.reserve(ordered_expression.size());
            for (auto expr : ordered_expression)
            {
                // check if expr is a Sym
                auto sym_node = dynamic_pointer_cast<SymNode>(expr);
                if (sym_node)
                {
                    // find the index of expr in input
                    auto it = find(input.begin(), input.end(), expr);
                    // check if expr is in input
                    if (it >= input.end())
                    {
                        // runtime error
                        runtime_error("Error in Function constructor");
                    }
                    // convert it find output to index
                    int index = it - input.begin();
                    AlgEl el({INPUT, index, 0, 0.0});
                }
                // check if expr is a Const
                auto const_node = dynamic_pointer_cast<ConstNode>(expr);
                if (const_node)
                {
                    AlgEl el({CONST, 0, 0, const_node->value});
                }
                // check if expr is a Zero
                auto zero_node = dynamic_pointer_cast<ZeroNode>(expr);
                if (zero_node)
                {
                    AlgEl el({CONST, 0, 0, 0.0});
                }
                // check if expr is a PlusNode
                auto plus_node = dynamic_pointer_cast<PlusNode>(expr);
                // check if expr is a MultNode
                auto mult_node = dynamic_pointer_cast<MultNode>(expr);
                if (mult_node || plus_node)
                {
                    // find the index of expr1 in ordered_expression
                    auto it = find(ordered_expression.begin(), ordered_expression.end(), mult_node->expr1);

                    // check if expr1 is in ordered_expression
                    if (it >= ordered_expression.end())
                    {
                        // runtime error
                        runtime_error("Error in Function constructor");
                    }
                    // convert it find output to index
                    int index1 = it - ordered_expression.begin();
                    // find the index of expr2 in ordered_expression
                    it = find(ordered_expression.begin(), ordered_expression.end(), mult_node->expr2);
                    // check if expr2 is in ordered_expression
                    if (it >= ordered_expression.end())
                    {
                        // runtime error
                        runtime_error("Error in Function constructor");
                    }
                    // convert it find output to index
                    int index2 = it - ordered_expression.begin();
                    AlgEl el({mult_node?MULT:PLUS, index1, index2, 0.0});
                }
            }
            // add the output instruction
            AlgEl el({OUTPUT, 0, 0, 0.0});
        }
        // call the function
        double Eval(const vector<double>& input)
        {
            work.resize(0);
            for (auto el : algorithm)
            {
                switch (el.ins)
                {
                case INPUT:
                    work.push_back(input.at(el.arg1));
                    break;
                case OUTPUT:
                    return work.back();
                    break;
                case PLUS:
                    work.push_back(work.at(el.arg1) + work.at(el.arg2));
                    break;
                case MULT:
                    work.push_back(work.at(el.arg1) * work.at(el.arg2));
                    break;
                case CONST:
                    work.push_back(el.val);
                    break;
                default:
                    // runtime error
                    runtime_error("Error in Function Eval");
                    break;
                }
            }
        }
        vector<AlgEl> algorithm;
        vector<double> work;
    };

    // matrix stuff
    class Matrix
    {
    public:
        Matrix(const int n_rows, const int n_cols) : n_rows_(n_rows), n_cols_(n_cols)
        {
            data.resize(n_rows * n_cols, Zero());
        };
        Expression &operator()(int i, int j) { return data[i + n_rows_ * j]; };
        const Expression &operator()(int i, int j) const { return data[i + n_rows_ * j]; };
        int n_rows() const
        {
            return n_rows_;
        };
        int n_cols() const
        {
            return n_cols_;
        };

    protected:
        vector<Expression> data;
        const int n_rows_;
        const int n_cols_;
    };
    ostream &operator<<(ostream &os, const Matrix &A)
    {
        for (int i = 0; i < A.n_rows(); i++)
        {
            for (int j = 0; j < A.n_cols(); j++)
            {
                os << A(i, j) << " ";
            }
            os << endl;
        }
        return os;
    }
    class Zerom : public Matrix
    {
    public:
        Zerom(int n_rows, int n_cols) : Matrix(n_rows, n_cols)
        {
            for (int i = 0; i < n_rows; i++)
            {
                for (int j = 0; j < n_cols; j++)
                {
                    (*this)(i, j) = Zero();
                }
            }
        };
    };
    class Eye : public Matrix
    {
    public:
        Eye(int n_rows) : Matrix(n_rows, n_rows)
        {
            for (int i = 0; i < n_rows; i++)
            {
                for (int j = 0; j < n_rows; j++)
                {
                    if (i == j)
                    {
                        (*this)(i, j) = Const(1.0);
                    }
                    else
                    {
                        (*this)(i, j) = Zero();
                    }
                }
            }
        };
    };
    class SymMatrix : public Matrix
    {
    public:
        SymMatrix(const string &name, const int n_rows, const int n_cols) : Matrix(n_rows, n_cols)
        {
            for (int i = 0; i < n_rows; i++)
            {
                for (int j = 0; j < n_cols; j++)
                {
                    (*this)(i, j) = Sym(name + string("(") + to_string(i) + string(",") + to_string(j) + string(")"));
                }
            }
        };
    };

    Matrix operator*(const Matrix &A, const Matrix &B)
    {
        int n_rows = A.n_rows();
        int n_cols = B.n_cols();
        int n_inner = A.n_cols();
        Zerom C(n_rows, n_cols);
        for (int i = 0; i < n_rows; i++)
        {
            for (int j = 0; j < n_cols; j++)
            {
                for (int k = 0; k < n_inner; k++)
                {
                    C(i, j) = C(i, j) + A(i, k) * B(k, j);
                }
            }
        }
        return C;
    }
    Matrix operator+(const Matrix &A, const Matrix &B)
    {
        int n_rows = A.n_rows();
        int n_cols = A.n_cols();
        Zerom C(n_rows, n_cols);
        for (int i = 0; i < n_rows; i++)
        {
            for (int j = 0; j < n_cols; j++)
            {
                C(i, j) = A(i, j) + B(i, j);
            }
        }
        return C;
    }
}
#endif // NODEINCLUDED
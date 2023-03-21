#ifndef NODEINCLUDED
#define NODEINCLUDED

#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include <type_traits>
using namespace std;
namespace SpaLS
{
    class Node
    {
    public:
        // Node();
        // ~Node();
        virtual ostream &print(ostream &os) { return os << "Node"; };
        virtual bool is_sym() { return false; };
        virtual bool is_const() { return false; };
        virtual bool is_zero() { return false; };
        virtual bool is_plus() { return false; };
        virtual bool is_mult() { return false; };
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
        bool is_zero() override { return true; };
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
    class PlusNode : public Node
    {
    public:
        PlusNode(const Expression &expr1, const Expression &expr2) : expr1(expr1), expr2(expr2){};
        const Expression expr1;
        const Expression expr2;
        virtual ostream &print(ostream &os) override { return os << "(" << expr1 << "+" << expr2 << ")"; };
        bool is_plus() override { return true; };
    };
    class MultNode : public Node
    {
    public:
        MultNode(const Expression &expr1, const Expression &expr2) : expr1(expr1), expr2(expr2){};
        const Expression expr1;
        const Expression expr2;
        virtual ostream &print(ostream &os) override { return os << "(" << expr1 << "*" << expr2 << ")"; };
        bool is_mult() override { return true; };
    };

    Expression operator+(const Expression &expr1, const Expression &expr2)
    {
        return Expression::make_new(PlusNode(expr1, expr2));
    }
    
    Expression operator*(const Expression &expr1, const Expression &expr2)
    {
        return Expression::make_new(MultNode(expr1, expr2));
    }

    Expression GetCoeff(const Expression& expr, const Sym& sym)
    {
        if(expr -> is_plus())
        {
            cout << "is plus" << endl;
        }
        return Zero();
    }
    
    // vector<Expression> GetCoefficients(const Expression& exprs, const vector<Sym>& vars)
    // {
    //     // Expression 
    //     // return coeffs;
    // };




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
                    (*this)(i, j) = Sym(name + string("(") + to_string(i) + string(",") + to_string(j)+ string(")") );
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
#ifndef EXPR_HPP
#define EXPR_HPP

#include "context.hpp"

class Expr {
  public:
    class Visitor;
    const Type *type;

    virtual ~Expr() = default;
    virtual void accept(Visitor&) = 0;
    virtual int weight() = 0;
    virtual int height() = 0;
    virtual int eval() = 0;

    const Type* getType() { return type; }
};

class Expr::Visitor {
  public:
    virtual ~Visitor() = default;
    virtual void visit(Bool_Expr*) = 0;
    virtual void visit(And_Expr*) = 0;
    virtual void visit(Or_Expr*) = 0;
    virtual void visit(Xor_Expr*) = 0;
    virtual void visit(Not_Expr*) = 0;
    virtual void visit(Eq_Expr*) = 0;
    virtual void visit(NotEq_Expr*) = 0;
    virtual void visit(Cond_Expr*) = 0;
    virtual void visit(AndThen_Expr*) = 0;
    virtual void visit(OrElse_Expr*) = 0;

    virtual void visit(Int_Expr*) = 0;
    virtual void visit(Add_Expr*) = 0;
    virtual void visit(Sub_Expr*) = 0;
    virtual void visit(Mult_Expr*) = 0;
    virtual void visit(Div_Expr*) = 0;
    virtual void visit(Mod_Expr*) = 0;
    virtual void visit(LessThan_Expr*) = 0;
    virtual void visit(GreaterThan_Expr*) = 0;
    virtual void visit(LessEqThan_Expr*) = 0;
    virtual void visit(GreaterEqThan_Expr*) = 0;
    virtual void visit(Negation_Expr*) = 0;
};
#endif

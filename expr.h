#ifndef EXPR_H
#define EXPR_H

#include <algorithm>
#include <iostream>
#include "type.h"

// Bool Expressions
class Bool_Expr;
class And_Expr;
class Or_Expr;
class Xor_Expr;
class Not_Expr;
class Eq_Expr;
class NotEq_Expr;
class Cond_Expr;
class AndThen_Expr;
class OrElse_Expr;

// Int Expressions
class Int_Expr;
class Add_Expr;
class Sub_Expr;
class Mult_Expr;
class Div_Expr;
class Mod_Expr;
class LessThan_Expr;
class GreaterThan_Expr;
class LessEqThan_Expr;
class GreaterEqThan_Expr;
class Negation_Expr;

//class Type;
Type_Exception Type_Error;

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

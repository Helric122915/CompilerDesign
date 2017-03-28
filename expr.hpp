#ifndef EXPR_HPP
#define EXPR_HPP

#include "context.hpp"

// Base Expr class that defines an expression and will be inherited from by all expressions.
class Expr {
public:
  class Visitor;
  const Type *type;

  virtual ~Expr() = default;
  
  // Accept visitor function that allows a visitor to interact with an expression.
  virtual void accept(Visitor&) = 0;

  // Pure virtual functions to be overriden in a derived class.
  //virtual int weight() = 0;
  //virtual int height() = 0;
  //virtual int eval() = 0;
  
  const Type* getType() { return type; }
};

// Based Visitor Class in the Expr class that will be inherited in each visitor function implementation.
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
  virtual void visit(OneComplement_Expr*) = 0;
};
#endif

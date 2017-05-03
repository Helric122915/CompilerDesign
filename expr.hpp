#ifndef EXPR_HPP
#define EXPR_HPP

#include "context.hpp"

class Name_Decl;
void print(Expr*);

// Base Expr class that defines an expression and will be inherited from by all expressions.
class Expr {
public:
  class Visitor;
  //const Type *type;
  Type *type;

  virtual ~Expr() = default;
  
  // Accept visitor function that allows a visitor to interact with an expression.
  virtual void accept(Visitor&) = 0;

  //const Type* getType() { return type; }
  Type* getType() { return type; }
};

// Based Visitor Class in the Expr class that will be inherited in each visitor function implementation.
class Expr::Visitor {
public:
  virtual ~Visitor() = default;
  virtual void visit(Call_Expr*) = 0;
  virtual void visit(Assign_Expr*) = 0;
  virtual void visit(Value_Expr*) = 0;
  virtual void visit(Ref_Expr*) = 0;
  virtual void visit(Init_Expr*) = 0;
  virtual void visit(Bind_Expr*) = 0;
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

class Initializer {
private:
public:
  Initializer(Var_Decl* d) : init(d) { }

  Var_Decl* init;
};

class Call_Expr : public Expr {
private:
  Expr* function;
  std::vector<Expr*> arguments;

public:
  Call_Expr(Expr* fn, Type* t, std::vector<Expr*>& args) : function(fn), arguments(args) { this->type = t; }
  Call_Expr(Expr* fn, Type* t, std::vector<Expr*>&& args) : function(fn), arguments(std::move(args)) { this->type = t; }

  void accept(Visitor& v) { return v.visit(this); }

  Expr* getFunction() { return function; }
  std::vector<Expr*> getArguments() { return arguments; }
};

class Assign_Expr : public Expr {
private:
  Expr* e1;
  Expr* e2;

public:
  Assign_Expr(Expr* e1, Expr* e2, Type* t) : e1(e1), e2(e2) { this->type = t; }

  void accept(Visitor& v) { return v.visit(this); }

  Expr* getE1() { return e1; }
  Expr* getE2() { return e2; }
};

class Ref_Expr : public Expr {
private:
  Name_Decl* d;

public:
  Ref_Expr(Name_Decl* d, Type* t) : d(d) { this->type = t; }

  Name_Decl* getDecl() { return d; }

  // Overriding of accept virtual function to accept visitors.
  void accept(Visitor& v) { return v.visit(this); }
};

class Value_Expr : public Expr {
private:
  Expr* e;

public:
  Value_Expr(Expr* e, Type* t) : e(e) { this->type = t; print(this); std::cout << "<- Value_Expr\n"; }

  Expr* getE() { return e; }

  // Overriding of accept virtual function to accept visitors.
  void accept(Visitor& v) { return v.visit(this); }
};

class Init_Expr : public Expr, public Initializer {
private:
  Expr* e;

public:
  Init_Expr(Expr* e, Var_Decl* d, Type* t) : Initializer(d), e(e) { this->type = t; }

  Expr* getE() { return e; }

  // Overriding of accept virtual function to accept visitors.
  void accept(Visitor& v) { return v.visit(this); }
};

class Bind_Expr : public Expr, public Initializer {
private:
  Expr* e;

public:
  Bind_Expr(Expr* e, Var_Decl* d, Type* t) : Initializer(d), e(e) { this->type = t; }

  Expr* getE() { return e; }

  // Overriding of accept virtual function to accept visitors.
  void accept(Visitor& v) { return v.visit(this); }
};
#endif

#ifndef BOOL_EXPR_HPP
#define BOOL_EXPR_HPP

#include "expr.hpp"

// The expressions true and false have type bool and the math values "true" and "false", respectively.
class Bool_Expr : public Expr {
private:
  bool value;

public:
  Bool_Expr(bool value, ASTcontext* cxt) : value(value) { this->type = (*cxt).Bool_; }
  ~Bool_Expr() = default;

  // Accessor function to retrieve the private variable value.
  int getValue() { return value; }

  // Overriding of accept virtual function to accept visitors.
  void accept(Visitor& v) { return v.visit(this); }
};

// The operand of not e shall have type bool, and the type of the expression is bool.
class Not_Expr : public Expr {
private:
  Expr *e;

public:
  Not_Expr(Expr *e, ASTcontext* cxt) : e(e) { this->type = (*cxt).Bool_; }
  ~Not_Expr() = default;

  // Accessor function to retrieve the private variable e.
  Expr* getE() { return e; }

  // Overriding of accept virtual function to accept visitors.
  void accept(Visitor& v) { return v.visit(this); }
};

// The operands of e1 and e2 shall have type bool. The result type of and is bool.
class And_Expr : public Expr {
private:
  Expr *e1, *e2;

public:
  And_Expr(Expr *e1, Expr *e2, ASTcontext* cxt) : e1(e1), e2(e2) { this->type = e1->type; }
  ~And_Expr() = default;

  // Accessor functions to retrieve the private variables e1 and e2.
  Expr* getE1() { return e1; }
  Expr* getE2() { return e2; }

  // Overriding of accept virtual function to accept visitors.
  void accept(Visitor& v) { return v.visit(this); }
};

// The operands of e1 or e2 shall have type bool. The result type of or is bool.
class Or_Expr : public Expr {
private:
  Expr *e1, *e2;

public:
  Or_Expr(Expr *e1, Expr *e2, ASTcontext* cxt) : e1(e1), e2(e2) { this->type = e1->type; }
  ~Or_Expr() = default;

  // Accessor functions to retrieve the private variables e1 and e2.
  Expr* getE1() { return e1; }
  Expr* getE2() { return e2; }

  // Overriding of accept virtual function to accept visitors.
  void accept(Visitor& v) { return v.visit(this); }
};

// The operands of e1 xor e2 shall have type bool. The result type of xor is bool.
class Xor_Expr : public Expr {
private:
  Expr *e1, *e2;

public:
  Xor_Expr(Expr *e1, Expr *e2, ASTcontext* cxt) : e1(e1), e2(e2) { this->type = e1->type; }
  ~Xor_Expr() = default;

  // Accessor functions to retrieve the private variables e1 and e2.
  Expr* getE1() { return e1; }
  Expr* getE2() { return e2; }

  // Overriding of accept virtual function to accept visitors.
  void accept(Visitor& v) { return v.visit(this); }
};

// The operands of e1 == e2 shall have the same type. The result type of == is bool.
class Eq_Expr : public Expr {
private:
  Expr *e1, *e2;

public:
  Eq_Expr(Expr *e1, Expr *e2, ASTcontext* cxt) : e1(e1), e2(e2) { this->type = (*cxt).Bool_; }
  ~Eq_Expr() = default;

  // Accessor functions to retrieve the private variables e1 and e2.
  Expr* getE1() { return e1; }
  Expr* getE2() { return e2; }

  // Overriding of accept virtual function to accept visitors.
  void accept(Visitor& v) { return v.visit(this); }
};

// The operands of e1 != e shall have the same type. The result type of != is bool.
class NotEq_Expr : public Expr {
private:
  Expr *e1, *e2;

public:
  NotEq_Expr(Expr *e1, Expr *e2, ASTcontext* cxt) : e1(e1), e2(e2) { this->type = (*cxt).Bool_; }
  ~NotEq_Expr() = default;

  // Accessor functions to retrieve the private variables e1 and e2.
  Expr* getE1() { return e1; }
  Expr* getE2() { return e2; }

  // Overriding of accept virtual function to accept visitors.
  void accept(Visitor& v) { return v.visit(this); }
};

// The operands of e1 ? e2 : e3, e1 shall have type bool, e2 and e3 shall have the same type.
// The result type of e1 ? e2 : e3 shall have the type of e2 and e3.
class Cond_Expr : public Expr {
private:
  Expr *e1, *e2, *e3;

public:
  Cond_Expr(Expr *e1, Expr *e2, Expr *e3, ASTcontext* cxt) : e1(e1), e2(e2), e3(e3) { this->type = e2->type; }
  ~Cond_Expr() = default;

  // Accessor functions to retrieve the private variables e1, e2 and e3.
  Expr* getE1() { return e1; }
  Expr* getE2() { return e2; }
  Expr* getE3() { return e3; }

  // Overriding of accept virtual function to accept visitors.
  void accept(Visitor& v) { return v.visit(this); }
};

// The operands of e1 AndThen e2 shall have type bool. The result type of AndThen shall have type bool.
class AndThen_Expr : public Expr {
private:
  Expr *e1, *e2;

public:
  AndThen_Expr(Expr *e1, Expr *e2, ASTcontext* cxt) : e1(e1), e2(e2) { this->type = (*cxt).Bool_; }
  ~AndThen_Expr() = default;

  // Accessor functions to retrieve the private variables e1 and e2.
  Expr* getE1() { return e1; }
  Expr* getE2() { return e2; }

  // Overriding of accept virtual function to accept visitors.
  void accept(Visitor& v) { return v.visit(this); }
};

// The operands of e1 OrElse e2 shall have type bool. The result type of OrElse shall have type bool.
class OrElse_Expr : public Expr {
private:
  Expr *e1, *e2;

public:
  OrElse_Expr(Expr *e1, Expr *e2, ASTcontext* cxt) : e1(e1), e2(e2) { this->type = (*cxt).Bool_; }
  ~OrElse_Expr() = default;

  // Accessor functions to retrieve the private variables e1 and e2.
  Expr* getE1() { return e1; }
  Expr* getE2() { return e2; }

  // Overriding of accept virtual function to accept visitors.
  void accept(Visitor& v) { return v.visit(this); }
};
#endif

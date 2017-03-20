#ifndef INT_EXPR_HPP
#define INT_EXPR_HPP

#include "expr.hpp"

// Integer literals have type int. The value of an integer literal is the one indicated by
// the expression.
class Int_Expr : public Expr {
private:
  int value, rep;

public:
  Int_Expr(int value, int rep, ASTcontext &cxt) : value(value), rep(rep) {
    this->type = cxt.Int_;
  }
  ~Int_Expr() = default;
  
  // Accessor function to retrieve the private variable value.
  int getValue() { return value; }
  int getRep() { return rep; }  
  // Overriding of accept virtual function to accept visitors.
  void accept(Visitor& v) { return v.visit(this); }
  
  // Overriding of virtual functions to access the expression.
  int weight() override { return 1; }
  int height() override { return 0; }
  int eval() override { return value; }
};

// The operands of e1 + e2 shall have type int, and the type of the expression is int.
class Add_Expr : public Expr {
private:
  Expr *e1, *e2;
  
public:
  Add_Expr(Expr *e1, Expr *e2, ASTcontext &cxt) : e1(e1), e2(e2) {
    if (e1->type == cxt.Int_ && e2->type == cxt.Int_)
      this->type = cxt.Int_;
    else
      throw Type_Exception("Add_Expr");
  }
  ~Add_Expr() = default;
  
  // Accessor functions to retrieve the private variables e1 and e2.
  Expr* getE1() { return e1; }
  Expr* getE2() { return e2; }
  
    // Overriding of accept virtual function to accept visitors.
  void accept(Visitor& v) { return v.visit(this); }
  
  // Overriding of virtual functions to access the expression.
  int weight() override { return 1 + e1->weight() + e2->weight(); }
  int height() override { return 1 + std::max(e1->height(),e2->height()); }
  int eval();
};

// Checks to see if the math operation works within the scale of type int.
int Add_Expr::eval() {
  int e1Val = e1->eval();
  int e2Val = e2->eval();
  
  if (e1Val > 0 && e2Val > 0)
    if (std::numeric_limits<int>::max() -  e1Val < e2Val)
      throw Overflow_Exception("Addition result too large.");
    else if (e1Val < 0 && e2Val < 0)
      if (std::numeric_limits<int>::min() - e1Val > e2Val)
	throw Overflow_Exception("Addition result too small.");
  
  return e1Val + e2Val;
}

// The operands of e1 - e2 shall have type int, and the type of the expression is int.
class Sub_Expr : public Expr {
private:
  Expr *e1, *e2;

public:
  Sub_Expr(Expr *e1, Expr *e2, ASTcontext &cxt) : e1(e1), e2(e2) {
    if (e1->type == cxt.Int_ && e2->type == cxt.Int_)
      this->type = cxt.Int_;
      else
        throw Type_Exception("Sub_Expr");
  }
  ~Sub_Expr() = default;
  
  // Accessor functions to retrieve the private variables e1 and e2.
  Expr* getE1() { return e1; }
  Expr* getE2() { return e2; }
  
  // Overriding of accept virtual function to accept visitors.
  void accept(Visitor& v) { return v.visit(this); }
  
  // Overriding of virtual functions to access the expression.
  int weight() override { return 1 + e1->weight() + e2->weight(); }
  int height() override { return 1 + std::max(e1->height(),e2->height()); }
  int eval();
};

// Checks to see if the math operation works within the scale of type int.
int Sub_Expr::eval() {
  int e1Val = e1->eval();
  int e2Val = e2->eval();
  
  if (e1Val > 0 && e2Val < 0)
    if (std::numeric_limits<int>::max() - e1Val < e2Val)
      throw Overflow_Exception("Subtraction result too large.");
  if (e1Val < 0 && e2Val > 0)
    if (std::numeric_limits<int>::min() - e1Val  > -e2Val)
      throw Overflow_Exception("Subtraction result too small.");
  
  return e1Val - e2Val;
}

// The operands of e1 * e2 shall have type int, and the type of the expression is int.
class Mult_Expr : public Expr {
private:
  Expr *e1, *e2;
  
public:
  Mult_Expr(Expr *e1, Expr *e2, ASTcontext &cxt) : e1(e1), e2(e2) {
    if (e1->type == cxt.Int_ && e2->type == cxt.Int_)
      this->type = cxt.Int_;
    else
      throw Type_Exception("Mult_Expr");
  }
  ~Mult_Expr() = default;
  
  // Accessor functions to retrieve the private variables e1 and e2.
  Expr* getE1() { return e1; }
  Expr* getE2() { return e2; }
  
  // Overriding of accept virtual function to accept visitors.
  void accept(Visitor& v) { return v.visit(this); }
  
  // Overriding of virtual functions to access the expression.
  int weight() override { return 1 + e1->weight() + e2->weight(); }
  int height() override { return 1 + std::max(e1->height(),e2->height()); }
  int eval();
};

// Checks to see if the math operation works within the scale of type int.
int Mult_Expr::eval() {
  int e1Val = e1->eval();
  int e2Val = e2->eval();
  
  if (e1Val == 0 || e2Val == 0)
    return 0;
  else if ((e1Val > 0 && e2Val > 0) || (e1Val < 0 && e2Val < 0))
    if (std::numeric_limits<int>::max()/abs(e1Val) < abs(e2Val))
      throw Overflow_Exception("Multiplaction result too large.");
    else if (e1Val > 0 && e2Val < 0)
      if (std::numeric_limits<int>::min()/e1Val > e2Val)
	throw Overflow_Exception("Multiplication result too small.");
      else if (e1Val < 0 && e2Val > 0)
	if (std::numeric_limits<int>::min()/e2Val > e1Val)
	  throw Overflow_Exception("Multiplication result too small.");
  
  return e1Val * e2Val;
}

// The operands of e1 / e2 shall have type int, and the type of the expression is int.
class Div_Expr : public Expr {
private:
  Expr *e1, *e2;
  
  public:
  Div_Expr(Expr *e1, Expr *e2, ASTcontext &cxt) : e1(e1), e2(e2) {
    if (e1->type == cxt.Int_ && e2->type == cxt.Int_)
      this->type = cxt.Int_;
    else
      throw Type_Exception("Div_Expr");
  }
  ~Div_Expr() = default;
  
  // Accessor functions to retrieve the private variables e1 and e2.
  Expr* getE1() { return e1; }
  Expr* getE2() { return e2; }
  
  // Overriding of accept virtual function to accept visitors.
  void accept(Visitor& v) { return v.visit(this); }
  
    // Overriding of virtual functions to access the expression.
  int weight() override { return 1 + e1->weight() + e2->weight(); }
  int height() override { return 1 + std::max(e1->height(),e2->height()); }
  int eval();
};

// Checks to see if the math operation works within the scale of type int.
int Div_Expr::eval() {
  int e1Val = e1->eval();
  int e2Val = e2->eval();
  
  if (e2Val == 0)
    throw Overflow_Exception("Cannot divide by zero.");
  else if (e1Val == std::numeric_limits<int>::min() && e2Val == -1)
    throw Overflow_Exception("Largest negative value is 1 larger than largest positive.");
  
  return e1Val / e2Val;
}

// The operands of e1 & e2 shall have type int, and the type of the expression is int.
class Mod_Expr : public Expr {
private:
  Expr *e1, *e2;
  
public:
  Mod_Expr(Expr *e1, Expr *e2, ASTcontext &cxt) : e1(e1), e2(e2) {
      if (e1->type == cxt.Int_ && e2->type == cxt.Int_)
        this->type = cxt.Int_;
      else
        throw Type_Exception("Mod_Expr");
  }
  ~Mod_Expr() = default;
  
  // Accessor functions to retrieve the private variables e1 and e2.
  Expr* getE1() { return e1; }
  Expr* getE2() { return e2; }
  
  // Overriding of accept virtual function to accept visitors.
    void accept(Visitor& v) { return v.visit(this); }
  
  // Overriding of virtual functions to access the expression.
  int weight() override { return 1 + e1->weight() + e2->weight(); }
  int height() override { return 1 + std::max(e1->height(),e2->height()); }
  int eval();
};

// Checks to see if the math operation works within the scale of type int.
int Mod_Expr::eval() {
  int e1Val = e1->eval();
  int e2Val = e2->eval();
  
  if (e2Val == 0)
    throw Overflow_Exception("Cannot divide by zero.");
  
  return e1Val % e2Val;
}

// The operands of e1 < e2 shall have type int, and the type of the expression is bool.
class LessThan_Expr : public Expr {
private:
  Expr *e1, *e2;
  
public:
    LessThan_Expr(Expr *e1, Expr *e2, ASTcontext &cxt) : e1(e1), e2(e2) {
      if (e1->type == cxt.Int_ && e2->type == cxt.Int_)
        this->type = cxt.Bool_;
      else
        throw Type_Exception("LessThan_Expr");
    }
  ~LessThan_Expr() = default;
  
  // Accessor functions to retrieve the private variables e1 and e2.
  Expr* getE1() { return e1; }
  Expr* getE2() { return e2; }
  
  // Overriding of accept virtual function to accept visitors.
  void accept(Visitor& v) { return v.visit(this); }
  
  // Overriding of virtual functions to access the expression.
  int weight() override { return 1 + e1->weight() + e2->weight(); }
  int height() override { return 1 + std::max(e1->height(),e2->height()); }
  int eval() override { return e1->eval() < e2->eval(); }
};

// The operands of e1 > e2 shall have type int, and the type of the expression is bool.
class GreaterThan_Expr : public Expr {
private:
  Expr *e1, *e2;
  
public:
  GreaterThan_Expr(Expr *e1, Expr *e2, ASTcontext &cxt) : e1(e1), e2(e2) {
      if (e1->type == cxt.Int_ && e2->type == cxt.Int_)
        this->type = cxt.Bool_;
      else
        throw Type_Exception("GreaterThan_Expr");
  }
  ~GreaterThan_Expr() = default;

  // Accessor functions to retrieve the private variables e1 and e2.
  Expr* getE1() { return e1; }
  Expr* getE2() { return e2; }
  
  // Overriding of accept virtual function to accept visitors.
  void accept(Visitor& v) { return v.visit(this); }
  
  // Overriding of virtual functions to access the expression.
    int weight() override { return 1 + e1->weight() + e2->weight(); }
  int height() override { return 1 + std::max(e1->height(),e2->height()); }
  int eval() override { return e1->eval() > e2->eval(); }
};

// The operands of e1 <= e2 shall have type int, and the type of the expression is bool.
class LessEqThan_Expr : public Expr {
private:
  Expr *e1, *e2;

public:
  LessEqThan_Expr(Expr *e1, Expr *e2, ASTcontext &cxt) : e1(e1), e2(e2) {
    if (e1->type == cxt.Int_ && e2->type == cxt.Int_)
      this->type = cxt.Bool_;
    else
      throw Type_Exception("LessEqThan_Expr");
  }
  ~LessEqThan_Expr() = default;

  // Accessor functions to retrieve the private variables e1 and e2.
  Expr* getE1() { return e1; }
  Expr* getE2() { return e2; }

  // Overriding of accept virtual function to accept visitors.
  void accept(Visitor& v) { return v.visit(this); }

  // Overriding of virtual functions to access the expression.
  int weight() override { return 1 + e1->weight() + e2->weight(); }
  int height() override { return 1 + std::max(e1->height(),e2->height()); }
  int eval() override { return e1->eval() <= e2->eval(); }
};

// The operands of e1 >= e2 shall have type int, and the type of the expression is bool.
class GreaterEqThan_Expr : public Expr {
private:
  Expr *e1, *e2;

public:
  GreaterEqThan_Expr(Expr *e1, Expr *e2, ASTcontext &cxt) : e1(e1), e2(e2) {
    if (e1->type == cxt.Int_ && e2->type == cxt.Int_)
      this->type = cxt.Bool_;
    else
      throw Type_Exception("GreaterEqThan_Expr");
  }
  ~GreaterEqThan_Expr() = default;

  // Accessor functions to retrieve the private variables e1 and e2.
  Expr* getE1() { return e1; }
  Expr* getE2() { return e2; }

  // Overriding of accept virtual function to accept visitors.
  void accept(Visitor& v) { return v.visit(this); }

  // Overriding of virtual functions to access the expression.
  int weight() override { return 1 + e1->weight() + e2->weight(); }
  int height() override { return 1 + std::max(e1->height(),e2->height()); }
  int eval() override { return e1->eval() >= e2->eval(); }
};

// The operand of -e shall have type int, and the type of the expression is int.
class Negation_Expr : public Expr {
private:
  Expr *e;

public:
  Negation_Expr(Expr *e, ASTcontext &cxt) : e(e) {
    if (e->type == cxt.Int_)
      this->type = cxt.Int_;
    else
      throw Type_Exception("Negation_Expr");
  }
  ~Negation_Expr() = default;

  // Accessor function to retrieve the private variable e.
  Expr* getE() { return e; }

  // Overriding of accept virtual function to accept visitors.
  void accept(Visitor& v) { return v.visit(this); }

  // Overriding of virtual functions to access the expression.
  int weight() override { return 1 + e->weight(); }
  int height() override { return 1 + e->height(); }
  int eval() override { return 0 - e->eval(); }
};

// The operand of ~e shall have type int, and the type of the expression is int.
class OneComplement_Expr : public Expr {
private:
  Expr *e;

public:
  OneComplement_Expr(Expr *e, ASTcontext &cxt) : e(e) {
    if (e->type == cxt.Int_)
      this->type = cxt.Int_;
    else if (e->type == cxt.Bool_)
      this->type = cxt.Bool_;
    else
      throw Type_Exception("OneComplement_Expr");
  }
  ~OneComplement_Expr() = default;

  // Accessor function to retreieve the private variable e.
  Expr* getE() { return e; }

  // Overriding of accept virtual function to accept visitors.
  void accept(Visitor& v) { return v.visit(this); }

  // Overriding of virtual functions to access the expression.
  int weight() override { return 1 + e->weight(); }
  int height() override { return 1 + e->height(); }
  int eval() override { return ~e->eval(); }
};
#endif

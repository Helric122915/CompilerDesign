#ifndef INT_EXPR_HPP
#define INT_EXPR_HPP

#include "expr.hpp"

// Integer literals have type int. The value of an integer literal is the one indicated by
// the expression.
class Int_Expr : public Expr {
private:
  int value, rep;

public:
  Int_Expr(int value, int rep, ASTcontext *cxt) : value(value), rep(rep) {
    this->type = (*cxt).Int_;
  }
  ~Int_Expr() = default;
  
  // Accessor function to retrieve the private variable value.
  int getValue() { return value; }
  int getRep() { return rep; }  

  // Overriding of accept virtual function to accept visitors.
  void accept(Visitor& v) { return v.visit(this); }
};

// The operands of e1 + e2 shall have type int, and the type of the expression is int.
class Add_Expr : public Expr {
private:
  Expr *e1, *e2;
  
public:
  Add_Expr(Expr *e1, Expr *e2, ASTcontext *cxt) : e1(e1), e2(e2) {
    if (e1->type == (*cxt).Int_ && e2->type == (*cxt).Int_)
      this->type = (*cxt).Int_;
    else
      throw Type_Exception("Add_Expr");
  }
  ~Add_Expr() = default;
  
  // Accessor functions to retrieve the private variables e1 and e2.
  Expr* getE1() { return e1; }
  Expr* getE2() { return e2; }
  
  // Overriding of accept virtual function to accept visitors.
  void accept(Visitor& v) { return v.visit(this); }
};

// The operands of e1 - e2 shall have type int, and the type of the expression is int.
class Sub_Expr : public Expr {
private:
  Expr *e1, *e2;

public:
  Sub_Expr(Expr *e1, Expr *e2, ASTcontext *cxt) : e1(e1), e2(e2) {
    if (e1->type == (*cxt).Int_ && e2->type == (*cxt).Int_)
      this->type = (*cxt).Int_;
      else
        throw Type_Exception("Sub_Expr");
  }
  ~Sub_Expr() = default;
  
  // Accessor functions to retrieve the private variables e1 and e2.
  Expr* getE1() { return e1; }
  Expr* getE2() { return e2; }
  
  // Overriding of accept virtual function to accept visitors.
  void accept(Visitor& v) { return v.visit(this); }
};

// The operands of e1 * e2 shall have type int, and the type of the expression is int.
class Mult_Expr : public Expr {
private:
  Expr *e1, *e2;
  
public:
  Mult_Expr(Expr *e1, Expr *e2, ASTcontext *cxt) : e1(e1), e2(e2) {
    if (e1->type == (*cxt).Int_ && e2->type == (*cxt).Int_)
      this->type = (*cxt).Int_;
    else
      throw Type_Exception("Mult_Expr");
  }
  ~Mult_Expr() = default;
  
  // Accessor functions to retrieve the private variables e1 and e2.
  Expr* getE1() { return e1; }
  Expr* getE2() { return e2; }
  
  // Overriding of accept virtual function to accept visitors.
  void accept(Visitor& v) { return v.visit(this); }
};

// The operands of e1 / e2 shall have type int, and the type of the expression is int.
class Div_Expr : public Expr {
private:
  Expr *e1, *e2;
  
  public:
  Div_Expr(Expr *e1, Expr *e2, ASTcontext *cxt) : e1(e1), e2(e2) {
    if (e1->type == (*cxt).Int_ && e2->type == (*cxt).Int_)
      this->type = (*cxt).Int_;
    else
      throw Type_Exception("Div_Expr");
  }
  ~Div_Expr() = default;
  
  // Accessor functions to retrieve the private variables e1 and e2.
  Expr* getE1() { return e1; }
  Expr* getE2() { return e2; }
  
  // Overriding of accept virtual function to accept visitors.
  void accept(Visitor& v) { return v.visit(this); }
};

// The operands of e1 & e2 shall have type int, and the type of the expression is int.
class Mod_Expr : public Expr {
private:
  Expr *e1, *e2;
  
public:
  Mod_Expr(Expr *e1, Expr *e2, ASTcontext *cxt) : e1(e1), e2(e2) {
      if (e1->type == (*cxt).Int_ && e2->type == (*cxt).Int_)
        this->type = (*cxt).Int_;
      else
        throw Type_Exception("Mod_Expr");
  }
  ~Mod_Expr() = default;
  
  // Accessor functions to retrieve the private variables e1 and e2.
  Expr* getE1() { return e1; }
  Expr* getE2() { return e2; }
  
  // Overriding of accept virtual function to accept visitors.
    void accept(Visitor& v) { return v.visit(this); }
};

// The operands of e1 < e2 shall have type int, and the type of the expression is bool.
class LessThan_Expr : public Expr {
private:
  Expr *e1, *e2;
  
public:
    LessThan_Expr(Expr *e1, Expr *e2, ASTcontext *cxt) : e1(e1), e2(e2) {
      if (e1->type == (*cxt).Int_ && e2->type == (*cxt).Int_)
        this->type = (*cxt).Bool_;
      else
        throw Type_Exception("LessThan_Expr");
    }
  ~LessThan_Expr() = default;
  
  // Accessor functions to retrieve the private variables e1 and e2.
  Expr* getE1() { return e1; }
  Expr* getE2() { return e2; }
  
  // Overriding of accept virtual function to accept visitors.
  void accept(Visitor& v) { return v.visit(this); }
};

// The operands of e1 > e2 shall have type int, and the type of the expression is bool.
class GreaterThan_Expr : public Expr {
private:
  Expr *e1, *e2;
  
public:
  GreaterThan_Expr(Expr *e1, Expr *e2, ASTcontext *cxt) : e1(e1), e2(e2) {
      if (e1->type == (*cxt).Int_ && e2->type == (*cxt).Int_)
        this->type = (*cxt).Bool_;
      else
        throw Type_Exception("GreaterThan_Expr");
  }
  ~GreaterThan_Expr() = default;

  // Accessor functions to retrieve the private variables e1 and e2.
  Expr* getE1() { return e1; }
  Expr* getE2() { return e2; }
  
  // Overriding of accept virtual function to accept visitors.
  void accept(Visitor& v) { return v.visit(this); }
};

// The operands of e1 <= e2 shall have type int, and the type of the expression is bool.
class LessEqThan_Expr : public Expr {
private:
  Expr *e1, *e2;

public:
  LessEqThan_Expr(Expr *e1, Expr *e2, ASTcontext *cxt) : e1(e1), e2(e2) {
    if (e1->type == (*cxt).Int_ && e2->type == (*cxt).Int_)
      this->type = (*cxt).Bool_;
    else
      throw Type_Exception("LessEqThan_Expr");
  }
  ~LessEqThan_Expr() = default;

  // Accessor functions to retrieve the private variables e1 and e2.
  Expr* getE1() { return e1; }
  Expr* getE2() { return e2; }

  // Overriding of accept virtual function to accept visitors.
  void accept(Visitor& v) { return v.visit(this); }
};

// The operands of e1 >= e2 shall have type int, and the type of the expression is bool.
class GreaterEqThan_Expr : public Expr {
private:
  Expr *e1, *e2;

public:
  GreaterEqThan_Expr(Expr *e1, Expr *e2, ASTcontext *cxt) : e1(e1), e2(e2) {
    if (e1->type == (*cxt).Int_ && e2->type == (*cxt).Int_)
      this->type = (*cxt).Bool_;
    else
      throw Type_Exception("GreaterEqThan_Expr");
  }
  ~GreaterEqThan_Expr() = default;

  // Accessor functions to retrieve the private variables e1 and e2.
  Expr* getE1() { return e1; }
  Expr* getE2() { return e2; }

  // Overriding of accept virtual function to accept visitors.
  void accept(Visitor& v) { return v.visit(this); }
};

// The operand of -e shall have type int, and the type of the expression is int.
class Negation_Expr : public Expr {
private:
  Expr *e;

public:
  Negation_Expr(Expr *e, ASTcontext *cxt) : e(e) {
    if (e->type == (*cxt).Int_)
      this->type = (*cxt).Int_;
    else
      throw Type_Exception("Negation_Expr");
  }
  ~Negation_Expr() = default;

  // Accessor function to retrieve the private variable e.
  Expr* getE() { return e; }

  // Overriding of accept virtual function to accept visitors.
  void accept(Visitor& v) { return v.visit(this); }
};

// The operand of ~e shall have type int, and the type of the expression is int.
class OneComplement_Expr : public Expr {
private:
  Expr *e;

public:
  OneComplement_Expr(Expr *e, ASTcontext *cxt) : e(e) {
    if (e->type == (*cxt).Int_)
      this->type = (*cxt).Int_;
    else if (e->type == (*cxt).Bool_)
      this->type = (*cxt).Bool_;
    else
      throw Type_Exception("OneComplement_Expr");
  }
  ~OneComplement_Expr() = default;

  // Accessor function to retreieve the private variable e.
  Expr* getE() { return e; }

  // Overriding of accept virtual function to accept visitors.
  void accept(Visitor& v) { return v.visit(this); }
};
#endif

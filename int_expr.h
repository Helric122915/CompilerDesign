#ifndef INT_EXPR_H
#define INT_EXPR_H

#include "expr.h"

class Int_Expr : public Expr {
  private:
    int value;

  public:
    Int_Expr(int value, ASTcontext &cxt) : value(value) {
      this->type = cxt.Int_;
    }

    void accept(Visitor& v) { return v.visit(this); }
    int weight() override { return 1; }
    int height() override { return 0; }
    int eval() override { return value; }
    int getValue() { return value; }
};

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

    void accept(Visitor& v) { return v.visit(this); }
    int weight() override { return 1 + e1->weight() + e2->weight(); }
    int height() override { return 1 + std::max(e1->height(),e2->height()); }
    int eval() override { return e1->eval() + e2->eval(); }
    Expr* getE1() { return e1; }
    Expr* getE2() { return e2; }
};

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

     void accept(Visitor& v) { return v.visit(this); }
     int weight() override { return 1 + e1->weight() + e2->weight(); }
     int height() override { return 1 + std::max(e1->height(),e2->height()); }
     int eval() override { return e1->eval() - e2->eval(); }
     Expr* getE1() { return e1; }
     Expr* getE2() { return e2; }
};

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

    void accept(Visitor& v) { return v.visit(this); }
    int weight() override { return 1 + e1->weight() + e2->weight(); }
    int height() override { return 1 + std::max(e1->height(),e2->height()); }
    int eval() override { return e1->eval() * e2->eval(); }
    Expr* getE1() { return e1; }
    Expr* getE2() { return e2; }
};

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

    void accept(Visitor& v) { return v.visit(this); }
    int weight() override { return 1 + e1->weight() + e2->weight(); }
    int height() override { return 1 + std::max(e1->height(),e2->height()); }
    int eval() override { return e1->eval() / e2->eval(); }
    Expr* getE1() { return e1; }
    Expr* getE2() { return e2; }
};

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

    void accept(Visitor& v) { return v.visit(this); }
    int weight() override { return 1 + e1->weight() + e2->weight(); }
    int height() override { return 1 + std::max(e1->height(),e2->height()); }
    int eval() override { return e1->eval() % e2->eval(); }
    Expr* getE1() { return e1; }
    Expr* getE2() { return e2; }
};

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

    void accept(Visitor& v) { return v.visit(this); }
    int weight() override { return 1 + e1->weight() + e2->weight(); }
    int height() override { return 1 + std::max(e1->height(),e2->height()); }
    int eval() override { return e1->eval() < e2->eval(); }
    Expr* getE1() { return e1; }
    Expr* getE2() { return e2; }
};

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

    void accept(Visitor& v) { return v.visit(this); }
    int weight() override { return 1 + e1->weight() + e2->weight(); }
    int height() override { return 1 + std::max(e1->height(),e2->height()); }
    int eval() override { return e1->eval() > e2->eval(); }
    Expr* getE1() { return e1; }
    Expr* getE2() { return e2; }
};

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

    void accept(Visitor& v) { return v.visit(this); }
    int weight() override { return 1 + e1->weight() + e2->weight(); }
    int height() override { return 1 + std::max(e1->height(),e2->height()); }
    int eval() override { return e1->eval() <= e2->eval(); }
    Expr* getE1() { return e1; }
    Expr* getE2() { return e2; }
};

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

    void accept(Visitor& v) { return v.visit(this); }
    int weight() override { return 1 + e1->weight() + e2->weight(); }
    int height() override { return 1 + std::max(e1->height(),e2->height()); }
    int eval() override { return e1->eval() >= e2->eval(); }
    Expr* getE1() { return e1; }
    Expr* getE2() { return e2; }
};

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

    void accept(Visitor& v) { return v.visit(this); }
    int weight() override { return 1 + e->weight(); }
    int height() override { return 1 + e->height(); }
    int eval() override { return 0 - e->eval(); }
    Expr* getE() { return e; }
};
#endif

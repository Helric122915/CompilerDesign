#ifndef BOOL_EXPR_H
0;136;0c#define BOOL_EXPR_H

#include "expr.h"

class Bool_Expr : public Expr {
  private:
    bool value;

  public:
    Bool_Expr(bool value, ASTcontext &cxt) : value(value) {
      this->type = cxt.Bool_;
    }
    ~Bool_Expr() = default;

    void accept(Visitor& v) { return v.visit(this); }
    int weight() override { return 1; }
    int height() override { return 0; }
    int eval() override { return value; }
    int getValue() { return value; }
};

class Not_Expr : public Expr {
  private:
    Expr *e;

  public:
    Not_Expr(Expr *e, ASTcontext &cxt) : e(e) {
      if (e->type == cxt.Bool_)
        this->type = cxt.Bool_;
      else
        throw Type_Exception("Not_Expr");
    }
    ~Not_Expr() = default;

    void accept(Visitor& v) { return v.visit(this); }
    int weight() override { return 1 + e->weight(); }
    int height() override { return 1 + e->height(); }
    int eval() override { return e->eval(); }
    Expr* getE() { return e; }
};

class And_Expr : public Expr {
  private:
    Expr *e1, *e2;

  public:
    And_Expr(Expr *e1, Expr *e2, ASTcontext &cxt) : e1(e1), e2(e2) {
      if (e1->type == cxt.Bool_ && e2->type == cxt.Bool_)
        this->type = cxt.Bool_;
      else
        throw Type_Exception("And_Expr");
    }
    ~And_Expr() = default;

    void accept(Visitor& v) { return v.visit(this); }
    int weight() override { return 1 + e1->weight() + e2->weight(); }
    int height() override { return 1 + std::max(e1->height(),e2->height()); }
    int eval() override { return e1->eval() & e2->eval(); }
    Expr* getE1() { return e1; }
    Expr* getE2() { return e2; }
};

class Or_Expr : public Expr {
  private:
    Expr *e1, *e2;

  public:
    Or_Expr(Expr *e1, Expr *e2, ASTcontext &cxt) : e1(e1), e2(e2) {
      if (e1->type == cxt.Bool_ && e2->type == cxt.Bool_)
        this->type = cxt.Bool_;
      else
        throw Type_Exception("Or_Expr");
    }
    ~Or_Expr() = default;

    void accept(Visitor& v) { return v.visit(this); }
    int weight() override { return 1 + e1->weight() + e2->weight(); }
    int height() override { return 1 + std::max(e1->height(),e2->height()); }
    int eval() override { return e1->eval() | e2->eval(); }
    Expr* getE1() { return e1; }
    Expr* getE2() { return e2; }
};

class Xor_Expr : public Expr {
  private:
    Expr *e1, *e2;

  public:
    Xor_Expr(Expr *e1, Expr *e2, ASTcontext &cxt) : e1(e1), e2(e2) {
      if(e1->type == cxt.Bool_ && e2->type == cxt.Bool_)
        this->type = cxt.Bool_;
      else
        throw Type_Exception("Xor_Expr");
    }
    ~Xor_Expr() = default;

    void accept(Visitor& v) { return v.visit(this); }
    int weight() override { return 1 + e1->weight() + e2->weight(); }
    int height() override { return 1 + std::max(e1->height(),e2->height()); }
    int eval() override { return e1->eval() ^ e2->eval(); }
    Expr* getE1() { return e1; }
    Expr* getE2() { return e2; }
};

class Eq_Expr : public Expr {
  private:
    Expr *e1, *e2;

  public:
    Eq_Expr(Expr *e1, Expr *e2, ASTcontext &cxt) : e1(e1), e2(e2) {
      if (e1->type == e2->type)
        this->type = cxt.Bool_;
      else
        throw Type_Exception("Eq_Expr");
    }
    ~Eq_Expr() = default;

    void accept(Visitor& v) { return v.visit(this); }
    int weight() override { return 1 + e1->weight() + e2->weight(); }
    int height() override { return 1 + std::max(e1->height(),e2->height()); }
    int eval() override { return e1->eval() == e2->eval(); }
    Expr* getE1() { return e1; }
    Expr* getE2() { return e2; }
};

class NotEq_Expr : public Expr {
  private:
    Expr *e1, *e2;

  public:
    NotEq_Expr(Expr *e1, Expr *e2, ASTcontext &cxt) : e1(e1), e2(e2) {
      if (e1->type == e2->type)
        this->type = cxt.Bool_;
      else
        throw Type_Exception("NotEq_Expr");
    }
    ~NotEq_Expr() = default;

    void accept(Visitor& v) { return v.visit(this); }
    int weight() override { return 1 + e1->weight() + e2->weight(); }
    int height() override { return 1 + std::max(e1->height(),e2->height()); }
    int eval() override { return e1->eval() != e2->eval(); }
    Expr* getE1() { return e1; }
    Expr* getE2() { return e2; }
};

class Cond_Expr : public Expr {
  private:
    Expr *e1, *e2, *e3;

  public:
    Cond_Expr(Expr *e1, Expr *e2, Expr *e3, ASTcontext &cxt) : e1(e1), e2(e2), e3(e3) {
      if (e1->type == cxt.Bool_ && e2->type == e3->type)
        this->type = e2->type;
      else
        throw Type_Exception("Cond_Expr");
    }
    ~Cond_Expr() = default;

    void accept(Visitor& v) { return v.visit(this); }
    int weight() override { return 1 + e1->weight() + e2->weight() + e3->weight(); }
    int height() override { return 1 + std::max(e1->height(),std::max(e2->height(),e3->height())); }
    int eval() override { return e1->eval() ? e2->eval() : e3->eval(); }
    Expr* getE1() { return e1; }
    Expr* getE2() { return e2; }
    Expr* getE3() { return e3; }
};

class AndThen_Expr : public Expr {
  private:
    Expr *e1, *e2;

  public:
    AndThen_Expr(Expr *e1, Expr *e2, ASTcontext &cxt) : e1(e1), e2(e2) {
      if (e1->type == cxt.Bool_ && e2->type == cxt.Bool_)
        this->type = cxt.Bool_;
      else
        throw Type_Exception("AndThen_Expr");
    }
    ~AndThen_Expr() = default;

    void accept(Visitor& v) { return v.visit(this); }
    int weight() override { return 1 + e1->weight() + e2->weight(); }
    int height() override { return 1 + std::max(e1->height(),e2->height()); }
    int eval() override { return e1->eval() ? e2->eval() : false; }
    Expr* getE1() { return e1; }
    Expr* getE2() { return e2; }
};

class OrElse_Expr : public Expr {
  private:
    Expr *e1, *e2;

  public:
    OrElse_Expr(Expr *e1, Expr *e2, ASTcontext &cxt) : e1(e1), e2(e2) {
      if (e1->type == cxt.Bool_ && e2->type == cxt.Bool_)
        this->type = cxt.Bool_;
      else
        throw Type_Exception("OrElse_Expr");
    }
    ~OrElse_Expr() = default;

    void accept(Visitor& v) { return v.visit(this); }
    int weight() override { return 1 + e1->weight() + e2->weight(); }
    int height() override { return 1 + std::max(e1->height(),e2->height()); }
    int eval() override { return e1->eval() ? true : e2->eval(); }
    Expr* getE1() { return e1; }
    Expr* getE2() { return e2; }
};
#endif

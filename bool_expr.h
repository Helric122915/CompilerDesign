#ifndef BOOL_EXPR_H
#define BOOL_EXPR_H

#include "expr.h"

class Bool_Expr : public Expr {
  private:
    bool value;

  public:
    Bool_Expr(bool value) : value(value) {
      this->type = &Bool_;
    }

    int weight() override { return 1; }
    int height() override { return 0; }
    int eval() override { return value; }
};

class Not_Expr : public Expr {
  private:
    Expr *e;

  public:
    Not_Expr(Expr *e) : e(e) {
      if (e->type == &Bool_)
        this->type = &Bool_;
      else
        throw Type_Error;
    }

    int weight() override { return 1 + e->weight(); }
    int height() override { return 1 + e->height(); }
    int eval() override { return !e->eval(); }
};

class And_Expr : public Expr {
  private:
    Expr *e1, *e2;

  public:
    And_Expr(Expr *e1, Expr *e2) : e1(e1), e2(e2) {
      if (e1->type == &Bool_ && e2->type == &Bool_)
        this->type = &Bool_;
      else
        throw Type_Error;
    }

    int weight() override { return 1 + e1->weight() + e2->weight(); }
    int height() override { return 1 + std::max(e1->height(),e2->height()); }
    int eval() override { return e1->eval() & e2->eval(); }
};

class Or_Expr : public Expr {
  private:
    Expr *e1, *e2;

  public:
    Or_Expr(Expr *e1, Expr *e2) : e1(e1), e2(e2) {
      if (e1->type == &Bool_ && e2->type == &Bool_)
        this->type = &Bool_;
      else
        throw Type_Error;
    }

    int weight() override { return 1 + e1->weight() + e2->weight(); }
    int height() override { return 1 + std::max(e1->height(),e2->height()); }
    int eval() override { return e1->eval() | e2->eval(); }
};

class Eq_Expr : public Expr {
  private:
    Expr *e1, *e2;

  public:
    Eq_Expr(Expr *e1, Expr *e2) : e1(e1), e2(e2) {
      if (e1->type == e2->type)
        this->type == &Bool_;
      else
        throw Type_Error;
    }

    int weight() override { return 1 + e1->weight() + e2->weight(); }
    int height() override { return 1 + std::max(e1->height(),e2->height()); }
    int eval() override { return e1->eval() == e2->eval(); }
};

class NotEq_Expr : public Expr {
  private:
    Expr *e1, *e2;

  public:
    NotEq_Expr(Expr *e1, Expr *e2) : e1(e1), e2(e2) {
      if (e1->type == e2->type)
        this->type == &Bool_;
      else
        throw Type_Error;
    }

    int weight() override { return 1 + e1->weight() + e2->weight(); }
    int height() override { return 1 + std::max(e1->height(),e2->height()); }
    int eval() override { return e1->eval() != e2->eval(); }
};
#endif

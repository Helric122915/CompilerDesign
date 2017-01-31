#ifndef BOOL_EXPR_H
#define BOOL_EXPR_H

#include "expr.h"
#include "context.h"

class Bool_Expr : public Expr {
  private:
    bool value;

  public:
    Bool_Expr(bool value, ASTcontext &cxt) : value(value) {
      this->type = cxt.Bool_;
    }

    int weight() override { return 1; }
    int height() override { return 0; }
    int eval() override { return value; }
};

class Not_Expr : public Expr {
  private:
    Expr *e;

  public:
    Not_Expr(Expr *e, ASTcontext &cxt) : e(e) {
      if (e->type == cxt.Bool_)
        this->type = cxt.Bool_;
      else
        throw Type_Error;
    }

    int weight() override { return 1 + e->weight(); }
    int height() override { return 1 + e->height(); }
    int eval() override { return ~e->eval(); }
};

class And_Expr : public Expr {
  private:
    Expr *e1, *e2;

  public:
    And_Expr(Expr *e1, Expr *e2, ASTcontext &cxt) : e1(e1), e2(e2) {
      if (e1->type == cxt.Bool_ && e2->type == cxt.Bool_)
        this->type = cxt.Bool_;
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
    Or_Expr(Expr *e1, Expr *e2, ASTcontext &cxt) : e1(e1), e2(e2) {
      if (e1->type == cxt.Bool_ && e2->type == cxt.Bool_)
        this->type = cxt.Bool_;
      else
        throw Type_Error;
    }

    int weight() override { return 1 + e1->weight() + e2->weight(); }
    int height() override { return 1 + std::max(e1->height(),e2->height()); }
    int eval() override { return e1->eval() | e2->eval(); }
};

class Xor_Expr : public Expr {
  private:
    Expr *e1, *e2;

  public:
    Xor_Expr(Expr *e1, Expr *e2, ASTcontext &cxt) : e1(e1), e2(e2) {
      if(e1->type == cxt.Bool_ && e2->type == cxt.Bool_)
        this->type = cxt.Bool_;
      else
        throw Type_Error;
    }

    int weight() override { return 1 + e1->weight() + e2->weight(); }
    int height() override { return 1 + std::max(e1->height(),e2->height()); }
    int eval() override { return e1->eval() ^ e2->eval(); }
};

class Eq_Expr : public Expr {
  private:
    Expr *e1, *e2;

  public:
    Eq_Expr(Expr *e1, Expr *e2, ASTcontext &cxt) : e1(e1), e2(e2) {
      if (e1->type == e2->type)
        this->type == cxt.Bool_;
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
    NotEq_Expr(Expr *e1, Expr *e2, ASTcontext &cxt) : e1(e1), e2(e2) {
      if (e1->type == e2->type)
        this->type == cxt.Bool_;
      else
        throw Type_Error;
    }

    int weight() override { return 1 + e1->weight() + e2->weight(); }
    int height() override { return 1 + std::max(e1->height(),e2->height()); }
    int eval() override { return e1->eval() != e2->eval(); }
};

class Cond_Expr : public Expr {
  private:
    Expr *e1, *e2, *e3;

  public:
    Cond_Expr(Expr *e1, Expr *e2, Expr *e3, ASTcontext &cxt) : e1(e1), e2(e2), e3(e3) {
      if (e1->type == cxt.Bool_)
        if (e2->type == e3->type)
          this->type == e2->type;
        else
          throw Type_Error;
      else
        throw Type_Error;
    }

    int weight() override { return 1 + e1->weight() + e2->weight() + e3->weight(); }
    int height() override { return 1 + std::max(e1->height(),std::max(e2->height(),e3->height())); }
    int eval() override { return e1->eval() ? e2->eval() : e3->eval(); }
};
#endif

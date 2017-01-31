#ifndef INT_EXPR_H
#define INT_EXPR_H

#include "expr.h"

class Int_Expr : public Expr {
  private:
    int value;

  public:
    Int_Expr(int value) : value(value) {
      this->type = &Int_;
    }

    int weight() override { return 1; }
    int height() override { return 0; }
    int eval() override { return value; }
};

class Add_Expr : public Expr {
  private:
    Expr *e1, *e2;

  public:
    Add_Expr(Expr *e1, Expr *e2) : e1(e1), e2(e2) {
      if (e1->type == &Int_ && e2->type == &Int_)
        this->type = &Int_;
      else
        throw Type_Error;
    }

    int weight() override { return 1 + e1->weight() + e2->weight(); }
    int height() override { return 1 + std::max(e1->height(),e2->height()); }
    int eval() override { return e1->eval() + e2->eval(); }
};

class Sub_Expr : public Expr {
  private:
    Expr *e1, *e2;

  public:
    Sub_Expr(Expr *e1, Expr *e2) : e1(e1), e2(e2) {
      if (e1->type == &Int_ && e2->type == &Int_)
        this->type = &Int_;
      else
        throw Type_Error;
     }

     int weight() override { return 1 + e1->weight() + e2->weight(); }
     int height() override { return 1 + std::max(e1->height(),e2->height()); }
     int eval() override { return e1->eval() - e2->eval(); }
};

class Mult_Expr : public Expr {
  private:
    Expr *e1, *e2;

  public:
    Mult_Expr(Expr *e1, Expr *e2) : e1(e1), e2(e2) {
      if (e1->type == &Int_ && e2->type == &Int_)
        this->type = &Int_;
      else
        throw Type_Error;
    }

    int weight() override { return 1 + e1->weight() + e2->weight(); }
    int height() override { return 1 + std::max(e1->height(),e2->height()); }
    int eval() override { return e1->eval() * e2->eval(); }
};

class Div_Expr : public Expr {
  private:
    Expr *e1, *e2;

  public:
    Div_Expr(Expr *e1, Expr *e2) : e1(e1), e2(e2) {
      if (e1->type == &Int_ && e2->type == &Int_)
        this->type = &Int_;
      else
        throw Type_Error;
    }

    int weight() override { return 1 + e1->weight() + e2->weight(); }
    int height() override { return 1 + std::max(e1->height(),e2->height()); }
    int eval() override { return e1->eval() / e2->eval(); }
};

class Mod_Expr : public Expr {
  private:
    Expr *e1, *e2;

  public:
    Mod_Expr(Expr *e1, Expr *e2) : e1(e1), e2(e2) {
      if (e1->type == &Int_ && e2->type == &Int_)
        this->type = &Int_;
      else
        throw Type_Error;
    }

    int weight() override { return 1 + e1->weight() + e2->weight(); }
    int height() override { return 1 + std::max(e1->height(),e2->height()); }
    int eval() override { return e1->eval() % e2->eval(); }
};

class LessThan_Expr : public Expr {
  private:
    Expr *e1, *e2;

  public:
    LessThan_Expr(Expr *e1, Expr *e2) : e1(e1), e2(e2) {
      if (e1->type == &Int_ && e2->type == &Int_)
        this->type == &Bool_;
      else
        throw Type_Error;
    }

    int weight() override { return 1 + e1->weight() + e2->weight(); }
    int height() override { return 1 + std::max(e1->height(),e2->height()); }
    int eval() override { return e1->eval() < e2->eval(); }
};

class GreaterThan_Expr : public Expr {
  private:
    Expr *e1, *e2;

  public:
    GreaterThan_Expr(Expr *e1, Expr *e2) : e1(e1), e2(e2) {
      if (e1->type == &Int_ && e2->type == &Int_)
        this->type == &Bool_;
      else
        throw Type_Error;
    }

    int weight() override { return 1 + e1->weight() + e2->weight()); }
    int height() override { return 1 + std::max(e1->height(),e2->height()); }
    int eval() override { return e1->eval() > e2->eval(); }
};

class LessEqThan_Expr : public Expr {
  private:
    Expr *e1, *e2;

  public:
    LessEqThan_Expr(Expr *e1, Expr *e2) : e1(e1), e2(e2) {
      if (e1->type == &Int_ && e2->type == &Int_)
        this->type == &Bool_;
      else
        throw Type_Error;
    }

    int weight() override { return 1 + e1->weight() + e2->weight()); }
    int height() override { return 1 + std::max(e1->height(),e2->height()); }
    int eval() override { return e1->eval() <= e2->eval(); }
};

class GreaterEqThan_Expr : public Expr {
  private:
    Expr *e1, *e2;

  public:
    GreaterEqThan_Expr(Expr *e1, Expr *e2) : e1(e1), e2(e2) {
      if (e1->type == &Int_ && e2->type == &Int_)
        this->type == &Bool_;
      else
        throw Type_Error;
    }

    int weight() override { return 1 + e1->weight() + e2->weight()); }
    int height() override { return 1 + std::max(e1->height(),e2->height()); }
    int eval() override { return e1->eval() >= e2->eval(); }
};
#endif

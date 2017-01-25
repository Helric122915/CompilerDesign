#ifndef EXPR_H
#define EXPR_H

#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <ctime>

// Base Class
struct Expr{
  virtual ~Expr(){}
  virtual int weight() = 0;
  virtual int height() = 0;
  virtual bool eval () = 0;
};

struct Bool_Expr: Expr {
  bool value;

  Bool_Expr(bool value)
  {
    this->value = value;
  }

  int weight() override { return 1; }
  int height() override { return 0; }
  bool eval() override { return value; }
};

struct Not_Expr : Expr {
  Expr *e;

  Not_Expr(Expr *e)
  {
    this->e = e;
  }

  int weight() override { return 1 + e->weight(); }
  int height() override { return 1 + e->height(); }
  bool eval() override { return !e->eval(); }
};

struct And_Expr : Expr {
  Expr *e1;
  Expr *e2;

  And_Expr(Expr *e1, Expr *e2)
  {
    this->e1 = e1;
    this->e2 = e2;
  }

  int weight() override { return 1 + e1->weight() + e2->weight(); }
  int height() override { return 1 + std::max(e1->height(),e2->height()); }
  bool eval() override { return e1->eval() & e2->eval(); }
};

struct Or_Expr : Expr {
  Expr *e1;
  Expr *e2;

  Or_Expr(Expr *e1, Expr *e2)
  {
    this->e1 = e1;
    this->e2 = e2;
  }

  int weight() override { return 1 + e1->weight() + e2->weight(); }
  int height() override { return 1 + std::max(e1->height(),e2->height()); }
  bool eval() override { return e1->eval() | e2->eval(); }
};

struct Cond_Expr : Expr {
  Expr *e1;
  Expr *e2;
  Expr *e3;

  Cond_Expr(Expr *e1, Expr *e2, Expr *e3)
  {
    this->e1 = e1;
    this->e2 = e2;
    this->e3 = e3;
  }

  int weight() override { return 1 + e1->weight() + e2->weight() + e3->weight(); }
  int height() override { return 1 + std::max(e1->height(),std::max(e2->height(),e3->height())); }
  bool eval() override { return (e1->eval() & e2->eval()) | (!e1->eval() & e3->eval()); }
};

#endif

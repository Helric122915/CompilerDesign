#ifndef WEIGHT_H
#define WEIGHT_H

#include "bool_expr.h"
#include "int_expr.h"

int weight(Expr *e)
{
  class V : public Expr::Visitor {
    private:
      int r;

    public:
      int getR() { return r; }
      void visit(Bool_Expr* e) { r = 1; }
      void visit(And_Expr* e) { r = 1 + weight(e->getE1()) + weight(e->getE2()); }
      void visit(Or_Expr* e) { r = 1 + weight(e->getE1()) + weight(e->getE2()); }
      void visit(Xor_Expr* e) { r = 1 + weight(e->getE1()) + weight(e->getE2()); }
      void visit(Not_Expr* e) { r = 1 + weight(e->getE()); }
      void visit(Eq_Expr* e) { r = 1 + weight(e->getE1()) + weight(e->getE2()); }
      void visit(NotEq_Expr* e) { r = 1 + weight(e->getE1()) + weight(e->getE2()); }
      void visit(Cond_Expr* e) { r = 1 + weight(e->getE1()) + weight(e->getE2()) + weight(e->getE3()); }
      void visit(AndThen_Expr* e) { r = 1 + weight(e->getE1()) + weight(e->getE2()); }
      void visit(OrElse_Expr* e) { r = 1 + weight(e->getE1()) + weight(e->getE2()); }

      void visit(Int_Expr* e) { r = 1; }
      void visit(Add_Expr* e) { r = 1 + weight(e->getE1()) + weight(e->getE2()); }
      void visit(Sub_Expr* e) { r = 1 + weight(e->getE1()) + weight(e->getE2()); }
      void visit(Mult_Expr* e) { r = 1 + weight(e->getE1()) + weight(e->getE2()); }
      void visit(Div_Expr* e) { r = 1 + weight(e->getE1()) + weight(e->getE2()); }
      void visit(Mod_Expr* e) { r = 1 + weight(e->getE1()) + weight(e->getE2()); }
      void visit(LessThan_Expr* e) { r = 1 + weight(e->getE1()) + weight(e->getE2()); }
      void visit(GreaterThan_Expr* e) { r = 1 + weight(e->getE1()) + weight(e->getE2()); }
      void visit(LessEqThan_Expr* e) { r = 1 + weight(e->getE1()) + weight(e->getE2()); }
      void visit(GreaterEqThan_Expr* e) { r = 1 + weight(e->getE1()) + weight(e->getE2()); }
      void visit(Negation_Expr *e) { r = 1 + weight(e->getE()); }
  };
  V vis;
  e->accept(vis);
  return vis.getR();
}
#endif

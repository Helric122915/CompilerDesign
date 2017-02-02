#include "bool_expr.h"
#include "int_expr.h"
#include <algorithm>

int height(Expr *e)
{
  class V : public Expr::Visitor {
    public:
      int r;
      void visit(Bool_Expr* e) { r = 1; }
      void visit(And_Expr* e) { r = 1 + std::max(height(e->getE1()) + height(e->getE2())); }
      void visit(Or_Expr* e) { r = 1 + std::max(height(e->getE1()) + height(e->getE2())); }
      void visit(Xor_Expr* e) { r = 1 + std::max(height(e->getE1()) + height(e->getE2())); }
      void visit(Not_Expr* e) { r = 1 + height(e->getE()); }
      void visit(Eq_Expr* e) { r = 1 + std::max(height(e->getE1()) + height(e->getE2())); }
      void visit(NotEq_Expr* e) { r = 1 + std::max(height(e->getE1()) + height(e->getE2())); }
      void visit(Cond_Expr* e) { r = 1 + std::max(height(e->getE1()) + std::max(height(e->getE2() + height(e->getE3()))); }
      void visit(AndThen_Expr* e) { r = 1 + std::max(height(e->getE1()) + height(e->getE2())); }
      void visit(OrElse_Expr* e) { r = 1 + std::max(height(e->getE1()) + height(e->getE2())); }

      void visit(Int_Expr* e) { r = 1; }
      void visit(Add_Expr* e) { r = 1 + std::max(height(e->getE1()) + height(e->getE2())); }
      void visit(Sub_Expr* e) { r = 1 + std::max(height(e->getE1()) + height(e->getE2())); }
      void visit(Mult_Expr* e) { r = 1 + std::max(height(e->getE1()) + height(e->getE2())); }
      void visit(Div_Expr* e) { r = 1 + std::max(height(e->getE1()) + height(e->getE2())); }
      void visit(Mod_Expr* e) { r = 1 + std::max(height(e->getE1()) + height(e->getE2())); }
      void visit(LessThan_Expr* e) { r = 1 + std::max(height(e->getE1()) + height(e->getE2())); }
      void visit(GreaterThan_Expr* e) { r = 1 + std::max(height(e->getE1()) + height(e->getE2())); }
      void visit(LessEqThan_Expr* e) { r = 1 + std::max(height(e->getE1()) + height(e->getE2())); }
      void visit(GreaterEqThan_Expr* e) { r = 1 + std::max(height(e->getE1()) + height(e->getE2())); }
      void visit(Negation_Expr *e) { r = 1 + height(e->getE()); }
  };
  V vis;
  e->accept(vis);
  return vis.r;
}

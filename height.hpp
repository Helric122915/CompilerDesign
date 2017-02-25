#ifndef HEIGHT_HPP
#define HEIGHT_HPP

#include "bool_expr.hpp"
#include "int_expr.hpp"

// Implementation of the Visitor pattern allowing for the height of an expression to be checked.
// Returns the height of the expression e that is passed in.
int height(Expr *e)
{
  class V : public Expr::Visitor {
  private:
    int r;

  public:
    // Accessor function to retieve the private variable r.
    int getR() { return r; }

    // Overriding of each visit virtual function set to the desired functionality of each expression.
    void visit(Bool_Expr* e) { r = 1; }
    void visit(And_Expr* e) { r = 1 + std::max(height(e->getE1()),height(e->getE2())); }
    void visit(Or_Expr* e) { r = 1 + std::max(height(e->getE1()),height(e->getE2())); }
    void visit(Xor_Expr* e) { r = 1 + std::max(height(e->getE1()),height(e->getE2())); }
    void visit(Not_Expr* e) { r = 1 + height(e->getE()); }
    void visit(Eq_Expr* e) { r = 1 + std::max(height(e->getE1()),height(e->getE2())); }
    void visit(NotEq_Expr* e) { r = 1 + std::max(height(e->getE1()),height(e->getE2())); }
    void visit(Cond_Expr* e) { r = 1 + std::max(height(e->getE1()),std::max(height(e->getE2()),height(e->getE3()))); }
    void visit(AndThen_Expr* e) { r = 1 + std::max(height(e->getE1()),height(e->getE2())); }
    void visit(OrElse_Expr* e) { r = 1 + std::max(height(e->getE1()),height(e->getE2())); }

    void visit(Int_Expr* e) { r = 1; }
    void visit(Add_Expr* e) { r = 1 + std::max(height(e->getE1()),height(e->getE2())); }
    void visit(Sub_Expr* e) { r = 1 + std::max(height(e->getE1()),height(e->getE2())); }
    void visit(Mult_Expr* e) { r = 1 + std::max(height(e->getE1()),height(e->getE2())); }
    void visit(Div_Expr* e) { r = 1 + std::max(height(e->getE1()),height(e->getE2())); }
    void visit(Mod_Expr* e) { r = 1 + std::max(height(e->getE1()),height(e->getE2())); }
    void visit(LessThan_Expr* e) { r = 1 + std::max(height(e->getE1()),height(e->getE2())); }
    void visit(GreaterThan_Expr* e) { r = 1 + std::max(height(e->getE1()),height(e->getE2())); }
    void visit(LessEqThan_Expr* e) { r = 1 + std::max(height(e->getE1()),height(e->getE2())); }
    void visit(GreaterEqThan_Expr* e) { r = 1 + std::max(height(e->getE1()),height(e->getE2())); }
    void visit(Negation_Expr *e) { r = 1 + height(e->getE()); }
    void visit(OneComplement_Expr *e) { r = 1 + height(e->getE()); }
  };
  V vis;
  e->accept(vis);
  return vis.getR();
}
#endif

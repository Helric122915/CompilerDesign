#ifndef EVAL_HPP
#define EVAL_HPP

#include "bool_expr.hpp"
#include "int_expr.hpp"

// Implementation of the Visistor pattern allowing for the evaluation of an expression to be conducted.
// Returns the result of the expression e that is passed in.
int eval(Expr *e)
{
  class V : public Expr::Visitor {
    private:
      int r;

    public:
      // Accessor function to retrieve the private variable r.
      int getR() { return r; }

      // Overriding of each visit virtual function set to the desired functionality of each expression.
      void visit(Bool_Expr* e) { r = e->getValue(); }
      void visit(And_Expr* e) { r = eval(e->getE1()) & eval(e->getE2()); }
      void visit(Or_Expr* e) { r = eval(e->getE1()) | eval(e->getE2()); }
      void visit(Xor_Expr* e) { r = eval(e->getE1()) ^ eval(e->getE2()); }
      void visit(Not_Expr* e) { r = !eval(e->getE()); }
      void visit(Eq_Expr* e) { r = eval(e->getE1()) == eval(e->getE2()); }
      void visit(NotEq_Expr* e) { r = eval(e->getE1()) != eval(e->getE2()); }
      void visit(Cond_Expr* e) { r = (eval(e->getE1())?eval(e->getE2()):eval(e->getE3())); }
      void visit(AndThen_Expr* e) { r = (eval(e->getE1())?eval(e->getE2()):false); }
      void visit(OrElse_Expr* e) { r = (eval(e->getE1())?true:eval(e->getE2())); }

      void visit(Int_Expr* e) { r = e->getValue(); }
      void visit(Add_Expr* e) { r = eval(e->getE1()) + eval(e->getE2()); }
      void visit(Sub_Expr* e) { r = eval(e->getE1()) - eval(e->getE2()); }
      void visit(Mult_Expr* e) { r = eval(e->getE1()) * eval(e->getE2()); }
      void visit(Div_Expr* e) { r = eval(e->getE1()) / eval(e->getE2()); }
      void visit(Mod_Expr* e) { r = eval(e->getE1()) % eval(e->getE2()); }
      void visit(LessThan_Expr* e) { r = eval(e->getE1()) < eval(e->getE2()); }
      void visit(GreaterThan_Expr* e) { r = eval(e->getE1()) > eval(e->getE2()); }
      void visit(LessEqThan_Expr* e) { r = eval(e->getE1()) <= eval(e->getE2()); }
      void visit(GreaterEqThan_Expr* e) { r = eval(e->getE1()) >= eval(e->getE2()); }
      void visit(Negation_Expr *e) { r = 0 - eval(e->getE()); }
  };
  V vis;
  e->accept(vis);
  return vis.getR();
}
#endif

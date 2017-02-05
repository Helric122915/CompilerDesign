#ifndef CHECK_H
#define CHECK_H

#include <stdexcept>
#include "expr.h"
#include "bool_expr.h"
#include "int_expr.h"

const Type* check(Expr* e, ASTcontext& cxt) {
  class V : public Expr::Visitor {
    private:
      ASTcontext cxt;
      const Type* r;

    public:
      V(ASTcontext cxt) : cxt(cxt) {}

      void visit(Bool_Expr* e) { r = cxt.Bool_; }
      void visit(And_Expr* e) {
        if (e->getE1()->getType() == cxt.Bool_ && e->getE2()->getType() == cxt.Bool_)
          r = cxt.Bool_;
        else throw Type_Exception("And_Expr");
      }
      void visit(Or_Expr* e) {
        if (e->getE1()->getType() == cxt.Bool_ && e->getE2()->getType() == cxt.Bool_)
          r = cxt.Bool_;
        else throw Type_Exception("Or_Expr");
      }
      void visit(Xor_Expr* e) {
        if (e->getE1()->getType() == cxt.Bool_ && e->getE2()->getType() == cxt.Bool_)
          r = cxt.Bool_;
        else throw Type_Exception("Xor_Expr");
      }
      void visit(Not_Expr* e) {
        if (e->getE()->getType() == cxt.Bool_)
          r = cxt.Bool_;
        else throw Type_Exception("Not_Expr");;
      }
      void visit(Eq_Expr* e) {
        if (e->getE1()->getType() == e->getE2()->getType())
          r = cxt.Bool_;
        else throw Type_Exception("Eq_Expr");;
      }
      void visit(NotEq_Expr* e) {
        if (e->getE1()->getType() == e->getE2()->getType())
          r = cxt.Bool_;
        else throw Type_Exception("NotEq_Expr");;
      }
      void visit(Cond_Expr* e) {
        if (e->getE1()->getType() == cxt.Bool_ && (e->getE2()->getType() == e->getE3()->getType()))
          r = e->getE2()->getType();
        else throw Type_Exception("Cond_Expr");;
      }
      void visit(AndThen_Expr* e) {
        if (e->getE1()->getType() == cxt.Bool_ && e->getE2()->getType() == cxt.Bool_)
          r = cxt.Bool_;
        else throw Type_Exception("AndThen_Expr");;
      }
      void visit(OrElse_Expr* e) {
        if (e->getE1()->getType() == cxt.Bool_ && e->getE2()->getType() == cxt.Bool_)
          r = cxt.Bool_;
        else throw Type_Exception("OrElse_Expr");;
      }

      void visit(Int_Expr* e) { r = cxt.Int_; }
      void visit(Add_Expr* e) {
        if (e->getE1()->getType() == cxt.Int_ && e->getE2()->getType() == cxt.Int_)
          r = cxt.Int_;
        else throw Type_Exception("Add_Expr");;
      }
      void visit(Sub_Expr* e) {
        if (e->getE1()->getType() == cxt.Int_ && e->getE2()->getType() == cxt.Int_)
          r = cxt.Int_;
        else throw Type_Exception("Sub_Expr");;
      }
      void visit(Mult_Expr* e) {
        if (e->getE1()->getType() == cxt.Int_ && e->getE2()->getType() == cxt.Int_)
          r = cxt.Int_;
        else throw Type_Exception("Mult_Expr");;
      }
      void visit(Div_Expr* e) {
        if (e->getE1()->getType() == cxt.Int_ && e->getE2()->getType() == cxt.Int_)
          r = cxt.Int_;
        else throw Type_Exception("Div_Expr");;
      }
      void visit(Mod_Expr* e) {
        if (e->getE1()->getType() == cxt.Int_ && e->getE2()->getType() == cxt.Int_)
          r = cxt.Int_;
        else throw Type_Exception("Mod_Expr");;
      }
      void visit(LessThan_Expr* e) {
        if (e->getE1()->getType() == cxt.Int_ && e->getE2()->getType() == cxt.Int_)
          r = cxt.Bool_;
        else throw Type_Exception("LessThan_Expr");;
      }
      void visit(GreaterThan_Expr* e) {
        if (e->getE1()->getType() == cxt.Int_ && e->getE2()->getType() == cxt.Int_)
          r = cxt.Bool_;
        else throw Type_Exception("GreaterThan_Expr");;
      }
      void visit(LessEqThan_Expr* e) {
        if (e->getE1()->getType() == cxt.Int_ && e->getE2()->getType() == cxt.Int_)
          r = cxt.Bool_;
        else throw Type_Exception("LessEqThan_Expr");;
      }
      void visit(GreaterEqThan_Expr* e) {
        if (e->getE1()->getType() == cxt.Int_ && e->getE2()->getType() == cxt.Int_)
          r = cxt.Bool_;
        else throw Type_Exception("GreaterEqThan_Expr");;
      }
      void visit(Negation_Expr* e) {
        if (e->getE()->getType() == cxt.Int_)
          r = cxt.Int_;
        else throw Type_Exception("Negation_Expr");;
      }

      const Type* getR() { return r; }
  };
  V vis(cxt);
  e->accept(vis);
  return vis.getR();
}
#endif

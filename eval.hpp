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
    void visit(Value_Expr* e) { r = eval(e->getE()); }
    void visit(Bool_Expr* e) { r = e->getValue(); }
    void visit(And_Expr* e) { r = eval(e->getE1()) & eval(e->getE2()); }
    void visit(Or_Expr* e) { r = eval(e->getE1()) | eval(e->getE2()); }
    void visit(Xor_Expr* e) { r = eval(e->getE1()) ^ eval(e->getE2()); }
    void visit(Not_Expr* e) { r = !eval(e->getE()); }
    void visit(Eq_Expr* e) { r = eval(e->getE1()) == eval(e->getE2()); }
    void visit(NotEq_Expr* e) { r = eval(e->getE1()) != eval(e->getE2()); }
    void visit(Cond_Expr* e) { r = eval(e->getE1())?eval(e->getE2()):eval(e->getE3()); }
    void visit(AndThen_Expr* e) { r = eval(e->getE1())?eval(e->getE2()):false; }
    void visit(OrElse_Expr* e) { r = eval(e->getE1())?true:eval(e->getE2()); }

    void visit(Int_Expr* e) { r = e->getValue(); }
    void visit(Add_Expr* e) {
      int e1Val = eval(e->getE1());
      int e2Val = eval(e->getE2());

      if (e1Val > 0 && e2Val > 0)
	if (std::numeric_limits<int>::max() - e1Val < e2Val)
	  throw Overflow_Exception("Addition result too large.");
        else if (e1Val < 0 && e2Val < 0)
          if (std::numeric_limits<int>::min() - e1Val > e2Val)
            throw Overflow_Exception("Addition result too small.");

      r = e1Val + e2Val;
    }
    void visit(Sub_Expr* e) {
      int e1Val = eval(e->getE1());
      int e2Val = eval(e->getE2());

      if (e1Val > 0 && e2Val < 0)
	if (std::numeric_limits<int>::max() - e1Val < e2Val)
	  throw Overflow_Exception("Subtraction result too large.");
      if (e1Val < 0 && e2Val > 0)
	if (std::numeric_limits<int>::min() - e1Val > -e2Val)
	  throw Overflow_Exception("Subtraction result too small.");

      r = e1Val - e2Val;
    }
    void visit(Mult_Expr* e) {
      int e1Val = eval(e->getE1());
      int e2Val = eval(e->getE2());

      if (e1Val == 0 || e2Val == 0)
	r = 0;
      else if ((e1Val > 0 && e2Val > 0) || (e1Val < 0 && e2Val < 0))
	if (std::numeric_limits<int>::max()/abs(e1Val) < abs(e2Val))
	  throw Overflow_Exception("Multiplication result too large.");
        else if (e1Val > 0 && e2Val < 0)
          if (std::numeric_limits<int>::min()/e1Val > e2Val)
            throw Overflow_Exception("Multiplication result too small.");
	  else if (e1Val < 0 && e2Val > 0)
	    if (std::numeric_limits<int>::min()/e2Val > e1Val)
	      throw Overflow_Exception("Multiplication result too small.");

      r = e1Val * e2Val;
    }
    void visit(Div_Expr* e) {
      int e1Val = eval(e->getE1());
      int e2Val = eval(e->getE2());

      if (e2Val == 0)
	throw Overflow_Exception("Cannot divide by zero.");
      else if (e1Val == std::numeric_limits<int>::min() && e2Val == -1)
	throw Overflow_Exception("Largest negative value is 1 larger than largest positive.");

      r = e1Val / e2Val;
    }
    void visit(Mod_Expr* e) {
      int e1Val = eval(e->getE1());
      int e2Val = eval(e->getE2());

      if (e2Val == 0)
	throw Overflow_Exception("Cannot divide by zero.");

      r = e1Val % e2Val;
    }
    void visit(LessThan_Expr* e) { r = eval(e->getE1()) < eval(e->getE2()); }
    void visit(GreaterThan_Expr* e) { r = eval(e->getE1()) > eval(e->getE2()); }
    void visit(LessEqThan_Expr* e) { r = eval(e->getE1()) <= eval(e->getE2()); }
    void visit(GreaterEqThan_Expr* e) { r = eval(e->getE1()) >= eval(e->getE2()); }
    void visit(Negation_Expr *e) { r = 0 - eval(e->getE()); }
    void visit(OneComplement_Expr *e) { r = ~eval(e->getE()); }
  };
  V vis;
  e->accept(vis);
  return vis.getR();
}
#endif

#ifndef EVAL_HPP
#define EVAL_HPP

#include "bool_expr.hpp"
#include "int_expr.hpp"
#include "evaluator.hpp"

// Implementation of the Visistor pattern allowing for the evaluation of an expression to be conducted.
// Returns the result of the expression e that is passed in.
Value eval(Evaluator& evaluator, Expr *e)
{
  class V : public Expr::Visitor {
  private:
    Value r;
    Evaluator& ev;

  public:
    V(Evaluator& ev) : ev(ev) {}

    // Accessor function to retrieve the private variable r.
    Value getR() { return r; }

    // Overriding of each visit virtual function set to the desired functionality of each expression.
    void visit(Call_Expr* e) {
      if (ev.stack_depth() == 2048)
	throw Overflow_Exception("www.StackOverflow.com");

      Value val = eval(ev, e->getFunction());
      auto* fn = static_cast<Fn_Decl*>(val.get_fn());

      Stack_Frame function_frame(ev);

      const std::vector<Expr*>& args = e->getArguments();
      const std::vector<Decl*>& params = fn->params;

      std::vector<Value> localVals;
      localVals.reserve(args.size());

      {
	auto pi = params.begin();
	auto ai = args.begin();
	while (pi != params.end() && ai != args.end()) {
	  Value loc = ev.automatic_allocate();
	  localVals.push_back(loc);
	  
	  ev.copy_initialize(loc, *ai);
	  
	  ++pi;
	  ++ai;
	}
      }

      Value ret = ev.automatic_allocate();

      {
	auto pi = params.begin();
	auto li = localVals.begin();
	while (pi != params.end() && li != localVals.end()) {
	  ev.automatic_bind(dynamic_cast<Name_Decl*>(*pi), *li);
	  ++pi;
	  ++li;
	}
	ev.automatic_bind(dynamic_cast<Name_Decl*>(fn->ret), ret);
      }

      evaluate(ev, ret, fn->body);

      r = ret.get_value_ref();
    }

    void visit(Assign_Expr* e) {
      Value v1 = eval(ev, e->getE1());
      Value v2 = eval(ev, e->getE2());
 
      v1.get_value_ref() = v2;

      r = v1;
    }
    void visit(Value_Expr* e) {
      Value ref = eval(ev, e->getE());
      r = ref.get_value_ref();
    }
    void visit(Ref_Expr* e) {
      if (Fn_Decl* fn = dynamic_cast<Fn_Decl*>(e->getDecl())) {
	r = Value(fn);
      }
      else
	r = ev.automatic_locate(e->getDecl());
    }
    void visit(Init_Expr* e) { r = eval(ev, e->getE()); }
    void visit(Bind_Expr* e) { r = eval(ev, e->getE()); }
    void visit(Assert_Expr* e) {
      Value cond = eval(ev, e->getE());
      if (!cond.get_int())
	throw Interpretor_Exception("Assertion Failed");
      r = cond;
    }
    void visit(Bool_Expr* e) { r = Value(e->getValue()); }
    void visit(And_Expr* e) { r = Value(eval(ev, e->getE1()).get_int() & eval(ev, e->getE2()).get_int()); }
    void visit(Or_Expr* e) { r = Value(eval(ev, e->getE1()).get_int() | eval(ev, e->getE2()).get_int()); }
    void visit(Xor_Expr* e) { r = Value(eval(ev, e->getE1()).get_int() ^ eval(ev, e->getE2()).get_int()); }
    void visit(Not_Expr* e) { r = Value(!eval(ev, e->getE()).get_int()); }
    void visit(Eq_Expr* e) { r = Value(eval(ev, e->getE1()).get_int() == eval(ev, e->getE2()).get_int()); }
    void visit(NotEq_Expr* e) { r = Value(eval(ev, e->getE1()).get_int() != eval(ev, e->getE2()).get_int()); }
    void visit(Cond_Expr* e) { r = Value(eval(ev, e->getE1()).get_int()?eval(ev, e->getE2()).get_int():eval(ev, e->getE3()).get_int()); }
    void visit(AndThen_Expr* e) { r = Value(eval(ev, e->getE1()).get_int()?eval(ev, e->getE2()).get_int():false); }
    void visit(OrElse_Expr* e) { r = Value(eval(ev, e->getE1()).get_int()?true:eval(ev, e->getE2()).get_int()); }

    void visit(Int_Expr* e) { r = Value(e->getValue()); }
    void visit(Add_Expr* e) {
      int e1Val = eval(ev, e->getE1()).get_int();
      int e2Val = eval(ev, e->getE2()).get_int();

      if (e1Val > 0 && e2Val > 0)
	if (std::numeric_limits<int>::max() - e1Val < e2Val)
	  throw Overflow_Exception("Addition result too large.");
        else if (e1Val < 0 && e2Val < 0)
          if (std::numeric_limits<int>::min() - e1Val > e2Val)
            throw Overflow_Exception("Addition result too small.");

      r = Value(e1Val + e2Val);
    }
    void visit(Sub_Expr* e) {
      int e1Val = eval(ev, e->getE1()).get_int();
      int e2Val = eval(ev, e->getE2()).get_int();

      if (e1Val > 0 && e2Val < 0)
	if (std::numeric_limits<int>::max() - e1Val < e2Val)
	  throw Overflow_Exception("Subtraction result too large.");
      if (e1Val < 0 && e2Val > 0)
	if (std::numeric_limits<int>::min() - e1Val > -e2Val)
	  throw Overflow_Exception("Subtraction result too small.");

      r = Value(e1Val - e2Val);
    }
    void visit(Mult_Expr* e) {
      int e1Val = eval(ev, e->getE1()).get_int();
      int e2Val = eval(ev, e->getE2()).get_int();

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

      r = Value(e1Val * e2Val);
    }
    void visit(Div_Expr* e) {
      int e1Val = eval(ev, e->getE1()).get_int();
      int e2Val = eval(ev, e->getE2()).get_int();

      if (e2Val == 0)
	throw Overflow_Exception("Cannot divide by zero.");
      else if (e1Val == std::numeric_limits<int>::min() && e2Val == -1)
	throw Overflow_Exception("Largest negative value is 1 larger than largest positive.");

      r = Value(e1Val / e2Val);
    }
    void visit(Mod_Expr* e) {
      int e1Val = eval(ev, e->getE1()).get_int();
      int e2Val = eval(ev, e->getE2()).get_int();

      if (e2Val == 0)
	throw Overflow_Exception("Cannot divide by zero.");

      r = Value(e1Val % e2Val);
    }
    void visit(LessThan_Expr* e) { r = Value(eval(ev, e->getE1()).get_int() < eval(ev, e->getE2()).get_int()); }    void visit(GreaterThan_Expr* e) { r = Value(eval(ev, e->getE1()).get_int() > eval(ev, e->getE2()).get_int()); }
    void visit(LessEqThan_Expr* e) { r = Value(eval(ev, e->getE1()).get_int() <= eval(ev, e->getE2()).get_int()); }
    void visit(GreaterEqThan_Expr* e) { r = Value(eval(ev, e->getE1()).get_int() >= eval(ev, e->getE2()).get_int()); }
    void visit(Negation_Expr *e) { r = Value(0 - eval(ev, e->getE()).get_int()); }
    void visit(OneComplement_Expr *e) { r = Value(~eval(ev, e->getE()).get_int()); }
  };
  V vis(evaluator);
  e->accept(vis);
  return vis.getR();
}
#endif

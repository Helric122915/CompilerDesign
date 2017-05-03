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
      std::cout << "Evaluating Call Expr\n";

      if (ev.stack_depth() == 512)
	throw Overflow_Exception("www.StackOverflow.com");

      Value val = eval(ev, e->getFunction());
      auto* fn = static_cast<Fn_Decl*>(val.get_fn());

      if (!fn->body)
	std::cout << "Function has no body tooooo love?!\n";

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
      std::cout << "Evaluating Assign_Expr\n";
      Value v1 = eval(ev, e->getE1());

      std::cout << "E2: ->"; print(e->getE2()); std::cout << '\n';

      Value v2 = eval(ev, e->getE2());
 
      std::cout << "v1-> "; v1.print_value(); std::cout << '\n';
      std::cout << "v2-> "; v2.print_value(); std::cout << '\n';

      v1.get_value_ref() = v2;

      r = v1;

      std::cout << "Finished Evaluating Assign_Expr\n";
    }
    void visit(Value_Expr* e) {
      Value ref = eval(ev, e->getE());
      r = ref.get_value_ref();
    }
    void visit(Ref_Expr* e) { /* Implement Sutton's from ast/lang.store/ev.cpp */ 
      std::cout << "Evaluating Reefer Expr\n";
      //if (dynamic_cast<Ref_Type*>(e->getType())) {
      r = ev.automatic_locate(e->getDecl());

      std::cout << "Ref_Expr: "; r.print_value();

      // determine if it has automatic storage or not
      // if automatic check frame
      // else check context
      //}
      //else if (dynamic_cast<Fn_Type*>(e->getType())) {
      // do magic function stuff
      //}
      std::cout << "Finished Evaluating Reefer Expr\n";
    }
    void visit(Init_Expr* e) { r = eval(ev, e->getE()); }
    void visit(Bind_Expr* e) { r = eval(ev, e->getE()); }
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
      std::cout << "Evaluating Add Expr\n";
      int e1Val = eval(ev, e->getE1()).get_int();
      int e2Val = eval(ev, e->getE2()).get_int();

      if (e1Val > 0 && e2Val > 0)
	if (std::numeric_limits<int>::max() - e1Val < e2Val)
	  throw Overflow_Exception("Addition result too large.");
        else if (e1Val < 0 && e2Val < 0)
          if (std::numeric_limits<int>::min() - e1Val > e2Val)
            throw Overflow_Exception("Addition result too small.");

      r = Value(e1Val + e2Val);
      std::cout << "Add_Expr evaluated r = " << r.get_int() << '\n';
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
    void visit(LessThan_Expr* e) { std::cout << "E1 int val: " << eval(ev, e->getE1()).get_int() << '\n'; r = Value(eval(ev, e->getE1()).get_int() < eval(ev, e->getE2()).get_int()); }
    void visit(GreaterThan_Expr* e) { r = Value(eval(ev, e->getE1()).get_int() > eval(ev, e->getE2()).get_int()); }
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

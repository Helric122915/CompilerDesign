#ifndef EVALSTMT_HPP
#define EVALSTMT_HPP

#include "stmt.hpp"
#include "value.hpp"
#include "print.hpp"

class Evaluator;

flow_kind eval_stmt(Evaluator& evaluate, Value ret, Stmt* s) {
  class V: public Stmt::Visitor {
  private:
    flow_kind r;
    Evaluator& evaluate;
    Value ret;

  public:
    V(Evaluator& evaluate, Value ret) : evaluate(evaluate), ret(ret) {}
    flow_kind getR() { return r; }
  
    void visit(Expr_Stmt* stmt) {
      std::cout << "Evaluating Expr Stmt\n";
      eval(evaluate, stmt->getExpr());
      r = flow_next;
    }
    void visit(Decl_Stmt* stmt) {
      std::cout << "Evaluating Decl Stmt\n";
      eval_decl(evaluate, stmt->getDecl());
      r = flow_next;
    }
    void visit(Block_Stmt* stmt) {
      std::cout << "Evaluating Block Statement\n";
      for (Stmt* s1 : stmt->Get_Statements()) {
	flow_kind f = eval_stmt(evaluate, ret, s1);
	if (f == flow_return) {
	  r = flow_return;
	  return;
	}
	if (f == flow_break) {
	  r = flow_break;
	  return;
	}
	if (f == flow_continue) {
	  r = flow_continue;
	  return;
	}
      }
      r = flow_next;
    }
    void visit(If_Stmt* stmt) {
      std::cout << "Evaluating If Statement\n";
      Value v = eval(evaluate, stmt->Get_Condition());
      if (v.get_int()) {
	std::cout << "Evaluating True Block\n";
	r = eval_stmt(evaluate, ret, stmt->GetTrueBlock());
      }
      else {
	std::cout << "Evaluating False Block\n";
	if (stmt->GetFalseBlock())
	  r = eval_stmt(evaluate, ret, stmt->GetFalseBlock());
      }
    }
    void visit(While_Stmt* stmt) {
      std::cout << "Evaluating While Statement\n";
      while(true) {
	print(stmt->Get_Condition());
	std::cout << '\n';
	std::cout << "Checking While Condition\n";
	Value v = eval(evaluate, stmt->Get_Condition());
	if (!v.get_int())
	  std::cout << "Condition is False: Exiting While\n";

	if (v.get_int()) {
	  std::cout << "Condition is True: Evaluating While Body\n";
	  flow_kind f = eval_stmt(evaluate, ret, stmt->Body);
	  if (f == flow_continue)
	    continue;
	  if (f == flow_break)
	    break;
	  if (f == flow_return)
	    r = f;
	}
	else
	  break;
      }
      r = flow_next;
    }
    void visit(Break_Stmt* stmt) {
      std::cout << "Evaluating Break Stmt\n";
      r = flow_break;
    }
    void visit(Continue_Stmt* stmt) {
      std::cout << "Evaluating Continue Stmt\n";
      r = flow_continue;
    }
    void visit(Return_Stmt* stmt) {
      std::cout << "Evaluating Return Stmt\n";
      //eval.copy_intialize(ret, s->Get_Return());
      r = flow_return;
    }
  };
  V vis(evaluate, ret);
  s->accept(vis);
  return vis.getR();
}
#endif

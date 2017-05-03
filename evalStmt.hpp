#ifndef EVALSTMT_HPP
#define EVALSTMT_HPP

#include "stmt.hpp"
#include "value.hpp"
#include "print.hpp"
//#include "evaluator.hpp"

class Evaluator;

flow_kind eval_stmt(Evaluator& evaluate, Value ret, Stmt* s) {
  class V: public Stmt::Visitor {
  private:
    flow_kind r;
    Evaluator& ev;
    Value ret;

  public:
    V(Evaluator& ev, Value ret) : ev(ev), ret(ret) {}
    flow_kind getR() { return r; }
  
    void visit(Expr_Stmt* stmt) {
      eval(ev, stmt->getExpr());
      r = flow_next;
    }
    void visit(Decl_Stmt* stmt) {
      eval_decl(ev, stmt->getDecl());
      r = flow_next;
    }
    void visit(Block_Stmt* stmt) {
      for (Stmt* s1 : stmt->Get_Statements()) {
	flow_kind f = eval_stmt(ev, ret, s1);
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
      Value v = eval(ev, stmt->Get_Condition());
      if (v.get_int()) {
	r = eval_stmt(ev, ret, stmt->GetTrueBlock());
      }
      else {
	if (stmt->GetFalseBlock())
	  r = eval_stmt(ev, ret, stmt->GetFalseBlock());
      }
    }
    void visit(While_Stmt* stmt) {
      while(true) {
	Value v = eval(ev, stmt->Get_Condition());
	if (v.get_int()) {
	  flow_kind f = eval_stmt(ev, ret, stmt->Body);
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
      r = flow_break;
    }
    void visit(Continue_Stmt* stmt) {
      r = flow_continue;
    }
    void visit(Return_Stmt* stmt) {
      ev.copy_initialize(ret, stmt->Get_Return());
      r = flow_return;
    }
  };
  V vis(evaluate, ret);
  s->accept(vis);
  return vis.getR();
}
#endif

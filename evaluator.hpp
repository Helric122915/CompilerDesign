#ifndef EVALUATOR_HPP
#define EVALUATOR_HPP

#include "enumeration.hpp"
#include "frame.hpp"

class Evaluator;
class Stmt;

Value eval(Evaluator& evaluate, Expr* e);
void eval_decl(Evaluator& evaluate, Decl* d);
flow_kind eval_stmt(Evaluator& evaluate, Value ret, Stmt* s);

class ASTcontext;

class Evaluator {
private:
  ASTcontext* cxt;

public:
  Evaluator(ASTcontext* cxt) : cxt(cxt) { }

  void enter_frame() { stack.enter_frame(); }
  void leave_frame() { stack.leave_frame(); }

  Frame* current_frame() { return stack.current_frame(); }

  int stack_depth() { return stack.frameStack.size(); }

  Value automatic_allocate() { return stack.allocate(); }
  Value automatic_locate(Name_Decl* d) { return stack.locate(d); }

  void copy_initialize(Value, Expr*);
  Value automatic_bind(Name_Decl*, Value);

  Frame_Stack stack;
};

void Evaluator::copy_initialize(Value val, Expr* e) {
  Value& v = val.get_value_ref();
  v = eval(*this, e);
}

Value Evaluator::automatic_bind(Name_Decl* d, Value v) {
  stack.bind(d, v);
  return v;
}

class Stack_Frame {
private:
  Evaluator& ev;

public:
  Stack_Frame(Evaluator& ev) : ev(ev) { ev.enter_frame(); }
  ~Stack_Frame() { ev.leave_frame(); }
};

#include "evalStmt.hpp"

Value evaluate(Evaluator& evaluate, Expr* e) {
  return eval(evaluate, e);
}

void evaluate(Evaluator& evaluate, Decl* d) {
  eval_decl(evaluate, d);
} 

flow_kind evaluate(Evaluator& evaluate, Value ret, Stmt* s) {
  return eval_stmt(evaluate, ret, s);
}
#endif

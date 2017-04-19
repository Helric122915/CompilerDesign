#ifndef SEMANTICS_HPP
#define SEMANTICS_HPP

#include "context.hpp"
#include "expr.hpp"
#include "int_expr.hpp"
#include "bool_expr.hpp"
#include "stmt.hpp"

//class ASTcontext;

//class Expr;

/*
// Bool Expressions
class Bool_Expr;
class And_Expr;
class Or_Expr;
class Xor_Expr;
class Not_Expr;
class Eq_Expr;
class NotEq_Expr;
class Cond_Expr;
class AndThen_Expr;
class OrElse_Expr;

// Int Expressions
class Int_Expr;
class Add_Expr;
class Sub_Expr;
class Mult_Expr;
class Div_Expr;
class Mod_Expr;
class LessThan_Expr;
class GreaterThan_Expr;
class LessEqThan_Expr;
class GreaterEqThan_Expr;
class Negation_Expr;
class OneComplement_Expr;
*/

class Semantic {
private:
  ASTcontext* cxt;
  std::vector<Stmt*> control;

public:
  Semantic(ASTcontext* cxt) : cxt(cxt) {}

  Expr* standard_conversion(Expr* e, Type* t);
  Expr* boolean_conversion(Expr* e);
  Expr* integer_conversion(Expr* e);
  void check_common(Expr* e1, Expr* e2);

  Stmt* block_statement(std::vector<Stmt*>&& stmtSeq);
  Stmt* if_statement(Expr* e, Stmt* trueBlock, Stmt* falseBlock);
  Stmt* while_init(Expr* e);
  Stmt* while_complete(Stmt* loop, Stmt* body);
  Stmt* break_statement();
  Stmt* continue_statement();
  Stmt* return_statement(Expr *);
  Stmt* declaration_statement(Decl* d);
  Stmt* expression_statement(Expr* e);

  // complex -> in trans_decl.cpp
  //Decl* function_declaration(const std::string n, 

  Expr* assign_expression(Expr* e1, Expr* e2);
  Expr* condition_expression(Expr* e1, Expr* e2, Expr* e3);
  Expr* logical_and_expression(Expr* e1, Expr* e2);
  Expr* logical_or_expression(Expr* e1, Expr* e2);
  Expr* or_expression(Expr* e1, Expr* e2);
  Expr* xor_expression(Expr* e1, Expr* e2);
  Expr* and_expression(Expr* e1, Expr* e2);
  Expr* one_complement_expression(Expr* e);  
  Expr* not_expression(Expr* e);
  Expr* less_than_expression(Expr* e1, Expr* e2);
  Expr* greater_than_expression(Expr* e1, Expr* e2);
  Expr* less_eq_than_expression(Expr* e1, Expr* e2);
  Expr* greater_eq_than_expression(Expr* e1, Expr* e2);
  Expr* equal_expression(Expr* e1, Expr* e2);
  Expr* not_equal_expression(Expr* e1, Expr* e2);
  Expr* add_expression(Expr* e1, Expr* e2);
  Expr* sub_expression(Expr* e1, Expr* e2);
  Expr* mul_expression(Expr* e1, Expr* e2);
  Expr* div_expression(Expr* e1, Expr* e2);
  Expr* mod_expression(Expr* e1, Expr* e2);
  Expr* negation_expression(Expr* e);
  Expr* call_expression(Expr* e, std::vector<Expr*> args);
  Expr* id_expression(const std::string symbol);
  Expr* id_expression(Decl* d);
  Expr* true_literal();
  Expr* false_literal();
  Expr* int_literal(Token* tok); 

  void push_control(Stmt* s) { control.push_back(s); }
  void pop_control() { control.pop_back(); }
  Stmt* current_control() { return (control.empty() ? nullptr : control.back()); }
  bool inside_loop();
};

Expr* Semantic::standard_conversion(Expr* e, Type* t) {
  if (e->getType() == t)
    return e;

  if (Ref_Type* reference = dynamic_cast<Ref_Type*>(e->getType())) {
    e = new Value_Expr(e, reference->obj);
    if (e->getType() == t)
      return e;
  }
}

Expr* Semantic::boolean_conversion(Expr* e) {
  return standard_conversion(e, cxt->Bool_);
}

Expr* Semantic::integer_conversion(Expr* e) {
  return standard_conversion(e, cxt->Int_);  
}

void Semantic::check_common(Expr* e1, Expr* e2) {
  if (e1->getType() != e2->getType())
    throw Semantic_Exception("Expected types to match");
}

Stmt* Semantic::block_statement(std::vector<Stmt*>&& stmtSeq){
  return new Block_Stmt(std::move(stmtSeq));
}
  
Stmt* Semantic::if_statement(Expr* e, Stmt* trueBlock, Stmt* falseBlock) {
  e = boolean_conversion(e);
  return new If_Stmt(e, trueBlock, falseBlock);
}
 
Stmt* Semantic::while_init(Expr* e) {
  e = boolean_conversion(e);
  While_Stmt* loop = new While_Stmt(e, nullptr);
  push_control(loop);
  return loop;
}

Stmt* Semantic::while_complete(Stmt* l, Stmt* body) {
  While_Stmt* loop = static_cast<While_Stmt*>(l);
  loop->Body = body;
  pop_control();
  return loop;
}

Stmt* Semantic::break_statement() {
  if (!inside_loop())
    throw Semantic_Exception("Cannot Break while not in a Loop");
  return new Break_Stmt();
}
 
Stmt* Semantic::continue_statement() {
  if (!inside_loop())
    throw Semantic_Exception("Cannot Break while not in a Loop");
  return new Continue_Stmt();
}
 
// Might not support
//Stmt* Semantic::return_statement() {}
 
Stmt* Semantic::return_statement(Expr* e) {
  // requires function implementation
  throw Semantic_Exception("Not implemented.");
}

Stmt* Semantic::declaration_statement(Decl* d) {
  return new Decl_Stmt(d);
}
 
Stmt* Semantic::expression_statement(Expr* e) {
  return new Expr_Stmt(e);
}

  // complex -> in trans_decl.cpp
  //Decl* function_declaration(const std::string n, 

Expr* Semantic::assign_expression(Expr* e1, Expr* e2) {

}
Expr* Semantic::condition_expression(Expr* e1, Expr* e2, Expr* e3) {
  Expr* condition = boolean_conversion(e1);
  check_common(e2, e3);
  return new Cond_Expr(condition, e2, e3, cxt);
}

Expr* Semantic::logical_and_expression(Expr* e1, Expr* e2) {
  Expr* conv1 = boolean_conversion(e1);
  Expr* conv2 = boolean_conversion(e2);
  return new AndThen_Expr(e1, e2, cxt);
}

Expr* Semantic::logical_or_expression(Expr* e1, Expr* e2) {
  Expr* conv1 = boolean_conversion(e1);
  Expr* conv2 = boolean_conversion(e2);
  return new OrElse_Expr(e1, e2, cxt);
}

Expr* Semantic::or_expression(Expr* e1, Expr* e2) {
  Expr* conv1 = boolean_conversion(e1);
  Expr* conv2 = boolean_conversion(e2);
  return new Or_Expr(e1, e2, cxt);
}

Expr* Semantic::xor_expression(Expr* e1, Expr* e2) {
  Expr* conv1 = boolean_conversion(e1);
  Expr* conv2 = boolean_conversion(e2);
  return new Xor_Expr(e1, e2, cxt);
}

Expr* Semantic::and_expression(Expr* e1, Expr* e2) {
  Expr* conv1 = boolean_conversion(e1);
  Expr* conv2 = boolean_conversion(e2);
  return new And_Expr(e1, e2, cxt);
}

Expr* Semantic::one_complement_expression(Expr* e) {
  Expr* conv = boolean_conversion(e);
  return new OneComplement_Expr(e, cxt);
}

Expr* Semantic::not_expression(Expr* e) {
  Expr* conv = boolean_conversion(e);
  return new Not_Expr(e, cxt);
}

Expr* Semantic::less_than_expression(Expr* e1, Expr* e2) {
  Expr* conv1 = integer_conversion(e1);
  Expr* conv2 = integer_conversion(e2);
  return new LessThan_Expr(e1, e2, cxt);
}

Expr* Semantic::greater_than_expression(Expr* e1, Expr* e2) {
  Expr* conv1 = integer_conversion(e1);
  Expr* conv2 = integer_conversion(e2);
  return new GreaterThan_Expr(e1, e2, cxt);
}

Expr* Semantic::less_eq_than_expression(Expr* e1, Expr* e2) {
  Expr* conv1 = integer_conversion(e1);
  Expr* conv2 = integer_conversion(e2);
  return new LessEqThan_Expr(e1, e2, cxt);
}

Expr* Semantic::greater_eq_than_expression(Expr* e1, Expr* e2) {
  Expr* conv1 = integer_conversion(e1);
  Expr* conv2 = integer_conversion(e2);
  return new GreaterEqThan_Expr(e1, e2, cxt);
}

Expr* Semantic::equal_expression(Expr* e1, Expr* e2) {
  check_common(e1, e2);
  return new Eq_Expr(e1, e2, cxt);
}

Expr* Semantic::not_equal_expression(Expr* e1, Expr* e2) {
  check_common(e1, e2);
  return new NotEq_Expr(e1, e2, cxt);
}

Expr* Semantic::add_expression(Expr* e1, Expr* e2) {
  Expr* conv1 = integer_conversion(e1);
  Expr* conv2 = integer_conversion(e2);
  return new Add_Expr(e1, e2, cxt);
}

Expr* Semantic::sub_expression(Expr* e1, Expr* e2) {
  Expr* conv1 = integer_conversion(e1);
  Expr* conv2 = integer_conversion(e2);
  return new Sub_Expr(e1, e2, cxt);
}

Expr* Semantic::mul_expression(Expr* e1, Expr* e2) {
  Expr* conv1 = integer_conversion(e1);
  Expr* conv2 = integer_conversion(e2);
  return new Mult_Expr(e1, e2, cxt);
}

Expr* Semantic::div_expression(Expr* e1, Expr* e2) {
  Expr* conv1 = integer_conversion(e1);
  Expr* conv2 = integer_conversion(e2);
  return new Div_Expr(e1, e2, cxt);
}

Expr* Semantic::mod_expression(Expr* e1, Expr* e2) {
  Expr* conv1 = integer_conversion(e1);
  Expr* conv2 = integer_conversion(e2);
  return new Mod_Expr(e1, e2, cxt);
}

Expr* Semantic::negation_expression(Expr* e) {
  Expr* conv = integer_conversion(e);
  return new Negation_Expr(e, cxt);
}

Expr* Semantic::call_expression(Expr* e, std::vector<Expr*> args) {

}

Expr* Semantic::id_expression(const std::string symbol) {

}

Expr* Semantic::id_expression(Decl* d) {

}

/*
Expr* Semantic::true_literal() {
  return new Bool_Expr(true, cxt);
}

Expr* Semantic::false_literal() {
  return new Bool_Expr(false, cxt);
}

Expr* Semantic::int_literal(Token* tok) {
  Int_Token* num = static_cast<Int_Token*>(tok);
  return new Int_Expr(num->getValue(), cxt->Int_);
}
*/

bool Semantic::inside_loop() {
  if (Stmt* s = current_control()) {
    if (dynamic_cast<While_Stmt*>(s))
      return true;
  }
  return false;
}
#endif

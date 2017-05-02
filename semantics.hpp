#ifndef SEMANTICS_HPP
#define SEMANTICS_HPP

#include "context.hpp"
#include "expr.hpp"
#include "int_expr.hpp"
#include "bool_expr.hpp"
#include "stmt.hpp"
#include "scope.hpp"

class Semantic {
private:
  ASTcontext* cxt;
  std::vector<Stmt*> control;
  std::string main = "Main";
  std::string ret = "Ret";

public:
  Semantic(ASTcontext* cxt) : cxt(cxt) { currCont = new Fn_Decl(main, std::move(std::vector<Decl*>()), new Ret_Decl(ret)); }

  Expr* standard_conversion(Expr* e, Type* t);
  Expr* boolean_conversion(Expr* e);
  Expr* integer_conversion(Expr* e);
  Expr* arithmetic_conversion(Expr* e);
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
  const std::string* identifier(Token* tok);
  Expr* id_expression(const std::string* symbol);
  Expr* id_expression(Decl* d);
  Expr* true_literal();
  Expr* false_literal();
  Expr* int_literal(Token* tok); 

  Var_Decl* variable_declaration(const std::string*, Type*);
  Var_Decl* variable_complete(Var_Decl*);
  Var_Decl* variable_complete(Expr*, Var_Decl*);

  Expr* copy_initialize(Var_Decl*, Expr*);
  Expr* copy_initialize(Type*, Expr*);

  Expr* reference_initialize(Var_Decl*, Expr*);
  Expr* reference_initialize(Type*, Expr*);

  void declare(Scope*,Name_Decl*);
  bool add_parent(Name_Decl*);

  void push_control(Stmt* s) { control.push_back(s); }
  void pop_control() { control.pop_back(); }
  Stmt* current_control() { return (control.empty() ? nullptr : control.back()); }
  bool inside_loop();

  void enter_scope(int k) { stack.push(k); }
  void leave_scope();
  Scope* current_scope() { return stack.top(); }

  void set_current_context(Decl* d) { currCont = d; }
  Decl* current_context() { return currCont; }

  Decl* unqual_lookup(const std::string*);

  Decl* currCont;
  ScopeStack stack;
};

Decl* Semantic::unqual_lookup(const std::string* symbol) {
  if (auto* dec = stack.searchName(*symbol))
    return dec;
  throw Semantic_Exception("Variable could not be found");
}

void Semantic::leave_scope() {
  Scope* s = current_scope();
  stack.pop();
};

Expr* Semantic::standard_conversion(Expr* e, Type* t) {
  if (e->getType() == t)
    return e;

  if (Ref_Type* reference = dynamic_cast<Ref_Type*>(e->getType())) {
    e = new Value_Expr(e, reference->obj);
    if (e->getType() == t) {
      return e;
    }
  }
  else
    throw Semantic_Exception("Could not Standardly Conversationationiate");
}

Expr* Semantic::boolean_conversion(Expr* e) {
  return standard_conversion(e, cxt->Bool_);
}

Expr* Semantic::integer_conversion(Expr* e) {
  return standard_conversion(e, cxt->Int_);  
}

Expr* Semantic::arithmetic_conversion(Expr* e) {
  Type* t = e->getType();

  if (Ref_Type* ref = dynamic_cast<Ref_Type*>(t))
    return new Value_Expr(e, ref->obj);
  return e;
}

void Semantic::check_common(Expr* e1, Expr* e2) {
  if (!e1 || !e2)
    throw Semantic_Exception("An Expression is Null in Check Common");
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
  std::cout << "Creating Assign Expr\n";

  print(e1); std::cout << " "; print(e2); std::cout << '\n';

  Type* t = e1->getType();

  std::cout << "e1->type: " << t << '\n';

  t = get_type(t);
  std::cout << "survive get_type??\n";

  std::cout << e1->getType() << "<- e1 " << get_type(t) << "<- nested type " << e2->getType() << "<- e2\n";

  // TODO throw exception if not reference

  e2 = standard_conversion(e2, get_type(t));
  std::cout << "Completing assign\n";
  return new Assign_Expr(e1, e2, t);
}
Expr* Semantic::condition_expression(Expr* e1, Expr* e2, Expr* e3) {
  Expr* condition = boolean_conversion(e1);

  if (condition->getType() == cxt->Bool_) {
    check_common(e2, e3);
    return new Cond_Expr(condition, e2, e3, cxt);
  }
  else
    throw Type_Exception("Cond_Expr expects bool e1, and same type e2/e3");
}

Expr* Semantic::logical_and_expression(Expr* e1, Expr* e2) {
  Expr* conv1 = boolean_conversion(e1);
  Expr* conv2 = boolean_conversion(e2);

  if (conv1->getType() == cxt->Bool_ && conv2->getType() == cxt->Bool_)
    return new AndThen_Expr(conv1, conv2, cxt);
  else
    throw Type_Exception("AndThen_Expr expects both bool Expr");
}

Expr* Semantic::logical_or_expression(Expr* e1, Expr* e2) {
  Expr* conv1 = boolean_conversion(e1);
  Expr* conv2 = boolean_conversion(e2);

  if (conv1->getType() == cxt->Bool_ && conv2->getType() == cxt->Bool_)
    return new OrElse_Expr(conv1, conv2, cxt);
  else
    throw Type_Exception("OrElse_Expr expects both bool Expr");
}

Expr* Semantic::or_expression(Expr* e1, Expr* e2) {
  Expr* conv1 = boolean_conversion(e1);
  Expr* conv2 = boolean_conversion(e2);

  if (conv1->getType() == cxt->Bool_ && conv2->getType() == cxt->Bool_)
    return new Or_Expr(conv1, conv2, cxt);
  else
    throw Type_Exception("Or_Expr expects both bool Expr");
}

Expr* Semantic::xor_expression(Expr* e1, Expr* e2) {
  Expr* conv1 = boolean_conversion(e1);
  Expr* conv2 = boolean_conversion(e2);

  if (conv1->getType() == cxt->Bool_ && conv2->getType() == cxt->Bool_)
    return new Xor_Expr(conv1, conv2, cxt);
  else
    throw Type_Exception("Xor_Expr expects both bool Expr");
}

Expr* Semantic::and_expression(Expr* e1, Expr* e2) {
  Expr* conv1 = boolean_conversion(e1);
  Expr* conv2 = boolean_conversion(e2);

  if (conv1->getType() == cxt->Bool_ && conv2->getType() == cxt->Bool_)
    return new And_Expr(conv1, conv2, cxt);
  else
    throw Type_Exception("And_Expr expects both bool Expr");
}

Expr* Semantic::one_complement_expression(Expr* e) {
  Expr* conv = boolean_conversion(e);

  if (conv->getType() == cxt->Bool_ || conv->getType() == cxt->Int_)
    return new OneComplement_Expr(conv, cxt);
  else
    throw Type_Exception("OneComplement_Expr expects either bool or int Expr");
}

Expr* Semantic::not_expression(Expr* e) {
  Expr* conv = boolean_conversion(e);

  if (conv->getType() == cxt->Bool_)
    return new Not_Expr(conv, cxt);
  else
    throw Type_Exception("Not_Expr expects a bool Expr");
}

Expr* Semantic::less_than_expression(Expr* e1, Expr* e2) {
  Expr* conv1 = integer_conversion(e1);
  Expr* conv2 = integer_conversion(e2);

  if (conv1->getType() == cxt->Int_ && conv2->getType() == cxt->Int_)
    return new LessThan_Expr(conv1, conv2, cxt);
  else
    throw Type_Exception("LessThan_Expr expects both int Expr");
}

Expr* Semantic::greater_than_expression(Expr* e1, Expr* e2) {
  Expr* conv1 = integer_conversion(e1);
  Expr* conv2 = integer_conversion(e2);

  if (conv1->getType() == cxt->Int_ && conv2->getType() == cxt->Int_)
    return new GreaterThan_Expr(conv1, conv2, cxt);
  else
    throw Type_Exception("GreaterThan_Expr expects both int Expr");
}

Expr* Semantic::less_eq_than_expression(Expr* e1, Expr* e2) {
  Expr* conv1 = integer_conversion(e1);
  Expr* conv2 = integer_conversion(e2);

  if (conv1->getType() == cxt->Int_ && conv2->getType() == cxt->Int_)
    return new LessEqThan_Expr(conv1, conv2, cxt);
  else
    throw Type_Exception("LessEqThan_Expr expects both int Expr");
}

Expr* Semantic::greater_eq_than_expression(Expr* e1, Expr* e2) {
  Expr* conv1 = integer_conversion(e1);
  Expr* conv2 = integer_conversion(e2);

  if (conv1->getType() == cxt->Int_ && conv2->getType() == cxt->Int_)
    return new GreaterEqThan_Expr(conv1, conv2, cxt);
  else
    throw Type_Exception("GreaterEqThan_Expr expects both int Expr");
}

Expr* Semantic::equal_expression(Expr* e1, Expr* e2) {
  std::cout << "Semanticating an equal expr\n";
  Expr* conv1 = arithmetic_conversion(e1);
  Expr* conv2 = arithmetic_conversion(e2);
  std::cout << "conversinated successfully\n";
  check_common(conv1, conv2);
  std::cout << "check  broke yo?\n";
  return new Eq_Expr(conv1, conv2, cxt);
}

Expr* Semantic::not_equal_expression(Expr* e1, Expr* e2) {
  Expr* conv1 = arithmetic_conversion(e1);
  Expr* conv2 = arithmetic_conversion(e2);
  check_common(conv1, conv2);
  return new NotEq_Expr(conv1, conv2, cxt);
}

Expr* Semantic::add_expression(Expr* e1, Expr* e2) {
  Expr* conv1 = integer_conversion(e1);
  Expr* conv2 = integer_conversion(e2);

  if (conv1->getType() == cxt->Int_ && conv2->getType() == cxt->Int_)
    return new Add_Expr(conv1, conv2, cxt);
  else
    throw Type_Exception("Add_Expr expects both int Expr");
}

Expr* Semantic::sub_expression(Expr* e1, Expr* e2) {
  Expr* conv1 = integer_conversion(e1);
  Expr* conv2 = integer_conversion(e2);

  if (conv1->getType() == cxt->Int_ && conv2->getType() == cxt->Int_)
    return new Sub_Expr(conv1, conv2, cxt);
  else
    throw Type_Exception("Sub_Expr expects both int Expr");
}

Expr* Semantic::mul_expression(Expr* e1, Expr* e2) {
  Expr* conv1 = integer_conversion(e1);
  Expr* conv2 = integer_conversion(e2);

  if (conv1->getType() == cxt->Int_ && conv2->getType() == cxt->Int_)
    return new Mult_Expr(conv1, conv2, cxt);
  else
    throw Type_Exception("Mult_Expr expects both int Expr");
}

Expr* Semantic::div_expression(Expr* e1, Expr* e2) {
  Expr* conv1 = integer_conversion(e1);
  Expr* conv2 = integer_conversion(e2);

  if (conv1->getType() == cxt->Int_ && conv2->getType() == cxt->Int_)
    return new Div_Expr(conv1, conv2, cxt);
  else
    throw Type_Exception("Div_Expr expects both int Expr");
}

Expr* Semantic::mod_expression(Expr* e1, Expr* e2) {
  Expr* conv1 = integer_conversion(e1);
  Expr* conv2 = integer_conversion(e2);

  if (conv1->getType() == cxt->Int_ && conv2->getType() == cxt->Int_)
    return new Mod_Expr(conv1, conv2, cxt);
  else
    throw Type_Exception("Mod_Expr expects both int Expr");
}

Expr* Semantic::negation_expression(Expr* e) {
  Expr* conv = integer_conversion(e);

  if (conv->getType() == cxt->Int_)
    return new Negation_Expr(conv, cxt);
  else
    throw Type_Exception("Negation_Expr expects an int Expr");
}

Expr* Semantic::call_expression(Expr* e, std::vector<Expr*> args) {
  throw Semantic_Exception("Call Expression is not implemented");
}

const std::string* Semantic::identifier(Token* tok) {
  Ident_Token* ident = dynamic_cast<Ident_Token*>(tok);
  return ident->value;
}

Expr* Semantic::id_expression(const std::string* symbol) {
  Decl* search = unqual_lookup(symbol);

  if (!search)
    throw Semantic_Exception("Variable has not been defined");

  return id_expression(search);
}

Expr* Semantic::id_expression(Decl* d) {
  Type* t = d->getType();

  // check if the type is a variable or function
  Type* ref = cxt->Get_Ref_Type(t);

  std::cout << ref << "<- ref type\n";

  //throw Semantic_Exception("Need to implement Get Ref Type");

  return new Ref_Expr(dynamic_cast<Name_Decl*>(d), ref);
}

Var_Decl* Semantic::variable_declaration(const std::string* id, Type* t) {
  // auto* owner = current_function();

  Var_Decl* dec = new Var_Decl(*id);
  dec->type = t;

  declare(current_scope(), dec);

  return dec;
}

/*
Var_Decl* Semantic::variable_complete(Var_Decl* d) {
  std::cout << "Complete uninit Var\n";
  //Var_Decl* dec = static_cast<Var_Decl*>(d);
  
  if (d->t == cxt->Bool_)
    d->init = new Bool_Expr(0, cxt);
  else if (d->t == cxt->Int_)
    d->init = new Int_Expr(0, cxt);
  else
    throw Semantic_Exception("Unexpected Variable type");

  return d;
}
*/ 

Var_Decl* Semantic::variable_complete(Expr* e, Var_Decl* d) {
  std::cout << "Complete init Var\n";

  d->init = copy_initialize(d, e);

  return d;
}

Expr* Semantic::copy_initialize(Var_Decl* var, Expr* e) {
  std::cout << "Copy Initializing with Var_Decl Type: " << var->getType() << "\n";

  Expr* init = copy_initialize(var->getType(), e);
  if (auto* i = dynamic_cast<Init_Expr*>(init))
    i->init = var;
  else if (auto* i = dynamic_cast<Bind_Expr*>(init))
    i->init = var;
  else
    throw Semantic_Exception("Unknown Initializer");
  return init;
}

Expr* Semantic::copy_initialize(Type* t, Expr* e) {
  if (dynamic_cast<Ref_Type*>(e->getType()))
    return reference_initialize(t, e);

  Expr* convert = standard_conversion(e, t);

  return new Init_Expr(convert, nullptr, convert->getType());
}

Expr* Semantic::reference_initialize(Var_Decl* d, Expr* e) {
  Expr* init = reference_initialize(d->getType(), e);
  if (auto* i = dynamic_cast<Bind_Expr*>(init))
    i->init = d;
  else
    throw Semantic_Exception("Unknown Initializer");
  return init;
}

Expr* Semantic::reference_initialize(Type* t, Expr* e) {
  if (e->getType() != t)
    throw Semantic_Exception("Binding to invalid type");

  return new Bind_Expr(e, nullptr, t);
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

void Semantic::declare(Scope* s, Name_Decl* d) {
  std::cout << "Declare variable\n";
  //if (auto* e = stack.searchName(d->getName()))
    //throw Semantic_Exception("Variable already declared");

  //bind(s, d);

  //if (add_parent(d)
  if(!s->addToScope(d))
    throw Semantic_Exception("Variable already declared in this scope");
}

//void Semantic::bind(Scope*s, Name_Decl* d) {
//  if (auto* table = stack.searchName(d->getName())
//}

bool Semantic::add_parent(Name_Decl* d) {
  if (dynamic_cast<Param_Decl*>(d))
    return false;
  if (dynamic_cast<Ret_Decl*>(d))
    return false;
  return true;
}
#endif

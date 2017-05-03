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
  Semantic(ASTcontext* cxt) : cxt(cxt) {}

  Program_Decl* start_translation() { return cxt->tranUnit; }
  Decl* finish_translation(Decl* d) { return d; }

  Expr* standard_conversion(Expr* e, Type* t);
  Expr* boolean_conversion(Expr* e);
  Expr* integer_conversion(Expr* e);
  Expr* arithmetic_conversion(Expr* e);
  Expr* function_conversion(Expr*);
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

  Decl* function_declaration(const std::string*, std::vector<Decl*>&&, Type*);
  Decl* function_completion(Decl* d) { return d; }
  Decl* function_completion(Decl*, Stmt*);
  Decl* function_parameter(Type*, const std::string*);

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
  Expr* call_expression(Expr* e, std::vector<Expr*>&& args);
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

  void push_control(Stmt* s) { control.push_back(s); }
  void pop_control() { control.pop_back(); }
  Stmt* current_control() { return (control.empty() ? nullptr : control.back()); }
  bool inside_loop();

  void enter_scope(int k) { stack.push(k); }
  void leave_scope();
  Scope* current_scope() { return stack.top(); }

  Fn_Decl* current_function() { return dynamic_cast<Fn_Decl*>(currCont); }

  void set_current_context(Decl* d) { currCont = d; }
  Decl* current_context() { return currCont; }

  Decl* unqual_lookup(const std::string*);

  Decl* currCont;
  ScopeStack stack;
};

Decl* Semantic::unqual_lookup(const std::string* symbol) {
  if (auto* dec = stack.searchName(*symbol))
    return dec;
  throw Semantic_Exception("Variable: " + *symbol + " could not be found");
}

void Semantic::leave_scope() {
  Scope* s = current_scope();
  stack.pop();
};

Expr* Semantic::standard_conversion(Expr* e, Type* t) {
  //if (dynamic_cast<Fn_Type*>(e->getType()))
  //  return e;

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

Expr* Semantic::function_conversion(Expr* e) {
  Type* t = e->getType();

  if (Ref_Type* ref = dynamic_cast<Ref_Type*>(t)) {
    t = ref->obj;
    //return new Value_Expr(e, t);
  }

  if (auto* fn = dynamic_cast<Fn_Type*>(t))
    return e;

  throw Semantic_Exception("Expected Function Type");
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
 
Stmt* Semantic::return_statement(Expr* e) {
  auto* fn = current_function();

  Expr* conv1 = standard_conversion(e, fn->ret->getType());

  Expr* ret = copy_initialize(dynamic_cast<Var_Decl*>(fn->ret), conv1);
  return new Return_Stmt(ret);
}

Stmt* Semantic::declaration_statement(Decl* d) {
 return new Decl_Stmt(d);
}
 
Stmt* Semantic::expression_statement(Expr* e) {
  return new Expr_Stmt(e);
}

Decl* Semantic::function_declaration(const std::string* id, std::vector<Decl*>&& params, Type* t) {
  Scope* parent = current_scope()->parent;
  Decl* context = current_context();

  Ret_Decl* ret = new Ret_Decl("", t, DeclContext());
  auto* fnType = cxt->Get_Fun_Type(params, ret);
  Fn_Decl* fn = new Fn_Decl(*id, std::move(params), ret, context, fnType);

  declare(parent, fn);
  return fn;
}

Decl* Semantic::function_completion(Decl* d, Stmt* body) {
  Fn_Decl* fn = static_cast<Fn_Decl*>(d);
  
  fn->body = body;

  return fn;
}

Decl* Semantic::function_parameter(Type* t, const std::string* id) {
  Param_Decl* param = new Param_Decl(*id, t, DeclContext());
  declare(current_scope(), param);
  return param;
}

Expr* Semantic::assign_expression(Expr* e1, Expr* e2) {
  Type* t = e1->getType();

  t = get_type(t);

  e2 = standard_conversion(e2, get_type(t));
  
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
  Expr* conv1 = arithmetic_conversion(e1);
  Expr* conv2 = arithmetic_conversion(e2);
  check_common(conv1, conv2);
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

Expr* Semantic::call_expression(Expr* e, std::vector<Expr*>&& args) {
  Expr* call = function_conversion(e);

  Fn_Type* t = dynamic_cast<Fn_Type*>(call->getType());

  if (!t)
    throw Semantic_Exception("Callee is not a function");

  std::vector<Type*>& params = t->params;
  if (args.size() < params.size())
    throw Semantic_Exception("Not enough arguments");
  if (args.size() > params.size())
    throw Semantic_Exception("Too many arguments");

  std::vector<Expr*> exprs;
  auto pi = params.begin();
  auto ai = args.begin();
  while(pi != params.end() && ai != args.end()) {
    Expr* call = copy_initialize(*pi, standard_conversion(*ai, *pi));
    exprs.push_back(call);
    ++pi;
    ++ai;
  }
  Type* ret = t->ret;

  return new Call_Expr(call, ret, std::move(exprs));
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
  if (!(dynamic_cast<Fn_Decl*>(d)))
      t = cxt->Get_Ref_Type(t);

  //throw Semantic_Exception("Need to implement Get Ref Type");

  return new Ref_Expr(dynamic_cast<Name_Decl*>(d), t);
}


Var_Decl* Semantic::variable_declaration(const std::string* id, Type* t) {
  auto* owner = current_function();

  Var_Decl* dec = new Var_Decl(*id, t, owner);
  dec->type = t;

  declare(current_scope(), dec);

  return dec;
}

Var_Decl* Semantic::variable_complete(Expr* e, Var_Decl* d) {
  d->init = copy_initialize(d, e);

  return d;
}

Expr* Semantic::copy_initialize(Var_Decl* var, Expr* e) {
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

bool Semantic::inside_loop() {
  if (Stmt* s = current_control()) {
    if (dynamic_cast<While_Stmt*>(s))
      return true;
  }
  return false;
}

void Semantic::declare(Scope* s, Name_Decl* d) {
  if(!s->addToScope(d))
    throw Semantic_Exception("Variable already declared in this scope");
}
#endif

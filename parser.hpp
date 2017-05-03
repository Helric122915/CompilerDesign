#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include "eval.hpp"
#include "evalDecl.hpp"
#include "token.hpp"
#include "semantics.hpp"
#include "declarative.hpp"

class Parser {
private:
  std::vector<Token*> tokens;
  std::vector<Token*>::iterator it;
  int numberRepOut;
  ASTcontext* cxt;
  Semantic* sema;

  bool eof() const { return it == tokens.end(); }
  Token* lookahead() const { return (eof() ? new Punct_Token(Eof_Tok) : *it); }
  Token* consume();
  Token* require(Token_Kind k);
  bool match(Token_Kind k);
  bool match_if(Token_Kind k);
  bool match_if(Token_Kind k, Token*&);
  std::vector<Decl*> topLevel_declaration_sequence();
  Decl* topLevel_declaration();
  Decl* block_declaration();
  Decl* function_declaration();
  std::vector<Decl*> function_parameter_list();
  Decl* function_parameter();
  Stmt* statement();
  std::vector<Stmt*> statement_seq();
  Stmt* block_statement();
  Stmt* if_statement();
  Stmt* while_statement();
  Stmt* break_statement();
  Stmt* continue_statement();
  Stmt* return_statement();
  Stmt* declaration_statement();
  Stmt* expression_statement();
  Decl* declaration();
  Decl* variable_declaration();
  Type* type_specifier();
  Type* simple_type_specifier();
  Expr* parseExpr();
  Expr* parseAssignExpr();
  Expr* parseLogicOrExpr();
  Expr* parseLogicAndExpr();
  Expr* parseBitWiseOrExpr();
  Expr* parseBitWiseXorExpr();
  Expr* parseBitWiseAndExpr();
  Expr* parseEqualExpr();
  Expr* parseOrderExpr();
  Expr* parseAddExpr();
  Expr* parseMultExpr();
  Expr* parseUnaryExpr();
  Expr* parsePrimaryExpr();
  const std::string* identifier();
  Expr* parseIdExpr();

public:
  Parser(std::vector<Token*> tokens, int repOut, ASTcontext* cxt) : tokens(tokens), numberRepOut(repOut), cxt(cxt) { it = this->tokens.begin(); sema = new Semantic(cxt); }
  ~Parser() = default;

  Decl* translate();
};

// Returns the token that is consumed and advances the iterator.
Token* Parser::consume() {
  Token* tok = lookahead();

  std::cout << "Current Token: " << printName(lookahead()->name) << '\n';

  ++it;
  return tok;
}

// Attempts to match to a specific Token_Kind and if it fails throws an exception.
Token* Parser::require(Token_Kind k) {
  if (lookahead()->name == k)
    return consume();
  else
    throw Syntax_Exception("Expected a token of type " + printName(k));
}

bool Parser::match(Token_Kind k) {
  return lookahead()->name == k;
}

// Attempts to match to a specific Token_Kind and returns false if it fails.
bool Parser::match_if(Token_Kind k) {
  if (lookahead()->name == k) {
    consume();
    return true;
  }
  else
    return false;
}

// Does the same as the other match_if but accepts a Token* by reference that can be used
// outside the function.
bool Parser::match_if(Token_Kind k, Token* & val) {
  if (lookahead()->name == k) {
    val = lookahead();
    consume();
    return true;
  }
  else
    return false;
}

Decl* Parser::translate() {
  std::cout << "Starting program translation\n";
  Program_Decl* program = sema->start_translation();

  Declarative_Region region(sema, global_scope, program);

  program->decls = topLevel_declaration_sequence();

  return sema->finish_translation(program);
}

std::vector<Decl*> Parser::topLevel_declaration_sequence() {
  std::cout << "Starting top level decl seq\n";
  std::vector<Decl*> decls;
  while(!eof()) {
    Decl* d = topLevel_declaration();
    decls.push_back(d);
  }
  std::cout << "Return top level decls\n";
  return decls;
}

Decl* Parser::topLevel_declaration() {
  switch(lookahead()->name) {
  case Def_Kw:
    return function_declaration();
  default:
    throw Semantic_Exception("Expected Function Declaration");
  }
}

Decl* Parser::function_declaration() {
  std::cout << "Parsing a function\n";
  require(Def_Kw);
  const std::string* id = identifier();

  Declarative_Region param_region(sema, function_param_scope);
  std::vector<Decl*> params;
  require(LParens_Tok);
  if (lookahead()->name != RParens_Tok)
    params = function_parameter_list();
  require(RParens_Tok);

  require(Arrow_Tok);
  Type* t = type_specifier();

  Decl* fn = sema->function_declaration(id, std::move(params), t);

  Declarative_Region function_region(sema, function_scope, fn);
  Stmt* s = block_statement();
  return sema->function_completion(fn, s);
}

std::vector<Decl*> Parser::function_parameter_list() {
  std::cout << "Parsing function params\n";
  std::vector<Decl*> params;
  while(true) {
    Decl* param = function_parameter();
    params.push_back(param);
    if (match_if(Comma_Tok))
      continue;
    if (match_if(RParens_Tok))
      break;
    else
      throw Semantic_Exception("Expected Function Parameter");
  }
  return params;
}

Decl* Parser::function_parameter() {
  Type* t = type_specifier();
  if (match_if(Ident_Tok)) {
    const std::string* id = identifier();
    return sema->function_parameter(t, id);
  }
  else
    throw Semantic_Exception("Expected Function Parameter Identifier");
}

Stmt* Parser::statement() {
  std::cout << "Parsing Statement\n";
  switch (lookahead()->name) {
  case LBrace_Tok:
    return block_statement();
  case If_Kw:
    return if_statement();
  case While_Kw:
    return while_statement();
  case Break_Kw:
    return break_statement();
  case Continue_Kw:
    return continue_statement();
  case Return_Kw:
    return return_statement();
  case Var_Kw:
    return declaration_statement();
  default:
    return expression_statement();
  }
}

std::vector<Stmt*> Parser::statement_seq() {
  std::vector<Stmt*> stmtSeq;

  while (lookahead()->name != RBrace_Tok) {
    std::cout << "Parsing new Statement Seq Statement\n";
    std::cout << printName(lookahead()->name) << " <- next token\n";
    Stmt* s = statement();
    stmtSeq.push_back(s);
  }
  return stmtSeq;
}

Stmt* Parser::block_statement() {
  Declarative_Region region(sema, block_scope);
  std::cout << "Parsing Block Statement\n";

  require(LBrace_Tok);
  std::vector<Stmt*> stmtSeq;
  if (lookahead()->name != RBrace_Tok)
    stmtSeq = statement_seq();
  std::cout << "Returned statement seq.\n";
  match_if(RBrace_Tok);  
  return sema->block_statement(std::move(stmtSeq));
}

Stmt* Parser::if_statement() {
  std::cout << "Parsing If Statement\n";
  require(If_Kw);
  require(LParens_Tok);
  Expr* e = parseExpr();
  require(RParens_Tok);
  Stmt* trueBlock = statement();
  
  Stmt* falseBlock = nullptr;
  if (match_if(Else_Kw)) {
    std::cout << "Matched Else Kw\n";
    falseBlock = statement();
  }
  return sema->if_statement(e, trueBlock, falseBlock);
}

Stmt* Parser::while_statement() {
  std::cout << "Parsing While Statement\n";
  require(While_Kw);
  require(LParens_Tok);
  Expr* e = parseExpr();
  require(RParens_Tok);
  Stmt* l = sema->while_init(e);
  std::cout << "Parsing body of while loop\n";
  Stmt* s = statement();
  return sema->while_complete(l, s);
}

Stmt* Parser::break_statement() {
  std::cout << "Parsing Break Statement\n";
  require(Break_Kw);
  require(Semicolon_Tok);
  return sema->break_statement();
}

Stmt* Parser::continue_statement() {
  std::cout << "Parsing Continue Statement\n";
  require(Continue_Kw);
  require(Semicolon_Tok);
  return sema->continue_statement();
}

Stmt* Parser::return_statement() {
  std::cout << "Parsing Return Statement\n";
  require(Return_Kw);
  Expr* e = parseExpr();
  match_if(Semicolon_Tok);
  return sema->return_statement(e);
}

Stmt* Parser::declaration_statement() {
  return new Decl_Stmt(declaration());
}

Stmt* Parser::expression_statement() {
  Expr* exp = parseExpr();
  require(Semicolon_Tok);
  return new Expr_Stmt(exp);
}

Decl* Parser::declaration() {
  require(Var_Kw);
  return variable_declaration();
}

Decl* Parser::variable_declaration() {
  // Retrieves the specified type and identifier.
  std::cout << "Parsing Variable Declaration\n";
  Type* t = type_specifier();
  const std::string* id = identifier();

  Local_Var_Decl* dec = sema->variable_declaration(id, t);

  require(Assign_Tok);  

  Expr* expr = parseExpr();

  if (t != expr->getType())
    throw Type_Exception("Type does not match declaration");

  print(expr); std::cout << "<- e in variable decl\n";

  Local_Var_Decl* initDecl = sema->variable_complete(expr, dec);

  require(Semicolon_Tok);

  return initDecl;
}

Type* Parser::type_specifier() {
  return simple_type_specifier();
}

Type* Parser::simple_type_specifier() {
  switch(lookahead()->name) {
  case Int_Kw: consume();
    return (*cxt).Int_;
  case Bool_Kw: consume();
    return (*cxt).Bool_;
  default:
    throw Syntax_Exception("Expected type after var keyword.");
  }
}

Expr* Parser::parseExpr() {
  return parseAssignExpr();
}

Expr* Parser::parseAssignExpr() {
  std::cout << "Parsing Assign Expr\n";
  Expr* t1 = parseLogicOrExpr();
  if (match_if(QuestionMark_Tok)) {
    Expr* t2 = parseExpr();
    require(Colon_Tok);
    return sema->condition_expression(t1, t2, parseExpr());    
  }
  else if (match_if(Assign_Tok)) {
    Expr* t2 = parseAssignExpr();

    if (!t2)
      std::cout << "Right side don't equal sometin yo!\n";
    else
      std::cout << "Right side does equal sometin yo!\n";

    print(t2);
    std::cout << '\n';

    return sema->assign_expression(t1, t2);
  }
  return t1;
}

Expr* Parser::parseLogicOrExpr() {
  Expr* t1 = parseLogicAndExpr();

  while (true) {
    if (match_if(OrElse_Tok))
      t1 = sema->logical_or_expression(t1, parseLogicAndExpr());
    else
      break;
  }
  return t1;
}

Expr* Parser::parseLogicAndExpr() {
  Expr* t1 = parseBitWiseOrExpr();

  while (true) {
    if (match_if(AndThen_Tok))
      t1 = sema->logical_and_expression(t1, parseBitWiseOrExpr());
    else
      break;
  }
  return t1;
}

Expr* Parser::parseBitWiseOrExpr() {
  Expr* t1 = parseBitWiseXorExpr();

  while (true) {
    if (match_if(Or_Tok))
      t1 = sema->or_expression(t1, parseBitWiseXorExpr());
    else
      break;
  }
  return t1;
}

Expr* Parser::parseBitWiseXorExpr() {
  Expr* t1 = parseBitWiseAndExpr();

  while (true) {
    if (match_if(Xor_Tok))
      t1 = sema->xor_expression(t1, parseBitWiseAndExpr());
    else
      break;
  }
  return t1;
}

Expr* Parser::parseBitWiseAndExpr() {
  Expr* t1 = parseEqualExpr();

  while (true) {
    if (match_if(And_Tok))
      t1 = sema->and_expression(t1, parseEqualExpr());
    else
      break;
  }
  return t1;
}

Expr* Parser::parseEqualExpr() {
  Expr* t1 = parseOrderExpr();

  while (true) {
    if (match_if(Eq_Tok))
      t1 = sema->equal_expression(t1, parseOrderExpr());
    else if (match_if(NotEq_Tok))
      t1 = sema->not_equal_expression(t1, parseOrderExpr());
    else
      break;
  }
  return t1;
}

Expr* Parser::parseOrderExpr() {
  Expr* t1 = parseAddExpr();

  while (true) {
    if (match_if(LessThan_Tok))
      t1 = sema->less_than_expression(t1, parseAddExpr());
    else if (match_if(LessEqThan_Tok))
      t1 = sema->less_eq_than_expression(t1, parseAddExpr());
    else if (match_if(GreaterThan_Tok))
      t1 = sema->greater_than_expression(t1, parseAddExpr());
    else if (match_if(GreaterEqThan_Tok))
      t1 = sema->greater_eq_than_expression(t1, parseAddExpr());
    else
      break;
  }
  return t1;
}

Expr* Parser::parseAddExpr() {
  Expr* t1 = parseMultExpr();

  while (true) {
    if (match_if(Plus_Tok))
      t1 = sema->add_expression(t1, parseMultExpr());
    else if (match_if(Minus_Tok))
      t1 = sema->sub_expression(t1, parseMultExpr());
    else
      break;
  }
  return t1;
}

Expr* Parser::parseMultExpr() {
  Expr* t1 = parseUnaryExpr();

  while (true) {
    if (match_if(Mult_Tok))
      t1 = sema->mul_expression(t1, parseUnaryExpr());
    else if (match_if(Div_Tok))
      t1 = sema->div_expression(t1, parseUnaryExpr());
    else if (match_if(Mod_Tok))
      t1 = sema->mod_expression(t1, parseUnaryExpr());
    else
      break;
  }
  return t1;
}

Expr* Parser::parseUnaryExpr() {
  if (match_if(Minus_Tok))
    return sema->negation_expression(parseUnaryExpr());
  else if (match_if(OneComplement_Tok))
    return sema->one_complement_expression(parseUnaryExpr());
  else if (match_if(Not_Tok))
    return sema->not_expression(parseUnaryExpr());
  else
    return parsePrimaryExpr();
}

Expr* Parser::parsePrimaryExpr() {
  Token* temp;

  if (match_if(Int_Tok,temp)) {
    Int_Token* token = dynamic_cast<Int_Token*>(temp);
    return new Int_Expr(token->getValue(), token->getRep(), cxt);
  }
  else if (match_if(Bool_Tok,temp))
    return new Bool_Expr(dynamic_cast<Bool_Token*>(temp)->getValue(), cxt);
  else if (match(Ident_Tok))
    return parseIdExpr();
  else if (match_if(LParens_Tok)) {
    Expr* expr = parseExpr();
    require(RParens_Tok);
    return expr;
  }
  else
    throw Syntax_Exception("Expecting Primary Expression");
}

const std::string* Parser::identifier() {
  std::cout << "I'm parsing an identifier\n";
  Token* ident = require(Ident_Tok);
  return sema->identifier(ident);
}

Expr* Parser::parseIdExpr() {
  std::cout << "I'm identifing an identifier\n";
  const std::string* ident = identifier();
  return sema->id_expression(ident);
}
#endif

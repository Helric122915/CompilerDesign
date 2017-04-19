#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include "eval.hpp"
#include "token.hpp"
#include "semantics.hpp"

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
  bool match_if(Token_Kind k);
  bool match_if(Token_Kind k, Token*&);
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
  const Type* type_specifier();
  const Type* simple_type_specifier();
  Expr* parseExpr();
  Expr* parseCondExpr();
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
  const std::string identifier();

public:
  Parser(std::vector<Token*> tokens, int repOut, ASTcontext* cxt) : tokens(tokens), numberRepOut(repOut), cxt(cxt) { it = this->tokens.begin(); sema = new Semantic(cxt); }
  ~Parser() = default;

  Stmt* next() { return statement(); }
};

// Returns the token that is consumed and advances the iterator.
Token* Parser::consume() {
  Token* tok = lookahead();
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

Stmt* Parser::statement() {
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
    Stmt* s = statement();
    stmtSeq.push_back(s);
  }
  return stmtSeq;
}

Stmt* Parser::block_statement() {
  // add declarative region

  require(LBrace_Tok);
  std::vector<Stmt*> stmtSeq;
  if (lookahead()->name != RBrace_Tok)
    stmtSeq = statement_seq();
  match_if(RBrace_Tok);  
  return sema->block_statement(std::move(stmtSeq));
}

Stmt* Parser::if_statement() {
  require(If_Kw);
  match_if(LParens_Tok);
  Expr* e = parseExpr();
  match_if(RParens_Tok);
  Stmt* trueBlock = statement();
  match_if(Else_Kw);
  Stmt* falseBlock = statement();
  return sema->if_statement(e, trueBlock, falseBlock);
}

Stmt* Parser::while_statement() {
  require(While_Kw);
  match_if(LParens_Tok);
  Expr* e = parseExpr();
  match_if(RParens_Tok);
  Stmt* l = sema->while_init(e);
  Stmt* s = statement();
  return sema->while_complete(l, s);
}

Stmt* Parser::break_statement() {
  require(Break_Kw);
  return sema->break_statement();
}

Stmt* Parser::continue_statement() {
  require(Continue_Kw);
  return sema->continue_statement();
}

Stmt* Parser::return_statement() {
  require(Return_Kw);
  // only used for void I believe
  //if (match_if(Semicolon_Tok))
  //  return new Return_Stmt();
  Expr* e = parseExpr();
  match_if(Semicolon_Tok);
  return sema->return_statement(e);
}

Stmt* Parser::declaration_statement() {
  //Decl* dec = declaration();
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
  const Type* t = type_specifier();
  const std::string id = identifier();

  // If the decl already exists in the Symbol Table that means it is already in the table and
  // can't be re-declared.
  if (Decl* dec = (*cxt).retrieveSymbol(id))
      throw Semantic_Exception("Variable " + id + " is already defined.");
  
  // Create a new decl that will be stored with the identifier in the Symbol Table.
  Var_Decl* dec = new Var_Decl(id);
  dec->type = t;
  require(Assign_Tok);
  Expr* expr = parseExpr();

  // Throw an exception if the specified type doesn't match the associated expr type.
  if (t != expr->getType())
    throw Type_Exception("Type does not match declaration");

  // Pre-evaluates the expr to only have to store either an int or bool expr in the decl.
  if (t == (*cxt).Int_)
    expr = new Int_Expr(eval(expr), numberRepOut, cxt);
  else if (t == (*cxt).Bool_)
    expr = new Bool_Expr(eval(expr), cxt);

  dec->init = expr;

  // Stores the decl into Symbol Table with the same identifier.
  (*cxt).insertDecl(dec);

  require(Semicolon_Tok);
  return dec;
}

const Type* Parser::type_specifier() {
  return simple_type_specifier();
}

const Type* Parser::simple_type_specifier() {
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
  return parseCondExpr();
}

Expr* Parser::parseCondExpr() {
  Expr* t1 = parseLogicOrExpr();

  while (true) {
    if (match_if(QuestionMark_Tok)) {
      Expr* t2 = parseExpr();
      if (match_if(Colon_Tok))
	t1 = sema->condition_expression(t1, t2, parseExpr());
    }
    else
      break;
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
  else if (match_if(LParens_Tok)) {
    Expr* expr = parseExpr();

    require(RParens_Tok);
    return expr;
  }
  else if (match_if(Ident_Tok, temp)) {
    std::string variable = *dynamic_cast<Ident_Token*>(temp)->value;

    // Checks to see if the variable exists in the Symbol Table.
    Var_Decl* dec = dynamic_cast<Var_Decl*>((*cxt).retrieveSymbol(variable));

    // If the variable doesn't exist throw an exception.
    if (!dec)
      throw Semantic_Exception("Variable " + variable + " has not been defined.");

    // If an assignment operator is next then the variable is having its decl updated, otherwise
    // just return the init expr.
    if (match_if(Assign_Tok)) {
      Expr* expr = parseExpr();

      // Pre-evaluates the expr as an int or bool expr before it is stored within the decl.
      if (expr->getType() == (*cxt).Int_)
	expr = new Int_Expr(eval(expr), numberRepOut, cxt);
      else if (expr->getType() == (*cxt).Bool_)
	expr = new Bool_Expr(eval(expr), cxt);

      // Updates the expr stored within the decl.
      dec->init = expr;
      (*cxt).insertDecl(dec);

      return expr;
    }
    return dec->init;
  }
  else
    throw Syntax_Exception("Expecting Primary Expression");
}

const std::string Parser::identifier() {
  Token* id = require(Ident_Tok);
  return *dynamic_cast<Ident_Token*>(id)->value;
}
#endif

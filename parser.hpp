#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include "int_expr.hpp"
#include "bool_expr.hpp"
#include "eval.hpp"
#include "token.hpp"
#include "stmt.hpp"

class Parser {
private:
  std::vector<Token*> tokens;
  std::vector<Token*>::iterator it;
  int numberRepOut;
  ASTcontext* cxt;

  bool eof() const { return it == tokens.end(); }
  Token* lookahead() const { return (eof() ? new Punct_Token(Eof_Tok) : *it); }
  Token* consume();
  Token* match(Token_Kind k);
  bool match_if(Token_Kind k);
  bool match_if(Token_Kind k, Token*&);
  Stmt* statement();
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
  Parser(std::vector<Token*> tokens, int repOut, ASTcontext* cxt) : tokens(tokens), numberRepOut(repOut), cxt(cxt) { it = this->tokens.begin(); }
  ~Parser() = default;

  //Expr* next() { return parseExpr(); }
  Stmt* next() { return statement(); }
};

Token* Parser::consume() {
  Token* tok = lookahead();
  ++it;
  return tok;
}

Token* Parser::match(Token_Kind k) {
  if (lookahead()->name == k)
    return consume();
  else
    throw Syntax_Exception("Expected: " + printName(k));
}

bool Parser::match_if(Token_Kind k) {
  if (lookahead()->name == k) {
    consume();
    return true;
  }
  else
    return false;
}

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
  case Var_Kw:
    return declaration_statement();
  default:
    return expression_statement();
  }
}

Stmt* Parser::declaration_statement() {
  Decl* dec = declaration();
  return new Decl_Stmt(dec);
}

Stmt* Parser::expression_statement() {
  Expr* exp = parseExpr();
  match_if(Semicolon_Tok);
  return new Expr_Stmt(exp);
}

Decl* Parser::declaration() {
  match(Var_Kw);
  return variable_declaration();
}

Decl* Parser::variable_declaration() {
  const Type* t = type_specifier();
  const std::string id = identifier();

  if (Decl* dec = (*cxt).retrieveSymbol(id))
      throw Semantic_Exception("Variable " + id + " is already defined.");
  
  Var_Decl* dec = new Var_Decl(id);
  dec->type = t;
  match(Assign_Tok);
  Expr* expr = parseExpr();

  if (t != expr->getType())
    throw Type_Exception("Type does not match declaration");

  if (t == (*cxt).Int_)
    expr = new Int_Expr(eval(expr), numberRepOut, cxt);
  else if (t == (*cxt).Bool_)
    expr = new Bool_Expr(eval(expr), cxt);

  dec->init = expr;

  (*cxt).insertDecl(dec);

  match_if(Semicolon_Tok);
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
  //std::cout << "Parsing Cond\n";

  Expr* t1 = parseLogicOrExpr();

  while (true) {
    if (match_if(QuestionMark_Tok)) {
      Expr* t2 = parseExpr();
      if (match_if(Colon_Tok))
	t1 = new Cond_Expr(t1,t2, parseExpr(), cxt);
    }
    else
      break;
  }

  return t1;
}

Expr* Parser::parseLogicOrExpr() {
  //std::cout << "Parsing Logic Or\n";

  Expr* t1 = parseLogicAndExpr();

  while (true) {
    if (match_if(OrElse_Tok))
      t1 = new OrElse_Expr(t1, parseLogicAndExpr(), cxt);
    else
      break;
  }

  return t1;
}

Expr* Parser::parseLogicAndExpr() {
  //std::cout << "Parsing Logic And\n";

  Expr* t1 = parseBitWiseOrExpr();

  while (true) {
    if (match_if(AndThen_Tok))
      t1 = new AndThen_Expr(t1, parseBitWiseOrExpr(), cxt);
    else
      break;
  }
  
  return t1;
}

Expr* Parser::parseBitWiseOrExpr() {
  Expr* t1 = parseBitWiseXorExpr();

  while (true) {
    if (match_if(Or_Tok))
      t1 = new Or_Expr(t1, parseBitWiseXorExpr(), cxt);
    else
      break;
  }

  return t1;
}

Expr* Parser::parseBitWiseXorExpr() {
  Expr* t1 = parseBitWiseAndExpr();

  while (true) {
    if (match_if(Xor_Tok))
      t1 = new Xor_Expr(t1, parseBitWiseAndExpr(), cxt);
    else
      break;
  }

  return t1;
}

Expr* Parser::parseBitWiseAndExpr() {
  Expr* t1 = parseEqualExpr();

  while (true) {
    if (match_if(And_Tok))
      t1 = new And_Expr(t1, parseEqualExpr(), cxt);
    else
      break;
  }

  return t1;
}

Expr* Parser::parseEqualExpr() {
  //std::cout << "Parsing Equal\n";

  Expr* t1 = parseOrderExpr();

  while (true) {
    if (match_if(Eq_Tok))
      t1 = new Eq_Expr(t1, parseOrderExpr(), cxt);
    else if (match_if(NotEq_Tok))
      t1 = new NotEq_Expr(t1, parseOrderExpr(), cxt);
    else
      break;
  }
  
  return t1;
}

Expr* Parser::parseOrderExpr() {
  //std::cout << "Parsing Order\n";

  Expr* t1 = parseAddExpr();

  while (true) {
    if (match_if(LessThan_Tok))
      t1 = new LessThan_Expr(t1, parseAddExpr(), cxt);
    else if (match_if(LessEqThan_Tok))
      t1 = new LessEqThan_Expr(t1, parseAddExpr(), cxt);
    else if (match_if(GreaterThan_Tok))
      t1 = new GreaterThan_Expr(t1, parseAddExpr(), cxt);
    else if (match_if(GreaterEqThan_Tok))
      t1 = new GreaterEqThan_Expr(t1, parseAddExpr(), cxt);
    else
      break;
  }
  
  return t1;
}

Expr* Parser::parseAddExpr() {
  //std::cout << "Parsing Add\n";

  Expr* t1 = parseMultExpr();

  while (true) {
    if (match_if(Plus_Tok))
      t1 = new Add_Expr(t1, parseMultExpr(), cxt);
    else if (match_if(Minus_Tok))
    t1 = new Sub_Expr(t1, parseMultExpr(), cxt);
    else
      break;
  }
  
  return t1;
}

Expr* Parser::parseMultExpr() {
  //std::cout << "Parsing Mult\n";

  Expr* t1 = parseUnaryExpr();

  while (true) {
    if (match_if(Mult_Tok))
      t1 = new Mult_Expr(t1, parseUnaryExpr(), cxt);
    else if (match_if(Div_Tok))
      t1 = new Div_Expr(t1, parseUnaryExpr(), cxt);
    else if (match_if(Mod_Tok))
      t1 = new Mod_Expr(t1, parseUnaryExpr(), cxt);
    else
      break;
  }
  
  return t1;
}

Expr* Parser::parseUnaryExpr() {
  //std::cout << "Parsing Unary\n";

  if (match_if(Minus_Tok))
    return new Negation_Expr(parseUnaryExpr(), cxt);
  else if (match_if(OneComplement_Tok))
    return new OneComplement_Expr(parseUnaryExpr(), cxt);
  else if (match_if(Not_Tok))
    return new Not_Expr(parseUnaryExpr(), cxt);
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

    if (!match_if(RParens_Tok))
      throw Syntax_Exception("Missing Expected Closing Paren");
    else
      return expr;
  }
  else if (match_if(Ident_Tok, temp)) {
    std::string variable = *dynamic_cast<Ident_Token*>(temp)->value;
    Var_Decl* dec = dynamic_cast<Var_Decl*>((*cxt).retrieveSymbol(variable));//*dynamic_cast<Ident_Token*>(temp)->value));

    if (!dec)
      throw Semantic_Exception("Variable " + variable + " has not been defined.");

    if (match_if(Assign_Tok)) {
      Expr* expr = parseExpr();

      if (expr->getType() == (*cxt).Int_)
	expr = new Int_Expr(eval(expr), numberRepOut, cxt);
      else if (expr->getType() == (*cxt).Bool_)
	expr = new Bool_Expr(eval(expr), cxt);

      dec->init = expr;
      (*cxt).insertDecl(dec);

      return expr;
    }

    if (dec)
      return dec->init;
    else
      throw Semantic_Exception("Variable " + variable + " has no definition");
  }
  else
    throw Syntax_Exception("Expecting Primary Expression");
}

const std::string Parser::identifier() {
  Token* id = match(Ident_Tok);
  return *dynamic_cast<Ident_Token*>(id)->value;
}
#endif

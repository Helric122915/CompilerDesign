#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include "int_expr.hpp"
#include "bool_expr.hpp"
#include "token.hpp"

class Parser {
private:
  std::vector<Token*> tokens;
  std::vector<Token*>::iterator it;
  ASTcontext cxt;

  bool eof() const { return it == tokens.end(); }
  Token* lookahead() const { return (eof() ? new Punct_Token(Eof_Tok) : *it); }
  void consume() { ++it; }
  void match(Token_Kind k);
  bool match_if(Token_Kind k);
  bool match_if(Token_Kind k, Token*&);
  Expr* parseExpr();
  Expr* parseCondExpr();
  Expr* parseLogicOrExpr();
  Expr* parseLogicAndExpr();
  Expr* parseEqualExpr();
  Expr* parseOrderExpr();
  Expr* parseAddExpr();
  Expr* parseMultExpr();
  Expr* parseUnaryExpr();
  Expr* parsePrimaryExpr();

public:
  Parser(std::vector<Token*> tokens, ASTcontext cxt) : tokens(tokens), cxt(cxt) { it = this->tokens.begin(); }
  ~Parser() = default;

  Expr* next() { return parseExpr(); }
};

void Parser::match(Token_Kind k) {
  if (lookahead()->name == k)
    consume();
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
    else;
    break;
  }

  return t1;
}

Expr* Parser::parseLogicOrExpr() {
  //std::cout << "Parsing Logic Or\n";

  Expr* t1 = parseLogicAndExpr();

  while (true) {
    if (match_if(Or_Tok))
      t1 = new Or_Expr(t1, parseLogicAndExpr(), cxt);
    else if (match_if(Xor_Tok))
      t1 = new Xor_Expr(t1, parseLogicAndExpr(), cxt);
    else if (match_if(OrElse_Tok))
      t1 = new OrElse_Expr(t1, parseLogicAndExpr(), cxt);
    else
      break;
  }

  return t1;
}

Expr* Parser::parseLogicAndExpr() {
  //std::cout << "Parsing Logic And\n";

  Expr* t1 = parseEqualExpr();

  while (true) {
    if (match_if(And_Tok))
      t1 = new And_Expr(t1, parseEqualExpr(), cxt);
    else if (match_if(AndThen_Tok))
      t1 = new AndThen_Expr(t1, parseEqualExpr(), cxt);
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

  //std::cout << "Parsing Primary\n";

  if (match_if(Int_Tok,temp)) {
    Int_Token* token = dynamic_cast<Int_Token*>(temp);
    return new Int_Expr(token->getValue(), token->getRep(), cxt);
    //return new Int_Expr(dynamic_cast<Int_Token*>(temp)->getValue(), cxt);
  }
  else if (match_if(Bool_Tok,temp))
    return new Bool_Expr(dynamic_cast<Bool_Token*>(temp)->getValue(), cxt);
  else if (match_if(LParens_Tok)) {
    Expr *ex = parseExpr();

    if (!match_if(RParens_Tok))
      throw Syntax_Exception("Missing Expected Closing Paren");
    else
      return ex;
  }
  else
    throw Syntax_Exception("Expecting Primary Expression");
}
#endif

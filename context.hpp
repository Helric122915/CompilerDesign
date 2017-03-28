#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include "type.hpp"
#include "token.hpp"
#include "decl.hpp"

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

// Context class that creates the bool and int type that will be held as type* throughout the
// program.
class ASTcontext {
public:
  const Bool_Type* Bool_ = new Bool_Type();
  const Int_Type* Int_ = new Int_Type();
  std::unordered_map<std::string, Token*> Keywords;
  std::unordered_map<std::string, Decl*> SymTab;

  ASTcontext();
  ~ASTcontext() = default;

  Token* checkKeywords(std::string);
  Token* insertSymbol(std::string);
  bool insertDecl(Decl*);
  Decl* retrieveSymbol(std::string);
};

ASTcontext::ASTcontext() {
  Keywords.insert({"var", new Ident_Token(Var_Kw)});
  Keywords.insert({"int", new Ident_Token(Int_Kw)});
  Keywords.insert({"bool", new Ident_Token(Bool_Kw)});
  Keywords.insert({"true", new Bool_Token(true)});
  Keywords.insert({"false", new Bool_Token(false)});
}

Token* ASTcontext::checkKeywords(std::string identifier) {
  auto iter = Keywords.find(identifier);
  if (iter == Keywords.end())
    return nullptr;
  else
    return iter->second;
}

Token* ASTcontext::insertSymbol(std::string identifier) {
  Ident_Token* tok = new Ident_Token(Ident_Tok);

  auto iter = SymTab.find(identifier);
  if (iter != SymTab.end()) {
    tok->value = &(iter->first);
    return tok;
  }
  else {
    auto result = SymTab.insert({identifier, nullptr});
    iter = result.first;
    tok->value = &(iter->first);
    return tok;
  }
}

bool ASTcontext::insertDecl(Decl* dec) {
  auto iter = SymTab.find(dynamic_cast<Var_Decl*>(dec)->getName());
  if (iter != SymTab.end()) {
    iter->second = dec;
    return true;
  }
  else
    return false;
}

Decl* ASTcontext::retrieveSymbol(std::string identifier) {
  auto iter = SymTab.find(identifier);
  if (iter != SymTab.end())
    return iter->second;

  return nullptr;
}
#endif

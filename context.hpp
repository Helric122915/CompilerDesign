#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <unordered_set>
#include "token.hpp"
#include "decl.hpp"
#include "type.hpp"

class Semantic;
class Assign_Expr;
class Value_Expr;
class Ref_Expr;
class Init_Expr;
class Bind_Expr;
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

/*
template<typename T>
class Term_Hash {
private:
public:
  std::size_t operator()(const T& t) const noexcept
  {
    hasher h;
    hash(h, &t);
    return h;
  }
};

template<typename T>
class Term_Eq {
  bool operator()(const T& a, const T& b) const noexcept {
    return equal(&a, &b);
  }
};

template<typename T>
using Canon_Set = std::unordered_set<T, Term_Hash<T>, Term_Eq<T>>;
*/

// Context class that creates the bool and int type that will be held as type* throughout the
// program.
class ASTcontext {
public:
  //const Bool_Type* Bool_ = new Bool_Type();
  //const Int_Type* Int_ = new Int_Type();
  Bool_Type* Bool_ = new Bool_Type();
  Int_Type* Int_ = new Int_Type();
  //Canon_Set<Ref_Type> Ref_;
  //std::unordered_set<Fn_Type> Fn_Type_;
  std::vector<Ref_Type> Ref_;
  std::unordered_map<std::string, Token*> Keywords;
  std::unordered_map<std::string, Decl*> SymTab;

  int numberRepOut;

  ASTcontext(int);
  ~ASTcontext() = default;

  Type* Get_Ref_Type(Type*);
  Token* checkKeywords(std::string);
  Token* insertSymbol(std::string);
  Decl* retrieveSymbol(std::string);
  bool insertDecl(Decl*);
};

ASTcontext::ASTcontext(int repOut) : numberRepOut(repOut) {
  std::cout << "Int Type: " << Int_ << '\n';
  std::cout << "Bool Type: " << Bool_ << '\n';

  // Base set of keywords for this language.
  Keywords.insert({"var", new Ident_Token(Var_Kw)});
  Keywords.insert({"int", new Ident_Token(Int_Kw)});
  Keywords.insert({"bool", new Ident_Token(Bool_Kw)});
  Keywords.insert({"for", new Ident_Token(For_Kw)});
  Keywords.insert({"if", new Ident_Token(If_Kw)});
  Keywords.insert({"else", new Ident_Token(Else_Kw)});
  Keywords.insert({"while", new Ident_Token(While_Kw)});
  Keywords.insert({"break", new Ident_Token(Break_Kw)});
  Keywords.insert({"continue", new Ident_Token(Continue_Kw)});
  Keywords.insert({"return", new Ident_Token(Return_Kw)});
  Keywords.insert({"def", new Ident_Token(Def_Kw)});
  Keywords.insert({"true", new Bool_Token(true)});
  Keywords.insert({"false", new Bool_Token(false)});

  Ref_.reserve(100);
}

Type* ASTcontext::Get_Ref_Type(Type* t) {
  std::cout << "Creating a ref type of type t: " << t << '\n';
  Type& ty = *(Ref_.emplace(Ref_.end(), t));

  std::cout << "Address of ty: " << &ty << '\n';
  return &ty;
}

// Checks if the identifier is stored within the keyword list.
// If it is, return the token associated with that keyword.
Token* ASTcontext::checkKeywords(std::string identifier) {
  auto iter = Keywords.find(identifier);
  if (iter == Keywords.end())
    return nullptr;
  else
    return iter->second;
}

// Checks if the Symbol is in the Symbol Table. If so it, it returns a token with the proper value.
// If not one is created with the proper value.
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

// Returns the decl associated with a given identifier if it exists.
Decl* ASTcontext::retrieveSymbol(std::string identifier) {
  auto iter = SymTab.find(identifier);
  if (iter != SymTab.end())
    return iter->second;

  return nullptr;
}

// Takes a decl and stores it with a given identifier if it exits.
bool ASTcontext::insertDecl(Decl* dec) {
  auto iter = SymTab.find(dynamic_cast<Var_Decl*>(dec)->getName());
  if (iter != SymTab.end()) {
    iter->second = dec;
    return true;
  }
  else
    return false;
}
#endif

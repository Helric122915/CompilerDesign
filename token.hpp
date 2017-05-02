#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <iostream>
#include <string>
#include <math.h>
#include "enumeration.hpp"

void printIntLexer(int,int);
std::string convertBinaryLexer(int);
std::string printName(int);

class Token {
public:
  int name;

  virtual ~Token() = default;
  virtual void print() = 0;
};

class Punct_Token : public Token {
public:
  Punct_Token(int name) { this->name = name; }

  void print() { std::cout << printName(name); }
};

class Bool_Token : public Token {
private:
  bool value;

public:
  Bool_Token(bool b) : value(b) { this->name = Bool_Tok; }

  bool getValue() { return value; }
  void print() { std::cout << printName(name) << " : " << (value ? "true" : "false"); }
};

class Int_Token : public Token {
private:
  int value;
  int rep;

public:
  Int_Token(int i, int rep) : value(i), rep(rep) { this->name = Int_Tok; }

  int getValue() { return value; }
  int getRep() { return rep; }
  void print() { std::cout << printName(name) << " : "; printIntLexer(value, rep); }
};

class Ident_Token : public Token {
public:
  Ident_Token(int name) { this->name = name; }

  const std::string* value;
  void print() { std::cout << printName(name) << (value ? " : " + *value : ""); }
};

void printIntLexer(int value, int rep) {
  switch (rep) {
  case 2: std::cout << convertBinaryLexer(value); break;
  case 10: std::cout << value; break;
  case 16: std::cout << std::hex << value; break;
  }
}

std::string convertBinaryLexer(int value) {
  int temp = abs(value);
  std::string returnString = "";

  while (temp > 0) {
    returnString.insert(0, std::to_string(temp % 2));

    temp = floor(temp / 2);
  }

  /*
  // Currently don't need because there aren't negative numbers
  // Just have the form Negation_Expr Int_Expr
  bool carry = true;
  int sum;

  if (value < 0) {
    for (int i = 0; i < returnString.length() - 1; ++i) {
      returnString[i] = (returnString[i] ? 0 : 1);
    }
    for (int i = returnString.length() -1; i > 0; --i) {
      sum = carry + returnString[i];
      carry = false;

      if (sum == 0)
	returnString[i] = 0;
      else if (sum = 1)
	returnString[i] = 1;
      else {
	returnString[i] = 1;
	carry = true;
      }
    }
  }
  */
  return returnString;
}

std::string printName(int name) {
  switch(name) {
  case Eof_Tok: return "Eof_Tok";
  case Eq_Tok: return "Eq_Tok";
  case NotEq_Tok: return "NotEq_Tok";
  case LParens_Tok: return "LParens_Tok";
  case RParens_Tok: return "RParens_Tok";
  case LessThan_Tok: return "LessThan_Tok";
  case LessEqThan_Tok: return "LessEqThan_Tok";
  case GreaterThan_Tok: return "GreaterThan_Tok";
  case GreaterEqThan_Tok: return "GreaterEqThan_Tok";
  case Plus_Tok: return "Plus_Tok";
  case Minus_Tok: return "Minus_Tok";
  case Mult_Tok: return "Mult_Tok";
  case Div_Tok: return "Div_Tok";
  case Mod_Tok: return "Mod_Tok";
  case Not_Tok: return "Not_Tok";
  case Or_Tok: return "Or_Tok";
  case OrElse_Tok: return "OrElse_Tok";
  case And_Tok: return "And_Tok";
  case AndThen_Tok: return "AndThen_Tok";
  case Xor_Tok: return "Xor_Tok";
  case OneComplement_Tok: return "OneComplement_Tok";
  case QuestionMark_Tok: return "QuestionMark_Tok";
  case Colon_Tok: return "Colon_Tok";
  case Int_Tok: return "Int_Tok";
  case Bool_Tok: return "Bool_Tok";
  case Ident_Tok: return "Ident_Tok";
  case Semicolon_Tok: return "Semicolon_Tok";
  case Assign_Tok: return "Assign_Tok";
  case LBrace_Tok: return "LBrace_Tok";
  case RBrace_Tok: return "RBrace_Tok";
  case Arrow_Tok: return "Arrow_Tok";
  case Int_Kw: return "Int_Kw";
  case Bool_Kw: return "Bool_Kw";
  case Var_Kw: return "Var_Kw";
  case For_Kw: return "For_Kw";
  case If_Kw: return "If_Kw";
  case Else_Kw: return "Else_Kw";
  case While_Kw: return "While_Kw";
  case Break_Kw: return "Break_Kw";
  case Continue_Kw: return "Continue_Kw";
  case Return_Kw: return "Return_Kw";
  case Def_Kw: return "Def_Kw";
  default: "Invalid Token";
  }
}
#endif

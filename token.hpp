#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <iostream>
#include <string>

void printIntLexer(int,int);
std::string convertBinaryLexer(int);
std::string printName(int);

enum Token_Kind {
  Eof_Tok,
  Eq_Tok,
  NotEq_Tok,
  LParens_Tok,
  RParens_Tok,
  LessThan_Tok,
  LessEqThan_Tok,
  GreaterThan_Tok,
  GreaterEqThan_Tok,
  Plus_Tok,
  Minus_Tok,
  Mult_Tok,
  Div_Tok,
  Mod_Tok,
  Not_Tok,
  Or_Tok,
  OrElse_Tok,
  And_Tok,
  AndThen_Tok,
  Xor_Tok,
  OneComplement_Tok,
  QuestionMark_Tok,
  Colon_Tok,
  Int_Tok,
  Bool_Tok,
  Ident_Tok,
  Int_Kw,
  Bool_Kw,
  Var_Kw,
  Semicolon_Tok,
  Assign_Tok
};

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
  case 0: return "Eof_Tok";
  case 1: return "Eq_Tok";
  case 2: return "NotEq_Tok";
  case 3: return "LParens_Tok";
  case 4: return "RParens_Tok";
  case 5: return "LessThan_Tok";
  case 6: return "LessEqThan_Tok";
  case 7: return "GreaterThan_Tok";
  case 8: return "GreaterEqThan_Tok";
  case 9: return "Plus_Tok";
  case 10: return "Minus_Tok";
  case 11: return "Mult_Tok";
  case 12: return "Div_Tok";
  case 13: return "Mod_Tok";
  case 14: return "Not_Tok";
  case 15: return "Or_Tok";
  case 16: return "OrElse_Tok";
  case 17: return "And_Tok";
  case 18: return "AndThen_Tok";
  case 19: return "Xor_Tok";
  case 20: return "OneComplement_Tok";
  case 21: return "QuestionMark_Tok";
  case 22: return "Colon_Tok";
  case 23: return "Int_Tok";
  case 24: return "Bool_Tok";
  case 25: return "Ident_Tok";
  case 26: return "Int_Kw";
  case 27: return "Bool_Kw";
  case 28: return "Var_Kw";
  case 29: return "Semicolon_Tok";
  case 30: return "Assign_Tok";
  default: "Invalid Token";
  }
}
#endif

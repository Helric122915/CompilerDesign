#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <iostream>
#include <string>

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

  void print() { std::cout << printName(name) << '\n'; }
};

class Bool_Token : public Token {
private:
  bool value;

public:
  Bool_Token(bool b, int name) : value(b) { this->name = name; }

  bool getValue() { return value; }
  void print() { std::cout << printName(name) << " : " << (value ? "true\n" : "false\n"); }
};

class Int_Token : public Token {
private:
  int value;

public:
  Int_Token(int i, int name) : value(i) { this->name = name; }

  int getValue() { return value; }
  void print() { std::cout << printName(name) << " : " << value << '\n'; }
};

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
    case 20: return "QuestionMark_Tok";
    case 21: return "Colon_Tok";
    case 22: return "Int_Tok";
    case 23: return "Bool_Tok";
    case 24: return "Str_Tok";
  }
}

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
  QuestionMark_Tok,
  Colon_Tok,
  Int_Tok,
  Bool_Tok,
  Str_Tok
};
#endif

#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <cctype>
#include "token.hpp"
#include "exception.hpp"

class Lexer {
private:
  std::string::iterator first;
  std::string::iterator last;
  //const char *first;
  //const char *last;
  std::string buffer;

  bool eof() const { return first == last; }
  char lookahead() const { return(eof() ? 0 : *first); }
  char ignore() { return (eof() ? 0 : *first++); }
  char consume();
  Token* lexeInt();
  Token* lexeBool(bool b);

public:
  Lexer(std::string::iterator first, std::string::iterator last) : first(first), last(last) {}
  ~Lexer() = default;

  Token* next();
};

char Lexer::consume() {
  if (eof())
    return 0;

  buffer += *first++;
  return buffer.back();
}

Token* Lexer::next() {
  buffer.clear();

  while(!eof()){
    switch(lookahead()) {
    case '(': {  consume();
	return new Punct_Token(LParens_Tok);
	  }
    case ')': { consume();
	return new Punct_Token(RParens_Tok);
    }
    case '?': { consume();
        return new Punct_Token(QuestionMark_Tok);
    }
    case ':': { consume();
        return new Punct_Token(Colon_Tok);
    }
    case '<': { consume();
	if (lookahead() == '=') {
	  consume();
	  return new Punct_Token(LessEqThan_Tok);
	}
	return new Punct_Token(LessThan_Tok);
    }
    case '>': { consume();
	if (lookahead() == '=') {
	  consume();
	  return new Punct_Token(GreaterEqThan_Tok);
	}
	return new Punct_Token(GreaterThan_Tok);
    }
    case '+': { consume();
        return new Punct_Token(Plus_Tok);
    }
    case '-': { consume();
        return new Punct_Token(Minus_Tok);
    }
    case '*': { consume();
        return new Punct_Token(Mult_Tok);
    }
    case '/': { consume();
        return new Punct_Token(Div_Tok);
    }
    case '%': { consume();
        return new Punct_Token(Mod_Tok);
    }
    case '=': { consume();
        if (lookahead() == '=') {
          consume();
          return new Punct_Token(Eq_Tok);
        }
        else {
          std::string s(1,lookahead());
          throw Token_Exception("Unexpected character: " + s + " following = operator.");
        }
    }
    case '!': { consume();
        if (lookahead() == '=') {
          consume();
          return new Punct_Token(NotEq_Tok);
        }
        return new Punct_Token(Not_Tok);
    }
    case '|': { consume();
        if (lookahead() == '|') {
          consume();
          return new Punct_Token(OrElse_Tok);
        }
        return new Punct_Token(Or_Tok);
    }
    case '&': { consume();
        if (lookahead() == '&') {
          consume();
          return new Punct_Token(AndThen_Tok);
        }
        return new Punct_Token(And_Tok);
    }
    case '^': { consume();
        return new Punct_Token(Xor_Tok);
    }
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9': return lexeInt();
    case 't': return lexeBool(true);
    case 'f': return lexeBool(false);
    case ' ': ignore();
    case '\n': ignore();
    case '\t': ignore();
      continue;
    }
  }
  return nullptr;
}

Token* Lexer::lexeInt() {
  consume();
  while(!eof() && std::isdigit(lookahead())) {
    consume();
  }

  return new Int_Token(std::stoi(buffer),Int_Tok);
}

Token* Lexer::lexeBool(bool b) {
  consume();
  if (b) {
    if (lookahead() == 'r') {
      consume();
      if (lookahead() == 'u') {
        consume();
        if (lookahead() == 'e') {
          consume();
          return new Bool_Token(true, Bool_Tok);
        }
        else
          throw Token_Exception("Expected e following u to form true boolean.");
      }
      else
        throw Token_Exception("Expected u following r to form true boolean.");
    }
    else
      throw Token_Exception("Expected r following t to form true boolean.");
  }
  else {
    if (lookahead() == 'a') {
      consume();
      if (lookahead() == 'l') {
        consume();
        if (lookahead() == 's') {
          consume();
          if (lookahead() == 'e') {
            consume();
            return new Bool_Token(false, Bool_Tok);
          }
          else
            throw Token_Exception("Expected e following s to form false boolean.");
        }
        else
          throw Token_Exception("Expected s following l to form false boolean.");
      }
      else
        throw Token_Exception("Expected l following a to form false boolean.");
    }
    else
      throw Token_Exception("Expected a following f to form false boolean.");
  }
  return nullptr;
}
#endif

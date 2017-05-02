#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include "token.hpp"
#include "exception.hpp"
#include "context.hpp"

class Lexer {
private:
  std::string::iterator first;
  std::string::iterator last;
  std::string buff;
  ASTcontext* cxt;

  bool eof() const { return first == last; }
  char lookahead() const { return(eof() ? 0 : *first); }
  char ignore() { return (eof() ? 0 : *first++); }
  void consume() { ++first; }
  char buffer();
  Token* lexeInt(char);
  Token* lexeIdentifier();
  Token* decideType();
  bool isHex();

public:
  Lexer(std::string::iterator first, std::string::iterator last, ASTcontext* cxt) : first(first), last(last), cxt(cxt) {}
  ~Lexer() = default;

  Token* next();
};

char Lexer::buffer() {
  if (eof())
    return 0;

  buff += *first++;
  return buff.back();
}

Token* Lexer::next() {
  buff.clear();

  while(!eof()) {
    switch(lookahead()) {
    case ';': { consume();
	return new Punct_Token(Semicolon_Tok);
    }
    case '(': { consume();
	return new Punct_Token(LParens_Tok);
    }
    case ')': { consume();
	return new Punct_Token(RParens_Tok);
    }    
    case '{': { consume();
	return new Punct_Token(LBrace_Tok);
    }
    case '}': { consume();
	return new Punct_Token(RBrace_Tok);
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
	if (lookahead() == '>') {
	  consume();
	  return new Punct_Token(Arrow_Tok);
	}
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
	return new Punct_Token(Assign_Tok);
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
    case '~': { consume();
	return new Punct_Token(OneComplement_Tok);
    }
    case '0': return decideType();
    case '1' ... '9': return lexeInt('d');
    case '_':
    case 'a' ... 'z': return lexeIdentifier();
    case ' ': 
    case '\n': 
    case '\t': 
    case '\v': 
    case '\f': 
    case '\r': ignore();
      continue;
    }
  }
  return nullptr;
}

// Lexes an integer based on its representation.
Token* Lexer::lexeInt(char rep) {
  if (rep == 'd') {
    while (!eof() && std::isdigit(lookahead())) {
      buffer();
    }
    return new Int_Token(std::stol(buff, nullptr, 10), cxt->numberRepOut);
  }
  else if (rep == 'h') {
    buffer();
    while (!eof() && isHex()) {
      buffer();
    }
    return new Int_Token(std::stol(buff, nullptr, 16), cxt->numberRepOut);
  }
  else if (rep == 'b') {
    buffer();
    while (!eof() && (lookahead() == '0' || lookahead() == '1')) {
	buffer();
      }
    return new Int_Token(std::stol(buff, nullptr, 2), cxt->numberRepOut);
  }
  else
    throw Token_Exception("Unexpected Integer Type");
}

// Checks if the current character is a hex value.
bool Lexer::isHex() {
  return std::isdigit(lookahead()) || tolower(lookahead()) == 'a' || tolower(lookahead()) == 'b' 
    || tolower(lookahead()) == 'c' || tolower(lookahead()) == 'd' || tolower(lookahead()) == 'e' 
    || tolower(lookahead()) == 'f';
}

// Lexes an identifier and determines if it is a keyword or a symbol.
Token* Lexer::lexeIdentifier() {
  buffer();
  while (std::isdigit(lookahead()) || std::isalpha(lookahead()) || lookahead() == '_') {
    buffer();
  }

  Token* tok = (*cxt).checkKeywords(buff);

  if (tok)
    return tok;
  else
    tok = (*cxt).insertSymbol(buff);

  return tok;
}

// Decides if the integer value is decimal, binary or hexadecimal.
Token* Lexer::decideType() {
  buffer();
  if (lookahead() == 'x') {
    buff.clear();
    consume();
    return lexeInt('h');
  }
  else if (lookahead() == 'b') {
    buff.clear();
    consume();
    return lexeInt('b');
  }
  else
    return lexeInt('d');
}
#endif

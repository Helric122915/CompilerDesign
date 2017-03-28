#ifndef DECL_HPP
#define DECL_HPP

#include "symbol.hpp"
#include <vector>

class Type;
class Expr;
//class Stmt;

class Decl {
public:
  const Type *type;

  Decl() {}
  virtual ~Decl() = default;
};

class Var_Decl : public Decl {
private:
  const std::string name;

public:
  Expr* init;
  Type* t;

  Var_Decl(const std::string name) : name(name), init(nullptr) {}
  ~Var_Decl() = default;

  const std::string getName() { return name; }   
  const Type* getType() { return type; }
};
#endif

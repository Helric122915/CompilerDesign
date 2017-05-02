#ifndef DECL_HPP
#define DECL_HPP

#include <vector>
#include "stmt.hpp"

class Type;
class Expr;
class Stmt;
class Name_Decl;
class Var_Decl;
class Fn_Decl;
class Param_Decl;
class Ret_Decl;
class Local_Var_Decl;

class Decl {
public:
  class Visitor;

  Decl() {}
  virtual ~Decl() = default;

  //const Type *type;
  //const Type* getType() { return type; }
  Type* type;
  Type* getType() { return type; }
  virtual void accept(Visitor&) = 0;
};

class Decl::Visitor {
public:
  virtual ~Visitor() = default;
  virtual void visit(Name_Decl*) = 0;
  virtual void visit(Var_Decl*) = 0;
  virtual void visit(Fn_Decl*) = 0;
  virtual void visit(Param_Decl*) = 0;
  virtual void visit(Ret_Decl*) = 0;
  virtual void visit(Local_Var_Decl*) = 0;
};

class Name_Decl : public Decl {
private:
public:
  Name_Decl(std::string name) : name(name) {}

  const std::string name;
  const std::string getName() { return name; }   
  void accept(Visitor& v) { v.visit(this); }
};

class Var_Decl : public Name_Decl {
private:
public:
  Expr* init;

  Var_Decl(const std::string name) : Name_Decl(name), init(nullptr) { this->type = nullptr;}
  ~Var_Decl() = default;
  void accept(Visitor& v) { v.visit(this); }
};

class Fn_Decl : public Name_Decl {
private:
public:
  Fn_Decl(std::string name, std::vector<Decl*>&& decls, Decl* r) : Name_Decl(name), params(std::move(decls)), ret(r), body() { }
  Fn_Decl(std::string name, std::vector<Decl*>&& decls, Decl* r, Stmt* b) : Name_Decl(name), params(std::move(decls)), ret(r), body(b) { }

  std::vector<Decl*> params;
  Decl* ret;
  Stmt* body;

  void accept(Visitor& v) { v.visit(this); }
};

class Param_Decl : public Var_Decl {
private:
public:
  using Var_Decl::Var_Decl;

  int index = -1;

  void accept(Visitor& v) { v.visit(this); }
};

class Ret_Decl : public Var_Decl {
private:
public:
  using Var_Decl::Var_Decl;

  void accept(Visitor& v) { v.visit(this); }
};

class Local_Var_Decl : public Var_Decl {
private:
public:
  using Var_Decl::Var_Decl;

  void accept(Visitor& v) { v.visit(this); }
};
#endif

#ifndef DECL_HPP
#define DECL_HPP

#include <vector>
#include "stmt.hpp"

class Type;
class Expr;
class Stmt;
class Decl;
class Name_Decl;
class Var_Decl;
class Fn_Decl;
class Param_Decl;
class Ret_Decl;
class Local_Var_Decl;
class Program_Decl;

class DeclContext {
private:
public:
  DeclContext() : sema(), lex() {}
  DeclContext(Decl* d) : sema(d), lex(d) {}
  DeclContext(Decl* s, Decl* l) : sema(s), lex(l) {}

  Decl* sema;
  Decl* lex;
};

class Decl {
public:
  class Visitor;

  Decl() {}
  virtual ~Decl() = default;

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
  virtual void visit(Program_Decl*) = 0;
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
  DeclContext dc;

public:
  Expr* init;

  Var_Decl(const std::string name, DeclContext dc) : Name_Decl(name), dc(dc), init(nullptr) { this->type = nullptr;}
  ~Var_Decl() = default;
  void accept(Visitor& v) { v.visit(this); }

  DeclContext getDC() { return dc; }
};

class Fn_Decl : public Name_Decl {
private:
  DeclContext dc;

public:
  Fn_Decl(std::string name, std::vector<Decl*>&& decls, Decl* r, DeclContext dc, Type* t) : Name_Decl(name), params(std::move(decls)), ret(r), body(), dc(dc) { this->type = t; }
  Fn_Decl(std::string name, std::vector<Decl*>&& decls, Decl* r, Stmt* b, DeclContext dc, Type* t) : Name_Decl(name), params(std::move(decls)), ret(r), body(b), dc(dc) { this->type = t; }

  std::vector<Decl*> params;
  Decl* ret;
  Stmt* body;

  void accept(Visitor& v) { v.visit(this); }

  DeclContext getDC() { return dc; }
};

class Param_Decl : public Var_Decl {
private:
public:
  Param_Decl(const std::string name, Type* t, DeclContext dc) : Var_Decl(name, dc) { this->type = t; }

  int index = -1;

  void accept(Visitor& v) { v.visit(this); }
};

class Ret_Decl : public Var_Decl {
private:
public:
  Ret_Decl(const std::string name, Type* t, DeclContext dc) : Var_Decl(name, dc) { this->type = t; }

  void accept(Visitor& v) { v.visit(this); }
};

class Local_Var_Decl : public Var_Decl {
private:
public:
  Local_Var_Decl(const std::string name, Type* t, DeclContext dc) : Var_Decl(name, dc) { this->type = t; }

  void accept(Visitor& v) { v.visit(this); }
};

class Program_Decl : public Decl {
private:
public:
  std::vector<Decl*> decls;

  void accept(Visitor& v) { v.visit(this); }
};
#endif

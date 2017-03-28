#ifndef STMT_HPP
#define STMT_HPP

class Expr;
class Decl;

class Stmt {
public:
  virtual ~Stmt() = default;

  virtual Expr* getExpr() = 0;
};

class Expr_Stmt : public Stmt {
private:
  Expr* e;

public:
  Expr_Stmt(Expr* e) : e(e) {}

  Expr* getExpr() { return e; }
};

class Decl_Stmt : public Stmt {
private:
  Decl* d;

public:
  Decl_Stmt(Decl* d) : d(d) {}

  Decl* getDecl() { return d; }
  Expr* getExpr() { return dynamic_cast<Var_Decl*>(d)->init; }
};
#endif

#ifndef STMT_HPP
#define STMT_HPP

#include <vector>

class Expr;
class Decl;
class Expr_Stmt;
class Decl_Stmt;
class Block_Stmt;
class If_Stmt;
class While_Stmt;
class Break_Stmt;
class Continue_Stmt;
class Return_Stmt;


class Stmt {
public:
  class Visitor;

  virtual ~Stmt() = default;

  virtual void accept(Visitor&) = 0;
};

class Stmt::Visitor {
public:
  virtual ~Visitor() = default;
  virtual void visit(Expr_Stmt*) = 0;
  virtual void visit(Decl_Stmt*) = 0;
  virtual void visit(Block_Stmt*) = 0;
  virtual void visit(If_Stmt*) = 0;
  virtual void visit(While_Stmt*) = 0;
  virtual void visit(Break_Stmt*) = 0;
  virtual void visit(Continue_Stmt*) = 0;
  virtual void visit(Return_Stmt*) = 0;
};

class Expr_Stmt : public Stmt {
private:
  Expr* e;

public:
  Expr_Stmt(Expr* e) : e(e) {}

  Expr* getExpr() { return e; }
  void accept(Visitor& v) { v.visit(this); }
};


class Decl_Stmt : public Stmt {
private:
  Decl* d;

public:
  Decl_Stmt(Decl* d) : d(d) {}

  Decl* getDecl() { return d; }

  void accept(Visitor& v) { v.visit(this); }
};

class Block_Stmt : public Stmt {
private:
  std::vector<Stmt*> stmts;

public:
  Block_Stmt(std::vector<Stmt*>&& stmtSeq) : stmts(std::move(stmtSeq)) {}

  std::vector<Stmt*> Get_Statements() { return stmts; }

  void accept(Visitor& v) { v.visit(this); }
};

class If_Stmt : public Stmt {
private:
  Expr* Condition;
  Stmt* TrueBlock;
  Stmt* FalseBlock;

public:
  If_Stmt(Expr* e, Stmt* trueBlock, Stmt* falseBlock) : Condition(e), TrueBlock(trueBlock), FalseBlock(falseBlock) {}

  Expr* Get_Condition() { return Condition; }
  Stmt* GetTrueBlock() { return TrueBlock; }
  Stmt* GetFalseBlock() { return FalseBlock; }

  void accept(Visitor& v) { v.visit(this); }
};

class While_Stmt : public Stmt {
private:
  Expr* Condition;

public:
  While_Stmt(Expr* e, Stmt* s) : Condition(e), Body(s) {}

  Expr* Get_Condition() { return Condition; }

  Stmt* Body;

  void accept(Visitor& v) { v.visit(this); }
};

class Break_Stmt : public Stmt {
  void accept(Visitor& v) { v.visit(this); }
};

class Continue_Stmt : public Stmt {
  void accept(Visitor& v) { v.visit(this); }
};

class Return_Stmt : public Stmt {
private:
  Expr* Return;

public:
  Return_Stmt(Expr* e) : Return(e) {}

  Expr* Get_Return() { return Return; }

  void accept(Visitor& v) { v.visit(this); }
};
#endif

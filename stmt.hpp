#ifndef STMT_HPP
#define STMT_HPP

#include <vector>

class Expr;
class Decl;

class Stmt {
public:
  virtual ~Stmt() = default;

  // temporarily not pure virtual / also will probably remove
  virtual Expr* getExpr() {};
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

class Block_Stmt : public Stmt {
private:
  std::vector<Stmt*> stmts;

public:
  Block_Stmt(std::vector<Stmt*>&& stmtSeq) : stmts(std::move(stmtSeq)) {}

  std::vector<Stmt*> Get_Statements() { return stmts; }
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
};

class While_Stmt : public Stmt {
private:
public:
  While_Stmt(Expr* e, Stmt* s) : Condition(e), Body(s) {}

  Expr* Get_Conditoin() { return Condition; }

  Expr* Condition;
  Stmt* Body;
};

class Break_Stmt : public Stmt {
};

class Continue_Stmt : public Stmt {
};

class Return_Stmt : public Stmt {
private:
  Expr* Return;

public:
  // Only used for void I believe
  //Return_Stmt() = default;
  Return_Stmt(Expr* e) : Return(e) {}

  Expr* Get_Return() { return Return; }
};
#endif

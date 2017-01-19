// Base Class
struct Expr{
  virtual ~Expr(){}
};

struct True_Expr : Expr {};

struct False_Expr : Expr {};

struct Not_Expr : Expr {
  Expr *e;
};

struct And_Expr : Expr {
  Expr *e1;
  Expr *e2;
};

struct Or_Expr : Expr {
  Expr *e1;
  Expr *e2;
};

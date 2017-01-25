#include "expr.h"

int main()
{
  Expr *e = new And_Expr(new Not_Expr(new Bool_Expr(true)), new Bool_Expr(false));

  std::cout << "Weight: " << e->weight() << "\n";

  std::cout << "Height: " << e->height() << "\n";

  std::cout << "Evaluation of Tree: " << (e->eval()?"true\n":"false\n");

  Expr *ex = new Cond_Expr(new Not_Expr(new Bool_Expr(false)), new Or_Expr(new Bool_Expr(true),new Bool_Expr(false)), new Bool_Expr(false));

  std::cout << "Weight: " << ex->weight() << "\n";
  std::cout << "Height: " << ex->height() << "\n";
  std::cout << "Evaluation of Tree: " << (ex->eval()?"true\n":"false\n");
}

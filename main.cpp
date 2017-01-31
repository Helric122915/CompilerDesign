#include "bool_expr.h"
#include "int_expr.h"

int main()
{
  try {
    Expr *e = new Mult_Expr(new Int_Expr(10),new Int_Expr(15));
    //Expr *e = new Or_Expr(new Bool_Expr(true),new Bool_Expr(false));

    std::cout << "Weight: " << e->weight() << "\n";
    std::cout << "Height: " << e->height() << "\n";
    std::cout << "Evaluation of Tree: " << e->eval() << "\n";
    //std::cout << "Evaluation of Tree: " << (e->eval()?"true\n":"false\n");
  }
  catch (Type_Exception e) {
    std::cout << e.message() << "\n";
  }

  //Expr *e = new And_Expr(new Not_Expr(new Bool_Expr(true)), new Bool_Expr(false));

  //std::cout << "Weight: " << e->weight() << "\n";

  //std::cout << "Height: " << e->height() << "\n";

  //std::cout << "Evaluation of Tree: " << (e->eval()?"true\n":"false\n");

  /*
  Expr *ex = new Cond_Expr(new Not_Expr(new Bool_Expr(false)), new Or_Expr(new Bool_Expr(true),new Bool_Expr(false)), new Bool_Expr(false));

  std::cout << "Weight: " << ex->weight() << "\n";
  std::cout << "Height: " << ex->height() << "\n";
  std::cout << "Evaluation of Tree: " << (ex->eval()?"true\n":"false\n");
  */
}

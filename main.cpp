#include "bool_expr.h"
#include "int_expr.h"
#include "context.h"
#include <sstream>
#include <string>
#include "eval.h"
#include "weight.h"
#include "height.h"
#include "check.h"

int main(int argc, char *argv[])
{
  int value1 = 10, value2 = 15;

  if (argc > 2) {
    std::stringstream convert1(argv[1]);

    if (!(convert1 >> value1))
      value1 = 0;

    std::stringstream convert2(argv[2]);

    if (!(convert2 >> value2))
      value2 = 0;
  }

  std::cout << value1 << "\n";
  std::cout << value2 << "\n";

  ASTcontext cxt;

  try {
    Expr *e = new Mult_Expr(new Int_Expr(value1,cxt),new Int_Expr(value2,cxt),cxt);
    //Expr *e = new Or_Expr(new Bool_Expr(true),new Bool_Expr(false));

    std::cout << "Type: " << check(e, cxt) << "\n";
    std::cout << "Weight: " << weight(e) << "\n";
    std::cout << "Height: " << height(e) << "\n";
    std::cout << "Evaluation of Tree: " << eval(e) << "\n";
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

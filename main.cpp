#include "bool_expr.h"
#include "int_expr.h"
#include "context.h"
#include <sstream>
#include <string>
#include "eval.h"
#include "weight.h"
#include "height.h"
#include "check.h"
#include "print.h"

void Evaluate_Int(Expr*, ASTcontext&);
void Evaluate_Bool(Expr*, ASTcontext&);

int main(int argc, char *argv[])
{
  ASTcontext cxt;
  int value1 = 10, value2 = 15;

  if (argc > 2) {
    std::stringstream convert1(argv[1]);

    if (!(convert1 >> value1))
      value1 = 0;

    std::stringstream convert2(argv[2]);

    if (!(convert2 >> value2))
      value2 = 0;
  }

  //std::cout << value1 << "\n";
  //std::cout << value2 << "\n";

  try {
    std::cout << "Testing And_Expr\n";
    Expr *andE = new And_Expr(new Bool_Expr(true,cxt),new Bool_Expr(true,cxt),cxt);
    Evaluate_Bool(andE,cxt);

    std::cout << "Testing Or_Expr\n";
    Expr *orE = new Or_Expr(new Bool_Expr(false,cxt),new Bool_Expr(true,cxt),cxt);
    Evaluate_Bool(orE,cxt);

    std::cout << "Testing Xor_Expr\n";
    Expr *xorE = new Xor_Expr(new Bool_Expr(true,cxt),new Bool_Expr(true,cxt),cxt);
    Evaluate_Bool(xorE,cxt);

    std::cout << "Testing Eq_Expr\n";
    Expr *eqE = new Eq_Expr(new Bool_Expr(false,cxt),new Bool_Expr(true,cxt),cxt);
    Evaluate_Bool(eqE,cxt);

    std::cout<< "Testing NotEq_Expr\n";
    Expr *noteqE = new NotEq_Expr(new Bool_Expr(true,cxt),new Bool_Expr(true,cxt),cxt);
    Evaluate_Bool(noteqE,cxt);

    std::cout << "Testing AndThen_Expr\n";
    Expr *andthenE = new AndThen_Expr(new Bool_Expr(false,cxt),new Bool_Expr(true,cxt),cxt);
    Evaluate_Bool(andthenE,cxt);

    std::cout << "Testing OrElse_Expr\n";
    Expr *orelseE = new OrElse_Expr(new Bool_Expr(true,cxt),new Bool_Expr(false,cxt),cxt);
    Evaluate_Bool(orelseE,cxt);

    std::cout << "Testing Add_Expr\n";
    Expr *addE = new Add_Expr(new Int_Expr(10,cxt),new Int_Expr(57,cxt),cxt);
    Evaluate_Int(addE,cxt);

    std::cout << "Testing Sub_Expr\n";
    Expr *subE = new Sub_Expr(new Int_Expr(37,cxt),new Int_Expr(15,cxt),cxt);
    Evaluate_Int(subE,cxt);

    std::cout << "Testing Mult_Expr\n";
    Expr *multE = new Mult_Expr(new Int_Expr(15,cxt),new Int_Expr(17,cxt),cxt);
    Evaluate_Int(multE,cxt);

    std::cout << "Testing Div_Expr\n";
    Expr *divE = new Div_Expr(new Int_Expr(17,cxt),new Int_Expr(5,cxt),cxt);
    Evaluate_Int(divE,cxt);

    std::cout << "Testing Mod_Expr\n";
    Expr *modE = new Mod_Expr(new Int_Expr(25,cxt),new Int_Expr(4,cxt),cxt);
    Evaluate_Int(modE,cxt);

    std::cout << "Testing LessThan_Expr\n";
    Expr *lessthanE = new LessThan_Expr(new Int_Expr(17,cxt),new Int_Expr(15,cxt),cxt);
    Evaluate_Bool(lessthanE,cxt);

    std::cout << "Testing GreaterThan_Expr\n";
    Expr *greaterthanE = new GreaterThan_Expr(new Int_Expr(27,cxt),new Int_Expr(15,cxt),cxt);
    Evaluate_Bool(greaterthanE,cxt);

    std::cout << "Testing LessEqThan_Expr\n";
    Expr *lesseqthanE = new LessEqThan_Expr(new Int_Expr(15,cxt),new Int_Expr(15,cxt),cxt);
    Evaluate_Bool(lesseqthanE,cxt);

    std::cout << "Testing GreaterEqThan_Expr\n";
    Expr *greatereqthanE = new GreaterEqThan_Expr(new Int_Expr(17,cxt),new Int_Expr(22,cxt),cxt);
    Evaluate_Bool(greatereqthanE,cxt);

    std::cout << "Testing Negation_Expr\n";
    Expr *negationE = new Negation_Expr(new Int_Expr(15,cxt),cxt);
    Evaluate_Int(negationE,cxt);
  }
  catch (Type_Exception e) {
    std::cout << e.message() << "\n";
  }
}

void Evaluate_Int(Expr* e, ASTcontext &cxt) {
  print(e);
  std::cout << "\n";
  std::cout << "Type Check: " << (cxt.Int_ == check(e, cxt)?"Match\n":"Don't Match\n");
  std::cout << "Weight: " << weight(e) << "\n";
  std::cout << "Height: " << height(e) << "\n";
  std::cout << "Evaluation of Tree: " << eval(e) << "\n";
  std::cout << "\n";
}

void Evaluate_Bool(Expr* e, ASTcontext &cxt) {
  print(e);
  std::cout << "\n";
  std::cout << "Type Check: " << (cxt.Bool_ == check(e, cxt)?"Match\n":"Don't Match\n");
  std::cout << "Weight: " << weight(e) << "\n";
  std::cout << "Height: " << height(e) << "\n";
  std::cout << "Evaluation of Tree: " << (eval(e)?"True\n":"False\n");
  std::cout << "\n";
}

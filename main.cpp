#include "eval.hpp"
#include "weight.hpp"
#include "height.hpp"
#include "check.hpp"
#include "print.hpp"
#include "lexer.hpp"

void Evaluate_Int(Expr*, ASTcontext&);
void Evaluate_Bool(Expr*, ASTcontext&);

int main(int argc, char *argv[])
{
  ASTcontext cxt;
  std::string input, inputType, outputType;
  
  if (argv[1])
    inputType = argv[1];
  if (argv[2])
    outputType = argv[2]; 
  
  if (inputType == "-b")
    inputType = "2";
  else if (inputType == "-h")
    inputType = "16";
  else
    inputType = "10";

  if (outputType == "-b")
    outputType = "2";
  else if (outputType == "-h")
    outputType = "16";
  else
    outputType = "10";

  try {
    while (getline(std::cin, input)) {
      if (input[0] != '#') {
	std::cout << "Lexing Line: " << input << '\n';

	Lexer *lexe = new Lexer(input.begin(),input.end(),stoi(inputType),stoi(outputType));

	Token *tok = lexe->next();

	while(tok) {
	  tok->print();

	  tok = lexe->next();

          if (tok)
	    std::cout << ", ";
	}

	std::cout << '\n';

	delete lexe;
      }
      else
	std::cout << "Comment: " << input.substr(1) << '\n';;
    }
  }
  catch (Token_Exception e) {
    std::cout << e.message() << '\n';
  }

  /*
  try {
    std::cout << "Testing Large Expression\n";
    Expr *largeE = new Mult_Expr(new Div_Expr(new Int_Expr(25,cxt),new Int_Expr(2,cxt),cxt),new Mult_Expr(new Sub_Expr(new Mod_Expr(new Int_Expr(35,cxt),new Int_Expr(4,cxt),cxt),new Int_Expr(5,cxt),cxt),new Add_Expr(new Int_Expr(15,cxt),new Int_Expr(6,cxt),cxt),cxt),cxt);
    Evaluate_Int(largeE,cxt);

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
    Expr *andthenE = new AndThen_Expr(new LessThan_Expr(new Int_Expr(15,cxt),new Int_Expr(17,cxt),cxt),new GreaterEqThan_Expr(new Int_Expr(2,cxt),new Int_Expr(5,cxt),cxt),cxt);
    Evaluate_Bool(andthenE,cxt);

    std::cout << "Testing OrElse_Expr\n";
    Expr *orelseE = new OrElse_Expr(new Eq_Expr(new Int_Expr(-12,cxt),new Negation_Expr(new Int_Expr(12,cxt),cxt),cxt),new NotEq_Expr(new Int_Expr(5,cxt),new Int_Expr(7,cxt),cxt),cxt);
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

    std::cout << "Testing Eq_Expr with Int\n";
    Expr *eqInt = new Eq_Expr(new Int_Expr(35,cxt),new Int_Expr(35,cxt),cxt);
    Evaluate_Bool(eqInt,cxt);

    std::cout << "Testing NotEq_Expr with Int\n";
    Expr *noteqInt = new NotEq_Expr(new Int_Expr(37,cxt),new Int_Expr(37,cxt),cxt);
    Evaluate_Bool(noteqInt,cxt);

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
  catch (Overflow_Exception e) {
    std::cout << e.message() << "\n";
  }

  try {
    std::cout << "Testing Type_Exception\n";
    Expr *andE = new And_Expr(new Int_Expr(25,cxt),new Bool_Expr(true,cxt),cxt);
    Evaluate_Bool(andE,cxt);
  }
  catch (Type_Exception e) {
    std::cout << e.message() << "\n";
  }

  try {
    std::cout << "Testing Overflow_Exception\n";
    Expr *addE = new Div_Expr(new Int_Expr(std::numeric_limits<int>::min(),cxt),new Int_Expr(-1,cxt),cxt);
    Evaluate_Int(addE,cxt);
  }
  catch (Overflow_Exception e) {
    std::cout << e.message() << "\n";
  }
  */
  return 0;
}

void Evaluate_Int(Expr* e, ASTcontext &cxt) {
  print(e);
  std::cout << "\nType Check: " << (cxt.Int_ == check(e, cxt)?"Match\n":"Don't Match\n");
  std::cout << "Weight: " << weight(e) << "\n";
  std::cout << "Height: " << height(e) << "\n";
  std::cout << "Evaluation of Tree: " << eval(e) << "\n\n";
}

void Evaluate_Bool(Expr* e, ASTcontext &cxt) {
  print(e);
  std::cout << "\nType Check: " << (cxt.Bool_ == check(e, cxt)?"Match\n":"Don't Match\n");
  std::cout << "Weight: " << weight(e) << "\n";
  std::cout << "Height: " << height(e) << "\n";
  std::cout << "Evaluation of Tree: " << (eval(e)?"True\n\n":"False\n\n");
}

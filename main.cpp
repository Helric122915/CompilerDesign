//#include "eval.hpp"
#include "weight.hpp"
#include "height.hpp"
#include "check.hpp"
#include "print.hpp"
#include "lexer.hpp"
#include "parser.hpp"

void Evaluate_Int(Expr*, int, ASTcontext*);
void Evaluate_Bool(Expr*, ASTcontext*);

int main(int argc, char *argv[])
{
  ASTcontext* cxt = new ASTcontext();
  std::string input, outputType;
  int outputTypeInt, exitValue;  

  if (argv[1])
    outputType = argv[1];
  
  if (outputType == "-b")
    outputType = "2";
  else if (outputType == "-h")
    outputType = "16";
  else
    outputType = "10";

  outputTypeInt = stoi(outputType);

  while (getline(std::cin, input)) {
    try {
      if (input.begin() != input.end()) {
	if (input[0] != '#') {
	  int commentIndex = input.find('#');
	  std::string comment = input.substr(commentIndex + 1);
	  input = input.substr(0, commentIndex - 1);
	    
	  if (commentIndex != std::string::npos)
	    std::cout << "Comment: " << comment << '\n';
	    
	  //std::cout << "Lexing Line: " << input << '\n';
	    
	  Lexer *lexe = new Lexer(input.begin(),input.end(),outputTypeInt, cxt);

	  std::vector<Token*> tokens;
	    
	  Token *tok = lexe->next();
	    
	  while(tok) {
	    tokens.push_back(tok);
	      
	    tok = lexe->next();
	  }
	  
	  if (tokens.size() > 0) {
	    for (std::vector<Token*>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
	      (*it)->print();
	      
	      if (std::next(it) != tokens.end())
		std::cout << ", ";
	    }
	    std::cout << '\n';

	    Parser *parse = new Parser(tokens, outputTypeInt, cxt);
	    
	    //Stmt* stmt = parse->next();	    

	    Expr* expr = parse->next()->getExpr();

	    if (expr->getType() == (*cxt).Int_)
	      Evaluate_Int(expr,stoi(outputType),cxt);
	    
	    else if (expr->getType() == (*cxt).Bool_)
	      Evaluate_Bool(expr,cxt);
	    
	    exitValue = eval(expr);

	    delete parse;
	  }
	  else
	    std::cout << "There were no tokens to parse.\n";
	  
	  delete lexe;
	}
	else
	  std::cout << "Comment: " << input.substr(1) << '\n';
      }
      else
	std::cout << "Nothing to Lex.\n";
    }
    catch (Token_Exception e) {
      std::cout << e.message() << '\n';
    }
    catch (Syntax_Exception e) {
      std::cout << e.message() << '\n';
    }
    catch (Type_Exception e) {
      std::cout << e.message() << "\n";
    }
    catch (Overflow_Exception e) {
      std::cout << e.message() << "\n";
    }
    catch (Semantic_Exception e) {
      std::cout << e.message() << "\n";
    }
  }

  return exitValue;
}

void Evaluate_Int(Expr* e, int outputType, ASTcontext* cxt) {
  std::cout << "Pretty Printer: ";
  print(e);
  std::cout << '\n';
  //std::cout << "\nType Check: " << ((*cxt).Int_ == check(e, cxt)?"Match\n":"Don't Match\n");
  //std::cout << "Weight: " << weight(e) << "\n";
  //std::cout << "Height: " << height(e) << "\n";
  std::cout << "Evaluation of Tree: ";
  printInt(eval(e), outputType);
  std::cout << "\n\n";
}

void Evaluate_Bool(Expr* e, ASTcontext* cxt) {
  std::cout << "Pretty Printer: ";
  print(e);
  std::cout << '\n';
  //std::cout << "\nType Check: " << ((*cxt).Bool_ == check(e, cxt)?"Match\n":"Don't Match\n");
  //std::cout << "Weight: " << weight(e) << "\n";
  //std::cout << "Height: " << height(e) << "\n";
  std::cout << "Evaluation of Tree: " << (eval(e)?"true\n\n":"false\n\n");
}

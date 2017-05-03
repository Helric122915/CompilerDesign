//#include "eval.hpp"
#include "weight.hpp"
#include "height.hpp"
#include "check.hpp"
#include "print.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "evaluator.hpp"

//void Evaluate_Int(Expr*, int, ASTcontext*);
//void Evaluate_Bool(Expr*, ASTcontext*);

int main(int argc, char *argv[])
{
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

  ASTcontext* cxt = new ASTcontext(outputTypeInt);
  Evaluator ev(cxt);

  //Stack_Frame main_frame(ev);

  std::string file;

  while (getline(std::cin, input)) {
    if (input.begin() != input.end()) {
      if (input[0] != '#') {
	int commentIndex = input.find('#');
	std::string comment = input.substr(commentIndex + 1);
	input = input.substr(0, commentIndex - 1);

	if (commentIndex != std::string::npos)
	  std::cout << "Comment: " << comment << '\n';
	file = file + input;
      }
    }
  }
   
  try {
    Lexer *lexe = new Lexer(file.begin(),file.end(), cxt);
    
    std::vector<Token*> tokens;
    
    std::cout << "Lexing Tokens\n";
    Token *tok = lexe->next();
    
    while(tok) {
      tokens.push_back(tok);
      
      tok = lexe->next();
    }
    
    if (tokens.size() > 0) {
      /*for (std::vector<Token*>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
	(*it)->print();
	
	if (std::next(it) != tokens.end())
	  std::cout << ", ";
      }
      std::cout << '\n';
      */
      Parser *parse = new Parser(tokens, outputTypeInt, cxt);

      std::cout << "Parsing Program\n";      
      Decl* ast = parse->translate();

      Program_Decl* program = static_cast<Program_Decl*>(ast);
      Fn_Decl* entry = nullptr;

      for (Decl* d : program->decls) {
	if (Name_Decl* name = dynamic_cast<Name_Decl*>(d)) {
	  if (name->getName() == *(cxt->insert("main"))) {
	      entry = dynamic_cast<Fn_Decl*>(name);
	      break;
	  }
	}
      }
	
      if (entry) {
	Expr* ref = parse->sema->id_expression(entry);
	Expr* call = parse->sema->call_expression(ref, {});

	Value v = eval(ev, call);
	v.print_value();
      }
      else {
	std::cout << "Y'all messed up\n";
      }

      delete parse;
      delete program;
      delete entry;
    }
    else
      std::cout << "There were no tokens to parse.\n";
    
    delete lexe;
  }
  catch (Token_Exception e) {
    std::cout << e.message() << '\n';
    }
  catch (Syntax_Exception e) {
    std::cout << e.message() << '\n';
  }
  catch (Type_Exception e) {
    std::cout << e.message() << '\n';
  }
  catch (Overflow_Exception e) {
    std::cout << e.message() << '\n';
  }
  catch (Semantic_Exception e) {
    std::cout << e.message() << '\n';
  }
  catch (Frame_Exception e) {
    std::cout << e.message() << '\n';
  }
  return exitValue;
}

/*
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
*/

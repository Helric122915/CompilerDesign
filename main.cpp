//#include "eval.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "generator.hpp"
#include "genDecl.hpp"
#include "genStmt.hpp"
#include "genExpr.hpp"

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

	//if (commentIndex != std::string::npos)
	  //std::cout << "Comment: " << comment << '\n';
	file = file + input;
      }
    }
  }
   
  try {
    Lexer *lexe = new Lexer(file.begin(),file.end(), cxt);
    
    std::vector<Token*> tokens;
    
    //std::cout << "Lexing Tokens\n";
    Token *tok = lexe->next();
    
    while(tok) {
      tokens.push_back(tok);
      
      tok = lexe->next();
    }
    
    if (tokens.size() > 0) {
       Parser *parse = new Parser(tokens, outputTypeInt, cxt);

       //std::cout << "Parsing Program\n";      
      Decl* ast = parse->translate();

      Program_Decl* program = static_cast<Program_Decl*>(ast);

      Generator gen(cxt);
      gen_decl(gen, program);

      delete parse;
      delete program;
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
  catch (Generator_Exception e) {
    std::cout << e.message() << '\n';
  }
}

#include <iostream>
#include <cstdlib>
#include <ctime>

#include "BST.h"
#include "expr.h"

int main()
{
  //srand(time(0)); // seed

  // create templated BST
  //BST<int>* bst = new BST<int>();

  //BST<Expr>* bst = new BST<Expr>();

  std::cout << "Start Code\n";

  True_Expr *True = new True_Expr();
  False_Expr *False = new False_Expr();

  std::cout << "Checkpoint A\n";

  And_Expr *e1 = new And_Expr();
  e1->e1 = True;
  e1->e2 = False;

  std::cout << "Checkpoint B\n";

  Not_Expr *e2 = new Not_Expr();
  e2->e = True;

  std::cout << "Checkpoint C\n";

  Or_Expr *e = new Or_Expr();
  e->e1 = e1;
  e->e2 = e2;

  std::cout << "Made it through code\n";
  /*
  bst->add();


  // fill it with nodes
  for (int i = 0; i < 20; ++i)
    bst->add((rand() % 900) + 100);

  // display number of leaves
  std::cout << bst->cLeaves() << " leaves!" << std::endl;

  // display number of total nodes
  std::cout << bst->numNodes() <<" total nodes!" <<std::endl;

  // display number of inner nodes
  std::cout << bst->innerNodes() <<" inner nodes!" <<std::endl;

  std::cout << "The tree does " 
	    << (!bst->search(300) ? "not " : "")
	    << "contain 300." << std::endl;

  // display the number of not odd nodes
  std::cout << "The number of not odd nodes is "
	    << bst->notOdds() <<std::endl;

  bst->treeDisplay();

  bst->display(); // call the display kick starter
  std::cout << std::endl;
  */
}

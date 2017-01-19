#include <iostream>
#include <cstdlib>
#include <ctime>

#include "BST.h"

int main()
{
  srand(time(0)); // seed

  // create templated BST
  BST<int>* bst = new BST<int>();


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

}

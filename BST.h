#ifndef BST_H
#define BST_H

#include <iostream>
#include <string>

#include "Node.h"

template <class T>
class BST
{
 public:
  Node<T>* root;
  
  BST(); //constructor
  ~BST();

  void add(T);
  void add(Node<T>*);
  void add(Node<T>*, Node<T>*);  // this is what actually adds the node

  int cLeaves(); // count the number of leaves
  int cLeaves(Node<T>*);

  // number of total nodes
  int numNodes();
  int numNodes(Node<T>*);

  // number of inner nodes
  int innerNodes();
  int innerNodes(Node<T>*);
  
  // determine if a node in the tree contains a given integer
  Node<T>* search(T);
  Node<T>* search(T, Node<T>*);

  // number of even numbers in the data
  int notOdds();
  int notOdds(Node<T>*);

  // display the tree like a tree! maybe
  void treeDisplay();
  void treeDisplay(Node<T>*, int);

  void display();
  void display(Node<T>*);
};

// constructor
template <class T>
BST<T>::BST()
{
  root = 0;
}

// destructor
template <class T>
BST<T>::~BST()
{
  delete root;
}

// add a new Node using the OTHER add
template <class T>
void BST<T>::add(T data)
{
  add(new Node<T>(data));
}

template <class T>
void BST<T>::add(Node<T>* n)
{
  if (!root)
    root = n;
  else
    add(n, root); // add to the tree, starting with root
}

template <class T>
void BST<T>::add(Node<T>* n, Node<T>* subRoot)
{
  if (n->data < subRoot->data)
    {
      if (!subRoot->left)
	subRoot->left = n;
      else
	add(n, subRoot->left);
    }
  else
    {
      if (!subRoot->right)
	subRoot->right = n;
      else
	add(n, subRoot->right);
    }
}

// display recursion kick start
template <class T>
void BST<T>::display()
{
  display(root);
}

template <class T>
void BST<T>::display(Node<T>* subRoot)
{
  if (subRoot)
    {
      display(subRoot->left); // display all left children
      std::cout << "[" << subRoot->data << "]"; // display myself
      display(subRoot->right); // display all right children
    }
}

template <class T>
int BST<T>::cLeaves()
{
  return cLeaves(root); // calls cLeaves with the root
}

template <class T>
int BST<T>::cLeaves(Node<T>* subRoot)
{
  if (!subRoot) // if the given node is null
    return 0;

  if (!subRoot->left && !subRoot->right) // if it has no children
    return 1;

  return cLeaves(subRoot->left) + cLeaves(subRoot->right); // has children
}

template <class T>
int BST<T>::numNodes()
{
  return numNodes(root); // same reasoning
}

template <class T>
int BST<T>::numNodes(Node<T>* subRoot)
{
  if (!subRoot)
    return 0;
  
  return 1 + numNodes(subRoot->left) + numNodes(subRoot->right);
}

template <class T>
int BST<T>::innerNodes()
{
  return innerNodes(root); // yada yada
}

template <class T>
int BST<T>::innerNodes(Node<T>* subRoot)
{
  if (!subRoot || (!subRoot->left && !subRoot->right))
    return 0;

  return 1 + innerNodes(subRoot->left) + innerNodes(subRoot->right);
}

template <class T>
Node<T>* BST<T>::search(T data)
{
  return search(data, root);
}

template <class T>
Node<T>* BST<T>::search(T data, Node<T>* subRoot)
{
  if (!subRoot)
    return 0;

  if (subRoot->data == data)
    return subRoot;

  if (data < subRoot->data)
    return search(data, subRoot->left);

  return search(data, subRoot->right);
} 

template <class T>
int BST<T>::notOdds()
{
  return notOdds(root);
}

template <class T>
int BST<T>::notOdds(Node<T>* subRoot)
{
  if (!subRoot)
    return 0;

  return notOdds(subRoot->left)
    + notOdds(subRoot->right)
    + (subRoot->data % 2 ? 0 : 1);
}

// call on root
template <class T>
void BST<T>::treeDisplay()
{
  treeDisplay(root, 0);
}

template <class T>
void BST<T>::treeDisplay(Node<T>* subRoot, int depth)
{
  if (!subRoot)
    return;

  std::string spaces ="";
  for (int i = 0; i < (depth * 5); ++i)
    spaces += " ";

  // display the right side of the tree
    treeDisplay(subRoot->right, depth + 1);
  // display myself
  std::cout << spaces << "[" << subRoot->data << "]\n";
  // display the left side of the tree
  treeDisplay(subRoot->left, depth + 1);
}
#endif

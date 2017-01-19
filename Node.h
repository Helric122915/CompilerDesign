#ifndef NODE_H
#define NODE_H

template <class T>
class Node
{
 public:
  T data; // the data
  Node<T>* left;
  Node<T>* right;

  Node(T data);
  ~Node();
};

template <class T>
Node<T>::Node(T data)
{
  this->data = data; // set the data
  left = right = 0; // set the children to null
}

template <class T>
Node<T>::~Node()
{
  // delete all children on the left, recursively
  if (left)
    delete left;

  // delete all children on the right, recursively
  if (right)
    delete right;
}

#endif

#ifndef TYPE_HPP
#define TYPE_HPP

#include "exception.hpp"
#include <string>
#include <iostream>
#include <algorithm>
#include <list>
#include <limits>
#include <unordered_map>

// Declares the base type to be used for any expression.
class Type {
  public:
    virtual ~Type() = default;
};

// Declares the two specific types Bool and Int to be used for expressions.
class Bool_Type : public Type {};
class Int_Type : public Type {};
// Not sure how to use
class Ref_Type : public Type {
public:
  Ref_Type(Type* t) : obj(t) {}

  Type* obj;
};
#endif

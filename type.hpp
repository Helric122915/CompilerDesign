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

class Ref_Type : public Type {
public:
  Ref_Type(Type* t) : obj(t) {}

  Type* obj;
};

class Fn_Type : public Type {
public:
  Fn_Type(std::vector<Type*>& params, Type* r) : params(params), ret(r) {}
  Fn_Type(std::vector<Type*>&& params, Type* r) : params(std::move(params)), ret(r) {}


  std::vector<Type*> params;
  Type* ret;
};

Type* get_type(Type* t) {
  if (auto* ref = dynamic_cast<Ref_Type*>(t))
    return ref->obj;
  else
    return t;
}
#endif

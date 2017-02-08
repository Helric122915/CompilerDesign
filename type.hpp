#ifndef TYPE_HPP
#define TYPE_HPP

#include "exception.hpp"

// Declares the base type to be used for any expression.
class Type {
  public:
    virtual ~Type() = default;
};

// Declares the two specific types Bool and Int to be used for expressions.
class Bool_Type : public Type {};
class Int_Type : public Type {};
#endif

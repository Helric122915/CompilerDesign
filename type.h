#ifndef TYPE_H
#define TYPE_H

#include <stdexcept>

class Type {
  public:
    virtual ~Type() {}
};

class Bool_Type : public Type {};
class Int_Type : public Type {};

class Type_Exception : public std::exception {
  public:
    virtual const char* message() const throw() {
      return "A Type Error has occured.";
    }
};
#endif

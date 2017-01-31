#ifndef TYPE_H
#define TYPE_H

#include <stdexcept>

struct Type {
  virtual ~Type() {}
};

struct Bool_Type : Type {};
struct Int_Type : Type {};

struct Type_Exception : public std::exception {
  virtual const char* message() const throw() {
    return "A Type Error has occured.";
  }
};
#endif

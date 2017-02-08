#ifndef TYPE_HPP
#define TYPE_HPP

#include <stdexcept>
#include <string>
#include <iostream>
#include <algorithm>
#include <list>

class Type {
  public:
    virtual ~Type() = default;
};

class Bool_Type : public Type {};
class Int_Type : public Type {};

class Type_Exception : public std::exception {
  private:
    std::string type;

  public:
    Type_Exception(std::string type) : type(type) {}
    ~Type_Exception() = default;

    virtual std::string message() const throw() {
      return "A Type Error of Type: " + type;
    }
};
#endif

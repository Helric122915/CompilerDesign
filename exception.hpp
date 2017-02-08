#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <stdexcept>
#include <string>
#include <iostream>
#include <algorithm>
#include <list>

// Declares a specific exception to be thrown when a type error occurs.
// This exception allows for a string to be passed when throwing the type exception.
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

// Declares a specific exception to be thrown when an overflow error occurs.
// This exception allows for a string to be passed when throwing the overflow exception.
class Overflow_Exception : public std::exception {
  private:
    std::string type;

  public:
    Overflow_Exception(std::string type) : type(type) {}
    ~Overflow_Exception() = default;

    virtual std::string message() const throw() {
      return "An Overflow Error of Type: " + type;
    }
};
#endif

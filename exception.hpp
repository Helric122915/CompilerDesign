#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <stdexcept>

class Base_Exception : public std::exception {
public:
  std::string type;

  virtual ~Base_Exception() = default;

  virtual std::string message() const throw() = 0;
};

// Declares a specific exception to be thrown when a type error occurs.
// This exception allows for a string to be passed when throwing the type exception.
class Type_Exception : public Base_Exception {
public:
  Type_Exception(std::string type) { this->type = type; }
  ~Type_Exception() = default;

  virtual std::string message() const throw() {
    return "A Type Error of Type: " + type;
  }
};

// Declares a specific exception to be thrown when an overflow error occurs.
// This exception allows for a string to be passed when throwing the overflow exception.
class Overflow_Exception : public Base_Exception {
public:
  Overflow_Exception(std::string type) { this->type = type; }
  ~Overflow_Exception() = default;

  virtual std::string message() const throw() {
    return "An Overflow Error of Type: " + type;
  }
};

// Declares a specific exception to be thrown when a token error occurs.
// This exception allows for a string to be passed when throwing the overflow exception.
class Token_Exception : public Base_Exception {
public:
  Token_Exception(std::string type) { this->type = type; }
  ~Token_Exception() = default;

  virtual std::string message() const throw() {
    return "A Token Exception Error of Type: " + type;
  }
};
#endif

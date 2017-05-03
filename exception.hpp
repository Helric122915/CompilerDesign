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

// Declares a specific exception to be thrown when a token doesn't match what is expected.
// This exception allows for a string to be passed when throwing the syntax exception.
class Syntax_Exception : public Base_Exception {
public:
  Syntax_Exception(std::string type) { this->type = type; }
  ~Syntax_Exception() = default;

  virtual std::string message() const throw() {
    return "A Syntax Exception Error of Type: " + type;
  }
};

// Declares a specific exception to be thrown when a semantic error occurs.
// This exception allows for a string to be passed when throwing the syntax exception.
class Semantic_Exception : public Base_Exception {
public:
  Semantic_Exception(std::string type) { this->type = type; }
  ~Semantic_Exception() = default;

  virtual std::string message() const throw() {
    return "A Semantic Exception Error of Type: " + type;
  }
};

// Declares a specific exception to be thrown when a frame error occurs.
// This exception allows for a string to be passed when throwing the syntax exception.
class Frame_Exception : public Base_Exception {
public:
  Frame_Exception(std::string type) { this->type = type; }
  ~Frame_Exception() = default;

  virtual std::string message() const throw() {
    return "A Frame Exception Error of Type: " + type;
  }
};

// Declares a specific exception to be thrown when a interpretor error occurs.
// This exception allows for a string to be passed when throwing the syntax exception.
class Interpretor_Exception : public Base_Exception {
public:
  Interpretor_Exception(std::string type) { this->type = type; }
  ~Interpretor_Exception() = default;

  virtual std::string message() const throw() {
    return "A Interpretor Exception Error of Type: " + type;
  }
};
 
// Declares a specific exception to be thrown when a generator error occurs.
// This exception allows for a string to be passed when throwing the syntax exception.
class Generator_Exception : public Base_Exception {
public:
  Generator_Exception(std::string type) { this->type = type; }
  ~Generator_Exception() = default;

  virtual std::string message() const throw() {
    return "A Generator Exception Error of Type: " + type;
  }
};
#endif

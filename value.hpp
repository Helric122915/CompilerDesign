#ifndef VALUE_HPP
#define VALUE_HPP

#include <iosfwd>
#include <list>
#include <vector>

#include <cassert>
#include <cstdint>
#include <cstring>
#include "exception.hpp"

#include "enumeration.hpp"

class Store_Value;

class Uninit_Value { };

union Value_Data {
  Value_Data() : u() { }
  Value_Data(int n) : z(n) { }
  Value_Data(Store_Value* r) : ref(r) { }
  //Value_Data(fn_value f) : fn(f) { }

  int z;  
  Store_Value* ref;
  Uninit_Value u;
  //mapping_decl* fn;

};

class Value {
private:
  int kind;

public:
  Value() : kind(uninit_value), data() { }
  Value(int n) : kind(int_value), data(n) { }
  Value(Store_Value* ref) : kind(ref_value), data(ref) { }
  
  //Value(mapping_decl* fn) : kind(fn_value), data(fn) { }

  bool is_uninit() { return kind == uninit_value; }
  bool is_integer() { return kind == int_value; }
  bool is_ref() { return kind == ref_value; }
  bool is_fn() { return kind == fn_value; }

  int get_int() { return data.z; }
  Store_Value* get_ref() { return data.ref; }
  //mapping_decl* get_fn { return data.fn; }

  void print_value();

  Value& get_value_ref();
  Value_Data data;
};

class Store_Value {
private:
public:
  using iterator = std::list<Store_Value>::iterator;

  template<typename... Args>
  Store_Value(Args&&... args) : position(), val(std::forward<Args>(args)...) { }

  Value& get_value() { return val; }

  iterator position;
  Value val;
};

Value& Value::get_value_ref() {
  if (is_integer())
    std::cout << "I'm int\n";
  else if (is_uninit())
    std::cout << "I'm uninit\n";
  else if (is_fn())
    std::cout << "I'm function?\n";
  else if (is_ref())
    return data.ref->get_value();
  else
    throw Semantic_Exception("Discrim union upset (value)");
}

void Value::print_value() {
  switch(kind) {
  case uninit_value: std::cout << "uninit_value\n"; break;
  case int_value: std::cout << "Int_Value: " << data.z << '\n'; break;
  case ref_value: {
    std::cout << "Ref_Value: " << data.ref << '\n'; 
    if (data.ref)
      std::cout << "Ref_Value: "; data.ref->val.print_value();
    break;
  }
  case fn_value: break;
  };
}
#endif

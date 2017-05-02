#ifndef FRAME_HPP
#define FRAME_HPP

#include <vector>
#include <stack>
#include <unordered_map>
#include "value.hpp"

class Value_Stack;

class Frame : public std::vector<Decl*> {};

class Frame_Stack {
private:
public:
  class Bind {
  private:
  public:
    Bind(Frame* f, Value val) : f(f), val(val) {}

    Frame* get_frame() { return f; }

    Value get_val() { return val; }

    Value& get_ref() { return val.get_value_ref(); }

    Frame* f;
    Value val;
  };

  class Value_Stack : std::vector<Bind> {
  private:
  public:
    Bind& top() { return back(); }

    void push(Frame* f, Value val) { emplace_back(f, val); }

    void pop() { pop_back(); }
  };

  Frame* current_frame() { return frameStack.top(); }
  void enter_frame() { frameStack.emplace(new Frame()); }
  void leave_frame() { frameStack.pop(); }
  Value allocate();
  void deallocate(Value loc);
  Value_Stack& bindings(Name_Decl*); 
  Bind& find(Name_Decl*);
  Value locate(Name_Decl* d);

  void bind(Name_Decl*,Value);

  //std::unordered_map<Decl*, Store_Value*> map;
  std::unordered_map<Name_Decl*, Value_Stack> map;
  std::list<Store_Value> mem;
  std::stack<Frame*> frameStack;
};

Value Frame_Stack::allocate() {
  std::cout << "Something is actually being allocated...wow\n";
  auto iter = mem.emplace(mem.end());
  iter->position = iter;
  return Value(&*iter);
}

void Frame_Stack::deallocate(Value val) {
  Store_Value* store = val.get_ref();
  mem.erase(store->position);
}

Frame_Stack::Value_Stack& Frame_Stack::bindings(Name_Decl* d) {
  auto iter = map.find(d);
  return iter->second;
}

Frame_Stack::Bind& Frame_Stack::find(Name_Decl* d) {
  Value_Stack& val = bindings(d);
  return val.top();
}

Value Frame_Stack::locate(Name_Decl* d) {
  std::cout << "Searching for d: " << d->getName() << '\n';
  Frame_Stack::Bind& top = find(d);
  top.get_val().print_value();
  return top.get_val();
}

void Frame_Stack::bind(Name_Decl* d, Value val) {
  Store_Value* store = val.get_ref();

  Frame* f = current_frame();

  auto result = map.emplace(d, Value_Stack());
  Value_Stack& v = result.first->second;
  v.push(f, val);

  f->push_back(d);
}
#endif

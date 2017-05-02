#ifndef SCOPE_HPP
#define SCOPE_HPP

#include <memory>
#include <vector>
#include "enumeration.hpp"
#include "decl.hpp"

class Scope : public std::vector<Name_Decl*> {
private:
public:
  Scope(int k) : kind(k), parent(nullptr) { }
  Scope(int k, Scope* p) : kind(k), parent(p) { }

  bool addToScope(Name_Decl* d);
  Name_Decl* searchName(const std::string);
  std::string scopeName();

  int kind;
  Scope* parent;
};

bool Scope::addToScope(Name_Decl* d) {
  for (auto& decl : *this)
    if (decl->getName() == d->getName())
      return false;
  push_back(d);
  return true;
};

Name_Decl* Scope::searchName(const std::string name) {
  for (auto& decl : *this)
    if (decl->getName() == name)
      return decl;
  return nullptr;
}

std::string Scope::scopeName() {
  switch(kind) {
  case global_scope: return "Global Scope";
  case function_param_scope: return "Function Param Scope";
  case function_scope: return "Function Scope";
  case block_scope: return "Block Scope";
  }
}

class ScopeStack : public std::vector<Scope*> {
private:
public:

  void push(Scope* s) { push_back(s); };
  void push(int);
  void pop() {  pop_back(); }
  Scope* top() { return back(); }

  Name_Decl* searchName(const std::string);
};

void ScopeStack::push(int scope_kind) {
  Scope* s = empty() ? nullptr : back();
  push_back(new Scope(scope_kind, s));
}

Name_Decl* ScopeStack::searchName(const std::string name) {
  //for (auto& scope : *this)
  for (unsigned scope = (*this).size(); scope-- > 0;) 
    if (Name_Decl* dec = (*this)[scope]->searchName(name))
      return dec;
  return nullptr;
}
#endif

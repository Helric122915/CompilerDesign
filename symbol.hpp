#ifndef SYMBOL_HPP
#define SYMBOL_HPP

#include <string>
#include <unordered_map>

class Symbol_Table {
private:
  std::unordered_map<std::string, void*> symbols;

public:
  const std::string* insert(const std::string&);
  const std::string* find(const std::string&);
};
#endif

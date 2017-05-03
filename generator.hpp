#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <unordered_map>
#include <vector>
#include <iosfwd>
#include <stack>
#include <string>
#include <sstream>
#include <iostream>
#include "type.hpp"

class ASTcontext;
class Type;
class Expr;
class Decl;
class Stmt;

class CodeGen;
class Generator;
class File;
class Method;
class Block;

Generator& get_generator(CodeGen&);
File& get_file_codegen(CodeGen&);
Method& get_method_codegen(CodeGen&);
Block& get_block_codegen(CodeGen&);

class CodeGen {
private:
public:
  CodeGen(ASTcontext* cxt, CodeGen& parent, std::ostream& os) : cxt(cxt), parent(&parent), os(&os) { }
  CodeGen(CodeGen& cg) : cxt(cg.cxt), parent(cg.parent), os(cg.os) { }

  virtual bool is_generator() { return false; }
  virtual bool is_file(){ return false; }
  virtual bool is_method() { return false; }
  virtual bool is_block() { return false; }

  std::ostream& outs() { return *os; }

  Generator& as_generator();
  File& as_file();
  Method& as_method();
  Block& as_block();

  CodeGen& get_parent() { return *parent; }

  ASTcontext* getContext() { return cxt; }
  CodeGen* getParent(){ return parent; }
  std::ostream* getOs() { return os; }

  ASTcontext* cxt;
  CodeGen* parent;
  std::ostream* os;
};

class Generator : public CodeGen {
private:
public:
  Generator(ASTcontext* cxt) : CodeGen(cxt, *this, std::cout) { }   

  bool is_generator() { return true; }
};

class File : public CodeGen {
private:
public:
  File(CodeGen& gen) : CodeGen(gen) { }

  Generator& get_parent() { return parent->as_generator(); }
  bool is_file() { return true; }
};
  
class Break_Continue {
private:
public:
  const std::string* break_label;
  const std::string* continue_label;
};

class Method : public CodeGen {
private:
public:
  Method(File& gen, Decl* fn) : CodeGen(gen.cxt, gen, ss), ss(), fn(fn), stack(0), mstack(0) { }

  File& get_parent() { return parent->as_file(); }
  bool is_method() { return true; }

  int declare_local(Decl* d);

  int lookup_local(Decl* d);

  std::string make_label(const std::string& s);

  void push_break_continue(std::string& b, std::string& c) { bc.push({&b, &c}); }

  void pop_break_continue() { bc.pop(); }

  const std::string& get_break_label() { return *bc.top().break_label; }
  const std::string& get_continue_label() { return *bc.top().continue_label; }

  int stack_size() { return stack; }
  int max_stack_size() { return mstack; }

  void push(int n) { stack += n; mstack = std::max(stack, mstack); }

  void pop(int n) { stack -= n; }

  void clear() { stack = 0; }

  std::stringstream ss;
  Decl* fn;
  int stack;
  int mstack;
  std::unordered_map<Decl*, int> locals;
  std::unordered_map<std::string, int> labels;
  std::stack<Break_Continue> bc;
};

int Method::declare_local(Decl* d) {
  auto result = locals.emplace(d, locals.size());
  return result.first->second;
}

int Method::lookup_local(Decl* d) {
  return locals.find(d)->second;
}

std::string Method::make_label(const std::string& s) {
  auto result = labels.emplace(s, 0);
  auto iter = result.first;

  ++iter->second;

  std::stringstream ss;
  ss << s << '/' << iter->second;
  return ss.str();
}

class Block : public CodeGen {
private:
public:
  Block(Method& gen, const std::string& n) : CodeGen(gen.cxt, gen, ss), ss(), label(get_method_codegen(gen).make_label(n)) { }

  std::string label;
  std::stringstream ss;
};

class generating_loop {
private:
public:
  generating_loop(Method& m, std::string& b, std::string& c) : cg(m) {
    cg.push_break_continue(b, c);
  }

  ~generating_loop() { cg.pop_break_continue(); }
  
  Method& cg;
};
/*
class Gen_Value {
private:
public:
  Gen_Value() : d() { }
  Gen_Value(Decl* d) : d(d) { }

  Decl* d:
};
*/
Generator& CodeGen::as_generator() {
  if (is_generator()) 
    return static_cast<Generator&>(*this); 
}
File& CodeGen::as_file() { 
  if (is_file()) 
    return static_cast<File&>(*this); 
}
Method& CodeGen::as_method() { 
  if (is_method()) 
    return static_cast<Method&>(*this); 
}
Block& CodeGen::as_block() { 
  if (is_block()) 
    return static_cast<Block&>(*this); 
}

Generator& get_generator(CodeGen& gen) {
  CodeGen& p = get_file_codegen(gen);
  return dynamic_cast<Generator&>(gen);
}

File& get_file_codegen(CodeGen& gen) {
  if (File* f = dynamic_cast<File*>(&gen))
      return *f;
  if (Generator* g = dynamic_cast<Generator*>(&gen))
    std::cout << "Shouldn't have happend";
  return get_file_codegen(gen.get_parent());
}

Method& get_method_codegen(CodeGen& gen) {
  if (Method* m = dynamic_cast<Method*>(&gen))
      return *m;
  if (File* f = dynamic_cast<File*>(&gen))
    std::cout << "Shouldn't have happend";
  if (Generator* g = dynamic_cast<Generator*>(&gen))
    std::cout << "Shouldn't have happend";
  return get_method_codegen(gen.get_parent());
}

Block& get_block_codegen(CodeGen& gen) { 
  if (Block* bg = dynamic_cast<Block*>(&gen)) 
    return *bg;
}

std::string describe(CodeGen&, Type*);

std::string describe_fn(CodeGen& gen, Fn_Type* t) {
  std::stringstream ss;
  ss << '(';
  for (Type* p : t->params)
    ss << describe(gen, p);
  ss << ')' << describe(gen, t->ret);
  return ss.str();
}

std::string describe(CodeGen& gen, Type* t) {
  if (dynamic_cast<Bool_Type*>(t))
    return "Z";
  if (dynamic_cast<Int_Type*>(t))
    return "I";
  if (dynamic_cast<Fn_Type*>(t))
    return describe_fn(gen, dynamic_cast<Fn_Type*>(t));
}

void emit_label(CodeGen& gen, const std::string& label);
void emit_goto(CodeGen& gen, const std::string& label);
void emit_pop(CodeGen& gen);
void emit_dup(CodeGen& gen);
void emit_ipush(CodeGen& gen, int n);
void stack_unary_ops(CodeGen& gen);
void stack_binary_ops(CodeGen& gen);
void emit_iadd(CodeGen& gen);
void emit_isub(CodeGen& gen);
void emit_imul(CodeGen& gen);
void emit_idiv(CodeGen& gen);
void emit_irem(CodeGen& gen);
void emit_ineg(CodeGen& gen);
void emit_iand(CodeGen& gen);
void emit_ior(CodeGen& gen);
void emit_ixor(CodeGen& gen);
void emit_icmp(CodeGen& gen, void (*emit_if)(CodeGen&, const std::string&));
void emit_ieq(CodeGen& gen);
void emit_ine(CodeGen& gen);
void emit_ilt(CodeGen& gen);
void emit_igt(CodeGen& gen);
void emit_ile(CodeGen& gen);
void emit_ige(CodeGen& gen);
void emit_if_icmp(CodeGen& gen, const char* cmp, const std::string& label);

void emit_if_icmpeq(CodeGen& gen, const std::string& label);
void emit_if_icmpne(CodeGen& gen, const std::string& label);
void emit_if_icmplt(CodeGen& gen, const std::string& label);
void emit_if_icmpgt(CodeGen& gen, const std::string& label);
void emit_if_icmple(CodeGen& gen, const std::string& label);
void emit_if_icmpge(CodeGen& gen, const std::string& label);
void emit_ireturn(CodeGen& gen);
void emit_istore(CodeGen& gen, int n);
void emit_iload(CodeGen& gen, int n);
void emit_new(CodeGen& gen, const std::string& cls);
void emit_invoke_static(CodeGen& gen, Decl* d);
void emit_invoke_special(CodeGen& gen, const std::string& s);
void emit_throw(CodeGen& gen);

void emit_label(CodeGen& gen, const std::string& label) {
  gen.outs() << "  " << label << ":\n";
}

void emit_goto(CodeGen& gen, const std::string& label) {
  gen.outs() << "  goto  " << label << '\n';
}

void emit_pop(CodeGen& gen) {
  gen.outs() << "  pop\n";
  Method& m = get_method_codegen(gen);
  m.pop(1);
}

void emit_dup(CodeGen& gen) {
  gen.outs() << "  dup\n";
  Method& m = get_method_codegen(gen);
  m.push(1);
}

void emit_ipush(CodeGen& gen, int n) {
  switch(n) {
  case -1: gen.outs() << "  iconst_m1\n"; break;
  case 0: gen.outs() << "  iconst_0\n"; break;
  case 1: gen.outs() << "  iconst_1\n"; break;
  case 2: gen.outs() << "  iconst_2\n"; break;
  case 3: gen.outs() << "  iconst_3\n"; break;
  case 4: gen.outs() << "  iconst_4\n"; break;
  case 5: gen.outs() << "  iconst_5\n"; break;
  default:
    if (-127 < n && n < 127)
      gen.outs() << "  bipush  " << n << '\n';
    else if (-32767 < n && n < 32767)
      gen.outs() << "  sipush  " << n << '\n';
    break;
  };
  Method& m = get_method_codegen(gen);
  m.push(1);
}

void stack_unary_ops(CodeGen& gen) {
  Method& m = get_method_codegen(gen);
  m.pop(1);
  m.push(1);
}

void stack_binary_ops(CodeGen& gen) {
  Method& m = get_method_codegen(gen);
  m.pop(2);
  m.push(1);
}

void emit_iadd(CodeGen& gen) {
  gen.outs() << "  iadd\n";
  stack_binary_ops(gen);
}

void emit_isub(CodeGen& gen) {
  gen.outs() << "  isub\n";
  stack_binary_ops(gen);
}

void emit_imul(CodeGen& gen) {
  gen.outs() << "  imul\n";
  stack_binary_ops(gen);
}

void emit_idiv(CodeGen& gen) {
  gen.outs() << "  idiv\n";
  stack_binary_ops(gen);
}

void emit_irem(CodeGen& gen) {
  gen.outs() << "  irem\n";
  stack_binary_ops(gen);
}

void emit_ineg(CodeGen& gen) {
  gen.outs() << "  ineg\n";
  stack_unary_ops(gen);
}

void emit_iand(CodeGen& gen) {
  gen.outs() << "  iand\n";
  stack_binary_ops(gen);
}

void emit_ior(CodeGen& gen) {
  gen.outs() << "  ior\n";
  stack_binary_ops(gen);
}

void emit_ixor(CodeGen& gen) {
  gen.outs() << "  ixor\n";
  stack_binary_ops(gen);
}

void emit_icmp(CodeGen& gen, void (*emit_if)(CodeGen&, const std::string&)) {
  Method& m = get_method_codegen(gen);
  std::string then = m.make_label("if/then");
  std::string end = m.make_label("if/end");
  emit_if(gen, then);
  emit_ipush(gen, 0);
  emit_goto(gen, end);
  emit_label(gen, then);
  emit_ipush(gen, 1);
  emit_label(gen, end);
}

void emit_ieq(CodeGen& gen) {
  emit_icmp(gen, emit_if_icmpeq);
}

void emit_ine(CodeGen& gen) {
  emit_icmp(gen, emit_if_icmpne);
}

void emit_ilt(CodeGen& gen) {
  emit_icmp(gen, emit_if_icmplt);
}

void emit_igt(CodeGen& gen) {
  emit_icmp(gen, emit_if_icmpgt);
}

void emit_ile(CodeGen& gen) {
  emit_icmp(gen, emit_if_icmple);
}

void emit_ige(CodeGen& gen) {
  emit_icmp(gen, emit_if_icmpge);
}

void emit_if_icmp(CodeGen& gen, const char* cmp, const std::string& label) {
  gen.outs() << "  if_icmp" << cmp << ' ' << label << '\n';
  Method& m = get_method_codegen(gen);
  m.pop(2);
}

void emit_if_icmpeq(CodeGen& gen, const std::string& label) {
  emit_if_icmp(gen, "eq", label);
}

void emit_if_icmpne(CodeGen& gen, const std::string& label) {
  emit_if_icmp(gen, "ne", label);
}

void emit_if_icmplt(CodeGen& gen, const std::string& label) {
  emit_if_icmp(gen, "lt", label);
}

void emit_if_icmpgt(CodeGen& gen, const std::string& label) {
  emit_if_icmp(gen, "gt", label);
}


void emit_if_icmple(CodeGen& gen, const std::string& label) {
  emit_if_icmp(gen, "le", label);
}


void emit_if_icmpge(CodeGen& gen, const std::string& label) {
  emit_if_icmp(gen, "ge", label);
}

void emit_ireturn(CodeGen& gen) {
  gen.outs() << "  ireturn\n";
  Method& m = get_method_codegen(gen);
  m.clear();
}

void emit_istore(CodeGen& gen, int n) {
  switch(n) {
  case 0: gen.outs() << "  istore_0\n"; break;
  case 1: gen.outs() << "  istore_1\n"; break;
  case 2: gen.outs() << "  istore_2\n"; break;
  case 3: gen.outs() << "  istore_3\n"; break;
  default: gen.outs() << "  istore " << n << '\n'; break;
  };
  Method& m = get_method_codegen(gen);
  m.pop(1);
}

void emit_iload(CodeGen& gen, int n) {
  switch(n) {
  case 0: gen.outs() << "  iload_0\n"; break;
  case 1: gen.outs() << "  iload_1\n"; break;
  case 2: gen.outs() << "  iload_2\n"; break;
  case 3: gen.outs() << "  iload_3\n"; break;
  default: gen.outs() << "  iload " << n << '\n'; break;
  };
  Method& m = get_method_codegen(gen);
  m.push(1);
}

void emit_new(CodeGen& gen, const std::string& cls) {
  gen.outs() << "  new " << cls << '\n';
  Method& m = get_method_codegen(gen);
  m.push(1);
}

void emit_invoke_static(CodeGen& gen, Decl* d) {
  auto* fn = static_cast<Fn_Decl*>(d);

  gen.outs() << "  invokestatic " << "Main/" << fn->getName() << describe(gen, fn->getType()) << '\n';

  Method& m = get_method_codegen(gen);
  m.pop(fn->params.size());
  m.push(1);
}

void emit_invoke_special(CodeGen& gen, const std::string& s) {
  gen.outs() << "  invokespecial " << s << '\n';
}

void emit_throw(CodeGen& gen) {
  gen.outs() << "  athorw\n";
  Method& m = get_method_codegen(gen);
  m.clear();
  m.push(1);
}
#endif

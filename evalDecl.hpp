#ifndef EVALDECL_HPP
#define EVALDECL_HPP

#include "decl.hpp"
#include "evaluator.hpp"

void eval_decl(Evaluator& evaluate, Decl* d) {
  class V: public Decl::Visitor {
  private:
    Evaluator& ev;

  public:
    V(Evaluator& ev) : ev(ev) {}

    void visit(Name_Decl* d) {} // Not Used
    void visit(Var_Decl* d) {
      std::cout << "Evaluating that Var Decl yo\n";
      Value val = ev.automatic_allocate();
      ev.automatic_bind(d, val);

      ev.copy_initialize(val, d->init);
    } 
    void visit(Fn_Decl* d) {} // Not Used
    void visit(Param_Decl* d) {} // Not Used
    void visit(Ret_Decl* d) {} // Not Used
    void visit(Local_Var_Decl* d) {
      std::cout << "Evaluating that Local Var Decl yo\n";
      Value val = ev.automatic_allocate();
      ev.automatic_bind(d, val);

      ev.copy_initialize(val, d->init);
    }
    void visit(Program_Decl* d) {}
  };
  V vis(evaluate);
  d->accept(vis);
}
#endif

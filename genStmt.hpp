#ifndef GENSTMT_HPP
#define GENSTMT_HPP

#include "stmt.hpp"
#include "value.hpp"

Value gen_expr(CodeGen&, Expr*);
void gen_decl(CodeGen&, Decl*);

void gen_stmt(CodeGen& generator, Stmt* s) {
  class V: public Stmt::Visitor {
  private:
    CodeGen& gen;

  public:
    V(CodeGen& gen) : gen(gen) {}
  
    void visit(Expr_Stmt* stmt) { gen_expr(gen, stmt->getExpr()); }
    void visit(Decl_Stmt* stmt) { gen_decl(gen, stmt->getDecl()); }
    void visit(Block_Stmt* stmt) {
      for (Stmt* st : stmt->Get_Statements())
	gen_stmt(gen, st);
    }
    void visit(If_Stmt* stmt) {
      Method& m = get_method_codegen(gen);
      std::string then = m.make_label("if/then");
      std::string end = m.make_label("if/end");
      gen_expr(gen, stmt->Get_Condition());
      emit_ipush(gen, 1);
      emit_if_icmpeq(gen, then);
      gen_stmt(gen, stmt->GetFalseBlock());
      emit_goto(gen, end);
      emit_label(gen, then);
      gen_stmt(gen, stmt->GetTrueBlock());
      emit_label(gen, end);
    }
    void visit(While_Stmt* stmt) {
      Method& m = get_method_codegen(gen);
      std::string top = m.make_label("while/top");
      std::string bot = m.make_label("while/bot");
      generating_loop loop(m, top, bot);
      emit_label(gen, top);
      gen_expr(gen, stmt->Get_Condition());
      emit_ipush(gen, 0);
      emit_if_icmpeq(gen, bot);
      gen_stmt(gen, stmt->Body);
      emit_goto(gen, top);
      emit_label(gen, bot);
    }
    void visit(Break_Stmt* stmt) {
      Method& m = get_method_codegen(gen);
      emit_goto(gen, m.get_break_label());
    }
    void visit(Continue_Stmt* stmt) {
      Method& m = get_method_codegen(gen);
      emit_goto(gen, m.get_continue_label());
    }
    void visit(Return_Stmt* stmt) {
      Expr* e = stmt->Get_Return();
      gen_expr(gen, e);

      Type* t = e->getType();

      emit_ireturn(gen);
    }
  };
  V vis(generator);
  s->accept(vis);
}
#endif

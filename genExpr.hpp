#ifndef GENEXPR_HPP
#define GENEXPR_HPP

#include "bool_expr.hpp"
#include "int_expr.hpp"

void gen_decl(CodeGen&, Decl*);
void gen_stmt(CodeGen&, Stmt*);

// Implementation of the Visistor pattern allowing for the evaluation of an expression to be conducted.
// Returns the result of the expression e that is passed in.
Value gen_expr(CodeGen& generator, Expr *e)
{
  class V : public Expr::Visitor {
  private:
    Value r;
    CodeGen& gen;

  public:
    V(CodeGen& gen) : gen(gen) {}

    Value getR() { return r; }

    // Overriding of each visit virtual function set to the desired functionality of each expression.
    void visit(Call_Expr* e) {
      throw Generator_Exception("Call Expr not implemented");
      Value fn = gen_expr(gen, e->getFunction());

      for (Expr* arg: e->getArguments())
	gen_expr(gen, arg);

      //Decl* d = fn.get_delcaration();
      //emit_invoke_static(gen, d);
    }
    void visit(Assign_Expr* e) {
      throw Generator_Exception("Assign Expr not implemented");
      Method& m = get_method_codegen(gen);

      //Value val = gen_expr(gen, e->getE1());
      //gen_expr(gen, e->getE2());
    }
    void visit(Value_Expr* e) {
      throw Generator_Exception("One Complement Expr not implemented");
    }
    void visit(Ref_Expr* e) { gen_decl(gen, e->getDecl()); }
    void visit(Init_Expr* e) { gen_expr(gen, e->getE()); }
    void visit(Bind_Expr* e) { gen_expr(gen, e->getE()); }
    void visit(Bool_Expr* e) { emit_ipush(gen, e->getValue()); }
    void visit(And_Expr* e) {
      gen_expr(gen, e->getE1());
      gen_expr(gen, e->getE2());
      emit_iand(gen);
    }
    void visit(Or_Expr* e) {
      gen_expr(gen, e->getE1());
      gen_expr(gen, e->getE2());
      emit_ior(gen);
    }
    void visit(Xor_Expr* e) {
      gen_expr(gen, e->getE1());
      gen_expr(gen, e->getE2());
      emit_ixor(gen);
    }
    void visit(Not_Expr* e) {
      gen_expr(gen, e->getE());
      emit_ipush(gen, 1);
      emit_ixor(gen);
    }
    void visit(Eq_Expr* e) {
      gen_expr(gen, e->getE1());
      gen_expr(gen, e->getE2());
      emit_ieq(gen);
    }
    void visit(NotEq_Expr* e) {
      gen_expr(gen, e->getE1());
      gen_expr(gen, e->getE2());
      emit_ine(gen);
    }
    void visit(Cond_Expr* e) {
      Method& m = get_method_codegen(gen);
      std::string then = m.make_label("if/then");
      std::string end = m.make_label("if/end");
      gen_expr(gen, e->getE1());
      emit_ipush(gen, 1);
      emit_if_icmpeq(gen, then);
      gen_expr(gen, e->getE3());
      emit_goto(gen, end);
      emit_label(gen, then);
      gen_expr(gen, e->getE2());
      emit_label(gen, end);
    }
    void visit(AndThen_Expr* e) {
      Method& m = get_method_codegen(gen);
      std::string skip = m.make_label("and/skip");
      gen_expr(gen, e->getE1());
      emit_ipush(gen, 0);
      emit_if_icmpeq(gen, skip);
      gen_expr(gen, e->getE2());
      emit_label(gen, skip); 
    }
    void visit(OrElse_Expr* e) {
      Method& m = get_method_codegen(gen);
      std::string skip = m.make_label("and/skip");
      gen_expr(gen, e->getE1());
      emit_ipush(gen, 1);
      emit_if_icmpeq(gen, skip);
      gen_expr(gen, e->getE2());
      emit_label(gen, skip);
    }

    void visit(Int_Expr* e) {
      emit_ipush(gen, e->getValue());
    }
    void visit(Add_Expr* e) {
      gen_expr(gen, e->getE1());
      gen_expr(gen, e->getE2());
      emit_iadd(gen);
    }
    void visit(Sub_Expr* e) {
      gen_expr(gen, e->getE1());
      gen_expr(gen, e->getE2());
      emit_isub(gen);
    }
    void visit(Mult_Expr* e) {
      gen_expr(gen, e->getE1());
      gen_expr(gen, e->getE2());
      emit_imul(gen);
    }
    void visit(Div_Expr* e) {
      gen_expr(gen, e->getE1());
      gen_expr(gen, e->getE2());
      emit_idiv(gen);
    }
    void visit(Mod_Expr* e) {
      gen_expr(gen, e->getE1());
      gen_expr(gen, e->getE2());
      emit_irem(gen);
    }
    void visit(LessThan_Expr* e) {
      gen_expr(gen, e->getE1());
      gen_expr(gen, e->getE2());
      emit_ilt(gen);
    }
    void visit(GreaterThan_Expr* e) {
      gen_expr(gen, e->getE1());
      gen_expr(gen, e->getE2());
      emit_igt(gen);
    }
    void visit(LessEqThan_Expr* e) {
      gen_expr(gen, e->getE1());
      gen_expr(gen, e->getE2());
      emit_ile(gen);
    }
    void visit(GreaterEqThan_Expr* e) {
      gen_expr(gen, e->getE1());
      gen_expr(gen, e->getE2());
      emit_ige(gen);
    }
    void visit(Negation_Expr *e) {
      gen_expr(gen, e->getE());
      emit_ineg(gen);
    }
    void visit(OneComplement_Expr *e) {
      throw Generator_Exception("One Complement Expr not implemented");
    }
  };
  V vis(generator);
  e->accept(vis);
  return vis.getR();
}
#endif

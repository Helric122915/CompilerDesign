#ifndef GENDECL_HPP
#define GENDECL_HPP

#include "decl.hpp"

Value gen_expr(CodeGen&, Expr*);
void gen_stmt(CodeGen&, Stmt*);

void gen_decl(CodeGen& generator, Decl* d) {
  class V: public Decl::Visitor {
  private:
    CodeGen& gen;

  public:
    V(CodeGen& gen) : gen(gen) {}

    void visit(Name_Decl* d) { throw Generator_Exception("Unreachable"); }
    void visit(Var_Decl* d) {
      Method& m = get_method_codegen(gen);
      int n = m.declare_local(d);

      gen_expr(gen, d->init);
    
      Type* t = d->getType();

      emit_istore(gen, n);
    }
    void visit(Fn_Decl* d) {
      Method mgen(get_file_codegen(gen), d);

      std::string name = d->getName();

      if (name == "main")
	name = "__main";

      mgen.outs() << ".method public static "
		  << name
		  << describe(gen, d->getType())
		  << '\n';

      for (Decl* p : d->params)
	mgen.declare_local(p);

      Block bgen(mgen, "entry");
      if (Stmt* s = d->body)
	gen_stmt(bgen, s);

      mgen.outs() << "  .limit stack " << mgen.max_stack_size() << '\n';
      mgen.outs() << "  .limit locals " << mgen.locals.size() << '\n';

      mgen.outs() << bgen.outs().rdbuf();
      mgen.outs() << ".end method\n\n";

      gen.outs() << mgen.outs().rdbuf();
    }
    void visit(Param_Decl* d) { throw Generator_Exception("Unreachable"); }
    void visit(Ret_Decl* d) { throw Generator_Exception("Unreachable"); }
    void visit(Local_Var_Decl* d) {
      Method& m = get_method_codegen(gen);
      int n = m.declare_local(d);

      gen_expr(gen, d->init);
    
      Type* t = d->getType();

      emit_istore(gen, n);
    }
    void visit(Program_Decl* d) {
      File fgen(gen);

      fgen.outs() << ".class public Main\n";
      fgen.outs() << ".super java/lang/Object\n\n";

      fgen.outs() << ".method public <init>()V\n";
      fgen.outs() << "  aload_0\n";
      fgen.outs() << "  invokespecial java/lang/Object/<init>()V\n";
      fgen.outs() << "  return\n";
      fgen.outs() << ".end method\n\n";

      fgen.outs() << ".method public static main([Ljava/lang/String;)V\n";
      fgen.outs() << "  .limit stack 1\n";
      fgen.outs() << "  .limit locals 1\n";
      fgen.outs() << "  invokestatic Main/__main()I\n";
      fgen.outs() << "  return\n";
      fgen.outs() << ".end method\n\n";

      for (Decl* decl : d->decls)
	gen_decl(fgen, decl);
    }
  };
  V vis(generator);
  d->accept(vis);
}
#endif

#ifndef PRINT_HPP
#define PRINT_HPP

#include "bool_expr.hpp"
#include "int_expr.hpp"

// Helper Visitor function to set if each expression type need parens.
bool needs_parens(Expr* e) {
  class V : public Expr::Visitor {
  private:
    bool r;

  public:
    // Accessor function to retrieve the private variable r.
    bool getR() { return r; }

    // Overriding of each visit virtual function set to the desired functionality of each expression.
    void visit(Bool_Expr* e) { r = false; }
    void visit(And_Expr* e) { r = true; }
    void visit(Or_Expr* e) { r = true; }
    void visit(Xor_Expr* e) { r = true; }
    void visit(Not_Expr* e) { r = true; }
    void visit(Eq_Expr* e) { r = true; }
    void visit(NotEq_Expr* e) { r = true; }
    void visit(Cond_Expr* e) { r = true; }
    void visit(AndThen_Expr* e) { r = true; }
    void visit(OrElse_Expr* e) { r = true; }

    void visit(Int_Expr* e) { r = false; }
    void visit(Add_Expr* e) { r = true; }
    void visit(Sub_Expr* e) { r = true; }
    void visit(Mult_Expr* e) { r = true; }
    void visit(Div_Expr* e) { r = true; }
    void visit(Mod_Expr* e) { r = true; }
    void visit(LessThan_Expr* e) { r = true; }
    void visit(GreaterThan_Expr* e) { r = true; }
    void visit(LessEqThan_Expr* e) { r = true; }
    void visit(GreaterEqThan_Expr* e) { r = true; }
    void visit(Negation_Expr* e) { r = false; }
    void visit(OneComplement_Expr*e ) { r = false; }
  };
  V vis;
  e->accept(vis);
  return vis.getR();
}

// Implementation of the Visitor pattern allowing for the printing of each expression to be completed.
// Returns nothing, only prints the expression to the terminal.
void print(Expr *e)
{
  class V : public Expr::Visitor {
    // Helper function that prints parens if they are needed for the expression e.
  private:
    void print_enclosed(Expr* e) {
      if (needs_parens(e)) {
	std::cout << "(";
	print(e);
	std::cout << ")";
      }
      else
	print(e);
    }

  public:
    // Overriding of each visit virtual function set to the desired functionality of each expression.
    void visit(Bool_Expr* e) {
      if (e->getValue())
	std::cout << "True";
      else
	std::cout << "False";
    }
    void visit(And_Expr* e) {
      print_enclosed(e->getE1());
      std::cout << " & ";
      print_enclosed(e->getE2());
    }
    void visit(Or_Expr* e) {
      print_enclosed(e->getE1());
      std::cout << " | ";
      print_enclosed(e->getE2());
    }
    void visit(Xor_Expr* e) {
      print_enclosed(e->getE1());
      std::cout << " ^ ";
      print_enclosed(e->getE2());
    }
    void visit(Not_Expr* e) {
      std::cout << "!";
      print_enclosed(e->getE());
    }
    void visit(Eq_Expr* e) {
      print_enclosed(e->getE1());
      std::cout << " == ";
      print_enclosed(e->getE2());
    }
    void visit(NotEq_Expr* e) {
      print_enclosed(e->getE1());
      std::cout << " != ";
      print_enclosed(e->getE2());
    }
    void visit(Cond_Expr* e) {
      print_enclosed(e->getE1());
      std::cout << " ? ";
      print_enclosed(e->getE2());
      std::cout << " : ";
      print_enclosed(e->getE3());
    }
    void visit(AndThen_Expr* e) {
      print_enclosed(e->getE1());
      std::cout << " ? ";
      print_enclosed(e->getE2());
      std::cout << " : ";
      std::cout << "False";
    }
    void visit(OrElse_Expr* e) {
      print_enclosed(e->getE1());
      std::cout << " ? ";
      std::cout << "True";
      std::cout << " : ";
      print_enclosed(e->getE2());
    }

    void visit(Int_Expr* e) {
      std::cout << e->getValue();
    }
    void visit(Add_Expr* e) {
      print_enclosed(e->getE1());
      std::cout << " + ";
      print_enclosed(e->getE2());
    }
    void visit(Sub_Expr* e) {
      print_enclosed(e->getE1());
      std::cout << " - ";
      print_enclosed(e->getE2());
    }
    void visit(Mult_Expr* e) {
      print_enclosed(e->getE1());
      std::cout << " * ";
      print_enclosed(e->getE2());
    }
    void visit(Div_Expr* e) {
      print_enclosed(e->getE1());
      std::cout << " / ";
      print_enclosed(e->getE2());
    }
    void visit(Mod_Expr* e) {
      print_enclosed(e->getE1());
      std::cout << " % ";
      print_enclosed(e->getE2());
    }
    void visit(LessThan_Expr* e) {
      print_enclosed(e->getE1());
      std::cout << " < ";
      print_enclosed(e->getE2());
    }
    void visit(GreaterThan_Expr* e) {
      print_enclosed(e->getE1());
      std::cout << " > ";
      print_enclosed(e->getE2());
    }
    void visit(LessEqThan_Expr* e) {
      print_enclosed(e->getE1());
      std::cout << " <= ";
      print_enclosed(e->getE2());
    }
    void visit(GreaterEqThan_Expr* e) {
      print_enclosed(e->getE1());
      std::cout << " >= ";
      print_enclosed(e->getE2());
    }
    void visit(Negation_Expr* e) {
      std::cout << "-";
      print_enclosed(e->getE());
    }
    void visit(OneComplement_Expr*e ) {
      std::cout << "~";
      print_enclosed(e->getE());
    }
  };
  V vis;
  e->accept(vis);
}
#endif

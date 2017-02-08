#ifndef PRINT_HPP
#define PRINT_HPP

#include "bool_expr.hpp"
#include "int_expr.hpp"

bool needs_parens(Expr* e) {
  class V : public Expr::Visitor {
    private:
      bool r;

    public:
      bool getR() { return r; }
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
  };
  V vis;
  e->accept(vis);
  return vis.getR();
}

void print(Expr *e)
{
  class V : public Expr::Visitor {
    public:
      void print_enclosed(Expr* e) {
        if (needs_parens(e)) {
          std::cout << "(";
          print(e);
          std::cout << ")";
        }
        else
          print(e);
      }

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
  };
  V vis;
  e->accept(vis);
}
#endif
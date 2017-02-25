#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include "type.hpp"

// Bool Expressions
class Bool_Expr;
class And_Expr;
class Or_Expr;
class Xor_Expr;
class Not_Expr;
class Eq_Expr;
class NotEq_Expr;
class Cond_Expr;
class AndThen_Expr;
class OrElse_Expr;

// Int Expressions
class Int_Expr;
class Add_Expr;
class Sub_Expr;
class Mult_Expr;
class Div_Expr;
class Mod_Expr;
class LessThan_Expr;
class GreaterThan_Expr;
class LessEqThan_Expr;
class GreaterEqThan_Expr;
class Negation_Expr;
class OneComplement_Expr;

// Context class that creates the bool and int type that will be held as type* throughout the
// program.
class ASTcontext {
public:
  const Bool_Type* Bool_;
  const Int_Type* Int_;

  ~ASTcontext();

  // Lists that will be used to store references to Expr objects to be deleted.
  std::list<Bool_Expr> Bools;
  std::list<And_Expr> Ands;
  std::list<Or_Expr> Ors;
  std::list<Xor_Expr> Xors;
  std::list<Not_Expr> Nots;
  std::list<Eq_Expr> Eqs;
  std::list<NotEq_Expr> Noteqs;
  std::list<Cond_Expr> Conds;
  std::list<AndThen_Expr> Andthens;
  std::list<OrElse_Expr> Orelses;

  std::list<Int_Expr> Ints;
  std::list<Add_Expr> Adds;
  std::list<Sub_Expr> Subs;
  std::list<Mult_Expr> Mults;
  std::list<Div_Expr> Divs;
  std::list<Mod_Expr> Mods;
  std::list<LessThan_Expr> Lessthans;
  std::list<GreaterThan_Expr> Greaterthans;
  std::list<LessEqThan_Expr> Lesseqthans;
  std::list<GreaterEqThan_Expr> Greatereqthans;
  std::list<Negation_Expr> Negations;
  std::list<OneComplement_Expr> Complements;
};

// Clears all of the lists of references to release used memory.
ASTcontext::~ASTcontext() {
  Bools.clear();
  Ands.clear();
  Ors.clear();
  Xors.clear();
  Nots.clear();
  Eqs.clear();
  Noteqs.clear();
  Conds.clear();
  Andthens.clear();
  Orelses.clear();

  Ints.clear();
  Adds.clear();
  Subs.clear();
  Mults.clear();
  Divs.clear();
  Lessthans.clear();
  Greaterthans.clear();
  Lesseqthans.clear();
  Greatereqthans.clear();
  Negations.clear();
  Complements.clear();
}
#endif

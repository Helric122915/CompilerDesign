#ifndef ENUMERATION_HPP
#define ENUMERATION_HPP

enum flow_kind {
  flow_next,
  flow_return,
  flow_break,
  flow_continue
};

enum Token_Kind {
  Eof_Tok,
  Eq_Tok,
  NotEq_Tok,
  LParens_Tok,
  RParens_Tok,
  LessThan_Tok,
  LessEqThan_Tok,
  GreaterThan_Tok,
  GreaterEqThan_Tok,
  Plus_Tok,
  Minus_Tok,
  Mult_Tok,
  Div_Tok,
  Mod_Tok,
  Not_Tok,
  Or_Tok,
  OrElse_Tok,
  And_Tok,
  AndThen_Tok,
  Xor_Tok,
  OneComplement_Tok,
  QuestionMark_Tok,
  Colon_Tok,
  Int_Tok,
  Bool_Tok,
  Ident_Tok,
  Semicolon_Tok,
  Assign_Tok,
  LBrace_Tok,
  RBrace_Tok,
  Arrow_Tok,

  // Keyword Tokens
  Int_Kw,
  Bool_Kw,
  Var_Kw,
  For_Kw,
  If_Kw,
  Else_Kw,
  While_Kw,
  Break_Kw,
  Continue_Kw,
  Return_Kw,
  Def_Kw
};

enum Value_Kind {
  uninit_value,
  int_value,
  ref_value,
  fn_value
};

enum Scope_Kind {
  global_scope,
  function_param_scope,
  function_scope,
  block_scope
};
#endif

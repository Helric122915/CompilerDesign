#ifndef DECLARATIVE_HPP
#define DECLARATIVE_HPP

class Declarative_Region {
private:
public:
  Declarative_Region(Semantic*, int, Decl*);
  Declarative_Region(Semantic*, int);
  ~Declarative_Region();

  Semantic* sema;
  Decl* prev;
};

Declarative_Region::Declarative_Region(Semantic* sema, int scope_kind, Decl* d) : sema(sema), prev(sema->current_context()) {
  sema->set_current_context(d);
  sema->enter_scope(scope_kind);
}

Declarative_Region::Declarative_Region(Semantic* sema, int scope_kind) : sema(sema), prev(nullptr) {
  sema->enter_scope(scope_kind);
}

Declarative_Region::~Declarative_Region() {
  if (prev)
    sema->set_current_context(prev);
  sema->leave_scope();
}
#endif

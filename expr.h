#ifndef EXPR_H
#define EXPR_H

#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "type.h"

Bool_Type Bool_;
Int_Type Int_;

Type_Exception Type_Error;

class Expr {
  public:
    const Type *type;

    virtual ~Expr(){}
    virtual int weight() = 0;
    virtual int height() = 0;
    virtual int eval() = 0;
};
#endif

#ifndef INTERP_H
#define INTERP_H

#include "ast.h"

typedef enum {
    NUM_V,
    BOOL_V,
    STR_V,
    CLOSURE_V,
    PRIMOP_V
} ValType;

typedef struct Env Env;
typedef struct Value Value;

struct Value {
    ValType type;

    double num;
    int boolean;
    char *str;

    char **params;
    int num_params;
    ExprC *body;
    Env *closure_env;
};

// environment is just a linked list
struct Env {
    char *name;
    Value *val;
    Env *next;
};

Value *interp(ExprC *expr, Env *env);
void serialize(Value *v);
void top_interp(ExprC *expr);
Env *top_env(void);

#endif

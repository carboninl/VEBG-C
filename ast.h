#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    NUM_C,
    BOOL_C,
    STR_C,
    ID_C,
    IF_C,
    LAM_C,
    APP_C
} ExprType;

// I know this wastes memory because every ExprC has all these fields
// even if most of them aren't used. I tried using a union but it got
// confusing so I just put everything in the struct.
typedef struct ExprC ExprC;

struct ExprC {
    ExprType type;

    // NUM_C
    double num;

    // BOOL_C
    int boolean;

    // STR_C and ID_C
    char *str;

    // IF_C
    ExprC *cond;
    ExprC *then_branch;
    ExprC *else_branch;

    // LAM_C
    char **params;
    int num_params;
    ExprC *body;

    // APP_C
    ExprC *func;
    ExprC **args;
    int num_args;
};

static ExprC *new_num(double n) {
    ExprC *expr = malloc(sizeof(ExprC));
    if (expr == NULL) {
        perror("malloc");
        exit(1);
    }

    expr->type = NUM_C;
    expr->num = n;
    return expr;
}

static ExprC *new_bool(int b) {
    ExprC *expr = malloc(sizeof(ExprC));
    if (expr == NULL) {
        perror("malloc");
        exit(1);
    }

    expr->type = BOOL_C;
    expr->boolean = b;
    return expr;
}

static ExprC *new_str(char *s) {
    ExprC *expr =malloc(sizeof(ExprC));
    if (expr == NULL) {
        perror("malloc");
        exit(1);
    }

    expr->type = STR_C;
    expr->str = s;
    return expr;
}

static ExprC *new_if(ExprC *cond, ExprC *then_branch, ExprC *else_branch) {
    ExprC *expr = malloc(sizeof(ExprC));
    if (expr == NULL) {
        perror("malloc");
        exit(1);
    }

    expr->type = IF_C;
    expr->cond = cond;
    expr->then_branch = then_branch;
    expr->else_branch = else_branch;
    return expr;
}

static ExprC *new_lam(char **params, int num_params, ExprC *body) {
    ExprC *expr = malloc(sizeof(ExprC)){
        if (expr == NULL) {
            perror("malloc");
            exit(1);
        }
    }

    expr->type = LAM_C;
    expr->params = params;
    expr->num_params = num_params;
    expr->body = body;
    return expr;
}
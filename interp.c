#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interp.h"

// Pranav's implementation 

// ripped straight from VEBG4
// made it a linked list so it works exactly the same conceptually
Value *env_lookup(Env *env, char *name) {
    Env *current = env;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current->val;
        }
        current = current->next;
    }
    printf("VEBG error: unbound variable %s\n", name);
    exit(1);
}

// add a new binding to the front of the environment
// has to be in front because we want to be able to shadow variables
Env *env_extend(Env *env, char *name, Value *val) {
    Env *new_env = malloc(sizeof(Env));
    new_env->name = name;
    new_env->val = val;
    new_env->next = env;
    return new_env;
}

// print a value to stdout
void serialize(Value *v) {
    if (v->type == NUM_V) {
        printf("%g\n", v->num);
    } else if (v->type == BOOL_V) {
        if (v->boolean == 1) {
            printf("true\n");
        } else {
            printf("false\n");
        }
    } else if (v->type == STR_V) {
        printf("\"%s\"\n", v->str);
    } else if (v->type == CLOSURE_V) {
        printf("#<procedure>\n");
    } else if (v->type == PRIMOP_V) {
        printf("#<primop>\n");
    }
}

// main interpreter
Value *interp(ExprC *expr, Env *env) {

    if (expr->type == NUM_C) {
        Value *v = malloc(sizeof(Value));
        v->type = NUM_V;
        v->num = expr->num;
        return v;
    }

    if (expr->type == BOOL_C) {
        Value *v = malloc(sizeof(Value));
        v->type = BOOL_V;
        v->boolean = expr->boolean;
        return v;
    }

    if (expr->type == STR_C) {
        Value *v = malloc(sizeof(Value));
        v->type = STR_V;
        v->str = expr->str;
        return v;
    }

    if (expr->type == ID_C) {
        return env_lookup(env, expr->str);
    }

    if (expr->type == IF_C) {
        Value *cond = interp(expr->cond, env);
        if (cond->type != BOOL_V) {
            printf("VEBG error: if condition is not a boolean\n");
            exit(1);
        }
        if (cond->boolean == 1) {
            return interp(expr->then_branch, env);
        } else {
            return interp(expr->else_branch, env);
        }
    }

    if (expr->type == LAM_C) {
        Value *v = malloc(sizeof(Value));
        v->type = CLOSURE_V;
        v->params = expr->params;
        v->num_params = expr->num_params;
        v->body = expr->body;
        v->closure_env = env;
        return v;
    }

    if (expr->type == APP_C) {
        Value *func = interp(expr->func, env);

        // evaluate all arguments
        Value **arg_vals = malloc(sizeof(Value*) * expr->num_args);
        for (int i = 0; i < expr->num_args; i++) {
            arg_vals[i] = interp(expr->args[i], env);
        }

        // call a primitive operator
        if (func->type == PRIMOP_V) {
            char *op = func->str;
            int n = expr->num_args;

            if (strcmp(op, "+") == 0) {
                if (n != 2) { printf("VEBG error: + needs 2 args\n"); exit(1); }
                if (arg_vals[0]->type != NUM_V || arg_vals[1]->type != NUM_V) {
                    printf("VEBG error: + requires numbers\n"); exit(1);
                }
                Value *result = malloc(sizeof(Value));
                result->type = NUM_V;
                result->num = arg_vals[0]->num + arg_vals[1]->num;
                return result;
            }

            if (strcmp(op, "-") == 0) {
                if (n != 2) { printf("VEBG error: - needs 2 args\n"); exit(1); }
                if (arg_vals[0]->type != NUM_V || arg_vals[1]->type != NUM_V) {
                    printf("VEBG error: - requires numbers\n"); exit(1);
                }
                Value *result = malloc(sizeof(Value));
                result->type = NUM_V;
                result->num = arg_vals[0]->num - arg_vals[1]->num;
                return result;
            }

            if (strcmp(op, "*") == 0) {
                if (n != 2) { printf("VEBG error: * needs 2 args\n"); exit(1); }
                if (arg_vals[0]->type != NUM_V || arg_vals[1]->type != NUM_V) {
                    printf("VEBG error: * requires numbers\n"); exit(1);
                }
                Value *result = malloc(sizeof(Value));
                result->type = NUM_V;
                result->num = arg_vals[0]->num * arg_vals[1]->num;
                return result;
            }

            if (strcmp(op, "/") == 0) {
                if (n != 2) { printf("VEBG error: / needs 2 args\n"); exit(1); }
                if (arg_vals[0]->type != NUM_V || arg_vals[1]->type != NUM_V) {
                    printf("VEBG error: / requires numbers\n"); exit(1);
                }
                if (arg_vals[1]->num == 0) {
                    printf("VEBG error: division by zero\n"); exit(1);
                }
                Value *result = malloc(sizeof(Value));
                result->type = NUM_V;
                result->num = arg_vals[0]->num / arg_vals[1]->num;
                return result;
            }

            if (strcmp(op, "<=") == 0) {
                if (n != 2) { printf("VEBG error: <= needs 2 args\n"); exit(1); }
                if (arg_vals[0]->type != NUM_V || arg_vals[1]->type != NUM_V) {
                    printf("VEBG error: <= requires numbers\n"); exit(1);
                }
                Value *result = malloc(sizeof(Value));
                result->type = BOOL_V;
                result->boolean = (arg_vals[0]->num <= arg_vals[1]->num) ? 1 : 0;
                return result;
            }

            if (strcmp(op, "equal?") == 0) {
                if (n != 2) { printf("VEBG error: equal? needs 2 args\n"); exit(1); }
                Value *a = arg_vals[0];
                Value *b = arg_vals[1];
                Value *result = malloc(sizeof(Value));
                result->type = BOOL_V;
                result->boolean = 0;
                if (a->type == CLOSURE_V || a->type == PRIMOP_V) {
                    result->boolean = 0;
                } else if (a->type != b->type) {
                    result->boolean = 0;
                } else if (a->type == NUM_V && a->num == b->num) {
                    result->boolean = 1;
                } else if (a->type == BOOL_V && a->boolean == b->boolean) {
                    result->boolean = 1;
                } else if (a->type == STR_V && strcmp(a->str, b->str) == 0) {
                    result->boolean = 1;
                }
                return result;
            }

            if (strcmp(op, "strlen") == 0) {
                if (n != 1) { printf("VEBG error: strlen needs 1 arg\n"); exit(1); }
                if (arg_vals[0]->type != STR_V) {
                    printf("VEBG error: strlen requires a string\n"); exit(1);
                }
                Value *result = malloc(sizeof(Value));
                result->type = NUM_V;
                result->num = strlen(arg_vals[0]->str);
                return result;
            }

            if (strcmp(op, "substring") == 0) {
                if (n != 3) { printf("VEBG error: substring needs 3 args\n"); exit(1); }
                if (arg_vals[0]->type != STR_V) {
                    printf("VEBG error: substring first arg must be string\n"); exit(1);
                }
                if (arg_vals[1]->type != NUM_V || arg_vals[2]->type != NUM_V) {
                    printf("VEBG error: substring start/stop must be numbers\n"); exit(1);
                }
                int start = (int)arg_vals[1]->num;
                int stop = (int)arg_vals[2]->num;
                int len = strlen(arg_vals[0]->str);
                if (start < 0 || stop > len || start > stop) {
                    printf("VEBG error: substring index out of range\n"); exit(1);
                }
                char *result_str = malloc(stop - start + 1);
                strncpy(result_str, arg_vals[0]->str + start, stop - start);
                result_str[stop - start] = '\0';
                Value *result = malloc(sizeof(Value));
                result->type = STR_V;
                result->str = result_str;
                return result;
            }

            if (strcmp(op, "error") == 0) {
                if (n != 1) { printf("VEBG error: error needs 1 arg\n"); exit(1); }
                printf("VEBG error: user-error ");
                serialize(arg_vals[0]);
                exit(1);
            }

            printf("VEBG error: unknown primop %s\n", op);
            exit(1);
        }

        // call a function that the user made (not primop)
        if (func->type == CLOSURE_V) {
            if (func->num_params != expr->num_args) {
                printf("VEBG error: wrong number of arguments\n");
                exit(1);
            }
            Env *new_env = func->closure_env;
            for (int i = 0; i < func->num_params; i++) {
                new_env = env_extend(new_env, func->params[i], arg_vals[i]);
            }
            return interp(func->body, new_env);
        }

        printf("VEBG error: tried to call a non-function\n");
        exit(1);
    }

    printf("VEBG error: unknown expression type\n");
    exit(1);
}

// top level environment with all the builtins so they're always available
Env *top_env(void) {
    Env *env = NULL;

    char *ops[] = {"+", "-", "*", "/", "<=", "equal?", "strlen", "substring", "error"};
    for (int i = 0; i < 9; i++) {
        Value *v = malloc(sizeof(Value));
        v->type = PRIMOP_V;
        v->str = ops[i];
        env = env_extend(env, ops[i], v);
    }

    Value *t = malloc(sizeof(Value));
    t->type = BOOL_V;
    t->boolean = 1;
    env = env_extend(env, "true", t);

    Value *f = malloc(sizeof(Value));
    f->type = BOOL_V;
    f->boolean = 0;
    env = env_extend(env, "false", f);

    return env;
}

void top_interp(ExprC *expr) {
    serialize(interp(expr, top_env()));
}

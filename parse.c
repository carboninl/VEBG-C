#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "list.h"
#include "parse.h"

static bool isNum(char *num) {
    while (*num != '\0') {
        if (!isdigit((unsigned char)*num)) {
            return false;
        }
        num++;
    }
    return true;
}

static ExprC *make_num(double n) {
    ExprC *e = calloc(1, sizeof(ExprC));
    e->type = NUM_C;
    e->num = n;
    return e;
}

static ExprC *make_bool(int b) {
    ExprC *e = calloc(1, sizeof(ExprC));
    e->type = BOOL_C;
    e->boolean = b;
    return e;
}

static ExprC *make_str(char *s) {
    ExprC *e = calloc(1, sizeof(ExprC));
    e->type = STR_C;
    e->str = strdup(s);
    return e;
}

static ExprC *make_id(char *s) {
    ExprC *e = calloc(1, sizeof(ExprC));
    e->type = ID_C;
    e->str = strdup(s);
    return e;
}

static ExprC *parseItem(Item *item);
static ExprC *parseList(List *exp);

static ExprC *parseItem(Item *item) {
    if (item == NULL) {
        return NULL;
    }

    if (item->type == ITEM_STRING) {
        char *s = item->value.string;

        if (isNum(s)) {
            return make_num(atof(s));
        }

        if (strcmp(s, "true") == 0) {
            return make_bool(1);
        }

        if (strcmp(s, "false") == 0) {
            return make_bool(0);
        }

        if (s[0] == '"' && s[strlen(s) - 1] == '"') {
            return make_str(s);
        }

        return make_id(s);
    }

    if (item->type == ITEM_LIST) {
        return parseList(item->value.list);
    }

    return NULL;
}

static ExprC *parseList(List *exp) {
    if (exp == NULL || exp->count == 0) {
        return NULL;
    }

    // if expression: (if cond then else)
    if (exp->count == 4 &&
        exp->items[0].type == ITEM_STRING &&
        strcmp(exp->items[0].value.string, "if") == 0) {

        ExprC *e = calloc(1, sizeof(ExprC));
        e->type = IF_C;

        e->cond = parseItem(&exp->items[1]);
        e->then_branch = parseItem(&exp->items[2]);
        e->else_branch = parseItem(&exp->items[3]);

        return e;
    }

    // lambda expression: (fn (x y) -> body)
    if (exp->count == 4 &&
        exp->items[0].type == ITEM_STRING &&
        strcmp(exp->items[0].value.string, "fn") == 0 &&
        exp->items[1].type == ITEM_LIST &&
        exp->items[2].type == ITEM_STRING &&
        strcmp(exp->items[2].value.string, "->") == 0) {

        List *params = exp->items[1].value.list;

        ExprC *e = calloc(1, sizeof(ExprC));
        e->type = LAM_C;

        e->num_params = params->count;
        e->params = malloc(sizeof(char *) * e->num_params);

        for (int i = 0; i < params->count; i++) {
            if (params->items[i].type != ITEM_STRING) {
                free(e->params);
                free(e);
                return NULL;
            }

            e->params[i] = strdup(params->items[i].value.string);
        }

        e->body = parseItem(&exp->items[3]);

        return e;
    }

    // given expression: {given {{x = e1} {y = e2}} do body}
    if (exp->count == 4 &&
        exp->items[0].type == ITEM_STRING &&
        strcmp(exp->items[0].value.string, "given") == 0 &&
        exp->items[1].type == ITEM_LIST &&
        exp->items[2].type == ITEM_STRING &&
        strcmp(exp->items[2].value.string, "do") == 0) {

        List *bindings = exp->items[1].value.list;

        char **names = malloc(sizeof(char *) * bindings->count);
        ExprC **values = malloc(sizeof(ExprC *) * bindings->count);

        for (int i = 0; i < bindings->count; i++) {
            if (bindings->items[i].type != ITEM_LIST) {
                free(names);
                free(values);
                return NULL;
            }

            List *binding = bindings->items[i].value.list;

            if (binding->count != 3 ||
                binding->items[0].type != ITEM_STRING ||
                binding->items[1].type != ITEM_STRING ||
                strcmp(binding->items[1].value.string, "=") != 0) {
                free(names);
                free(values);
                return NULL;
            }

            names[i] = strdup(binding->items[0].value.string);
            values[i] = parseItem(&binding->items[2]);

        }

        ExprC *body = parseItem(&exp->items[3]);

        return new_given(names, values, bindings->count, body);
    }

    // function application: (func arg1 arg2 ...)
    ExprC *e = calloc(1, sizeof(ExprC));
    e->type = APP_C;

    e->func = parseItem(&exp->items[0]);

    e->num_args = exp->count - 1;
    e->args = malloc(sizeof(ExprC *) * e->num_args);

    for (int i = 0; i < e->num_args; i++) {
        e->args[i] = parseItem(&exp->items[i + 1]);
    }

    return e;
}

ExprC *parse(List *exp) {
    return parseList(exp);
}

List *tokenize(const char *str, const char **endptr) {
    List *list = create_list();

    if (*str == '{') {
        str++;

        char token[20];
        char *p = token;
        bool inToken = false;

        while (*str != '}') {
            switch (*str) {
                case ' ':
                    if (inToken) {
                        inToken = false;
                        *p = 0;
                        add_string(list, token);
                        p = token;
                    }
                    str++;
                    break;

                case '{': {
                    const char *end;
                    add_list(list, tokenize(str, &end));
                    str = end;
                    break;
                }

                default:
                    inToken = true;
                    *p = *str;
                    p++;
                    str++;
            }
        }

        if (inToken) {
            *p = 0;
            add_string(list, token);
        }
        str++;
        *endptr = str;
        return list;
    } else {
        printf("VEBG-C Error: Expected opening bracket {\n");
        *endptr = str;
        return NULL;
    }
}

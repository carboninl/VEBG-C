#ifndef PARSE_H
#define PARSE_H

typedef struct ExprC ExprC;
typedef struct List List;

ExprC *parse(List *exp);
List *tokenize(const char *str, const char **endptr);

#endif

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

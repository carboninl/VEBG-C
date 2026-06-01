#include "list.h"

int main(){
    // {fn (x y z) -> {+ 1 2}}
    List *exp = create_list();
    add_string(exp, "fn");

    List *args = create_list();
    add_string(args, "x");
    add_string(args, "y");
    add_string(args, "z");
    add_list(exp, args);

    add_string(exp, "->");

    List *body = create_list();
    add_string(body, "+");
    add_string(body, "1");
    add_string(body, "2");
    add_list(exp, body);

    print_list(exp, 0);
}
#include <string.h>
#include <stdbool.h> 
#include <stdlib.h>
#include <stdio.h>   

#include "list.h"

List *tokenize(const char *str, const char **endptr){
    List *list = create_list();

    if (*str == '{'){
        str++; // skip opening {

        char token[20];
        char* p = token;
        bool inToken = false;

        while (*str != '}'){ // until we reach the closing bracket...
            switch (*str){
                case ' ':   // if space, add the completed token
                    if (inToken) {
                        inToken = false;
                        *p = 0;
                        add_string(list, token);
                        p = token;
                    }
                    str++;
                    break;
                
                case '{': { // if opening bracket, recursively call tokenize
                    const char *end;
                    add_list(list, tokenize(str, &end));
                    str = end; // go to where we ended
                    break;
                }

                default: // otherwise, continue to build token
                    inToken = true;
                    *p = *str;
                    p++;
                    str++;
            }
        }

        if (inToken){ // finish final token if necessary
            *p = 0;
            add_string(list, token);
        }
        str++;              // move past closing }
        *endptr = str;      // return updated pointer
        return list;
    } else {
        printf("VEBG-C Error: Expected opening bracket {\n");
        *endptr = str;
        return NULL;
    }
}

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

    //print_list(exp, 0);

    const char *end;
    print_list(tokenize("{fn {x y z} -> {+ 1 2}}", &end), 0);
    print_list(tokenize("{+ 1 2}", &end), 0);
    print_list(tokenize("{given {{a = 1}{b = 2} do {* a b}}}", &end), 0);
    print_list(tokenize("{}", &end), 0);

}
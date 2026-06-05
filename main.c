#include "interp.h"
#include "parse.h"

int main(){
    const char *end;
    top_interp(parse(tokenize("{{fn {x} -> {+ x 1}} 9}", &end)));
}

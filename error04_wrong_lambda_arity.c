#include "parse.h"
#include "interp.h"

int main(void) {
    const char *end;
    top_interp(parse(tokenize("{{fn {x y} -> {+ x y}} 1}", &end)));
    return 0;
}
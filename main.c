#include "interp.h"
#include "parse.h"

int main(void) {
    const char *end;
    top_interp(parse(tokenize("{{fn {x} -> {+ x 1}} 9}", &end)));
}
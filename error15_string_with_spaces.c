#include "parse.h"
#include "interp.h"

int main(void) {
    const char *end;
    top_interp(parse(tokenize("{\"hello world\"}", &end)));
    return 0;
}
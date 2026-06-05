#include "parse.h"
#include "interp.h"

int main(void) {
    const char *end;
    top_interp(parse(tokenize("{if 0 1 2}", &end)));
    return 0;
}
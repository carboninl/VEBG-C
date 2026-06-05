#include "parse.h"
#include "interp.h"

int main(void) {
    const char *end;
    top_interp(parse(tokenize("{given {{x = 1} {x = 2}} do x}", &end)));
    return 0;
}
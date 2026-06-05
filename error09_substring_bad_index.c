#include "parse.h"
#include "interp.h"

int main(void) {
    const char *end;
    top_interp(parse(tokenize("{substring \"abc\" 3 1}", &end)));
    return 0;
}
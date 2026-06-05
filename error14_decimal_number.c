#include "parse.h"
#include "interp.h"

int main(void) {
    const char *end;
    top_interp(parse(tokenize("{+ 1.5 2}", &end)));
    return 0;
}
#include "parse.h"
#include "interp.h"

int main(void) {
    const char *end;
    top_interp(parse(tokenize("{/ 10 0}", &end)));
    return 0;
}
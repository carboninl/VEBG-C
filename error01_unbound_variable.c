/* compile and run all errors:
for f in error*.c; do
    gcc -Wall -Wextra -pedantic "$f" parse.c interp.c list.c -o "${f%.c}"
done
for f in error*; do
    if [[ -x "$f" && "$f" != *.c ]]; then
        echo "===================================="
        echo "RUNNING: $f"
        echo "===================================="

        ./"$f"

        echo "EXIT CODE: $?"
        echo
    fi
done
*/

/* errors 1-10 are runtime errors */
/* errors 11-16 are parser errors */
/* exit code 139 = segfault */


#include "parse.h"
#include "interp.h"

int main(void) {
    const char *end;
    top_interp(parse(tokenize("{x}", &end)));
    return 0;
}
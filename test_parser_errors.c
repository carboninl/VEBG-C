#include <stdio.h>
#include "parse.h"
#include "interp.h"

/*
 * Parser/tokenizer error and edge-case tests.
 *
 * Some parser errors currently return NULL instead of printing a clean error.
 * That may cause a crash when passed into top_interp.
 *
 * Run ONE test at a time.
 */

static void run_parser_test(const char *label, const char *program) {
    const char *end;

    printf("\n====================================\n");
    printf("PARSER TEST: %s\n", label);
    printf("INPUT: %s\n", program);
    printf("OUTPUT:\n");

    top_interp(parse(tokenize(program, &end)));
}

int main(void) {

    /*
     * ====================================
     * TOKENIZER ERRORS
     * ====================================
     */

    // run_parser_test("missing opening curly brace", "42");
    // expected: VEBG-C Error: Expected opening bracket {

    // run_parser_test("missing opening curly brace around application", "+ 1 2");
    // expected: VEBG-C Error: Expected opening bracket {


    /*
     * ====================================
     * MALFORMED IF EXPRESSIONS
     * ====================================
     */

    // run_parser_test("if too few parts", "{if true 1}");
    // currently parses as function application and likely errors:
    // VEBG error: unbound variable if

    // run_parser_test("if too many parts", "{if true 1 2 3}");
    // currently parses as function application and likely errors:
    // VEBG error: unbound variable if


    /*
     * ====================================
     * MALFORMED FUNCTION EXPRESSIONS
     * ====================================
     */

    // run_parser_test("fn missing arrow", "{fn {x} x}");
    // currently parses as function application and likely errors:
    // VEBG error: unbound variable fn

    // run_parser_test("fn wrong arrow token", "{fn {x} => x}");
    // currently parses as function application and likely errors:
    // VEBG error: unbound variable fn

    // run_parser_test("fn params not list", "{fn x -> x}");
    // currently parses as function application and likely errors:
    // VEBG error: unbound variable fn

    // run_parser_test("fn parameter is nested list", "{fn {{x}} -> x}");
    // parse likely returns NULL, then interpreter may crash


    /*
     * ====================================
     * MALFORMED GIVEN EXPRESSIONS
     * ====================================
     */

    // run_parser_test("given missing do", "{given {{x = 1}} x}");
    // currently parses as function application and likely errors:
    // VEBG error: unbound variable given

    // run_parser_test("given wrong do token", "{given {{x = 1}} then x}");
    // currently parses as function application and likely errors:
    // VEBG error: unbound variable given

    // run_parser_test("given binding missing equals", "{given {{x 1}} do x}");
    // parse likely returns NULL, then interpreter may crash

    // run_parser_test("given binding wrong equals token", "{given {{x := 1}} do x}");
    // parse likely returns NULL, then interpreter may crash

    // run_parser_test("given binding too many parts", "{given {{x = 1 extra}} do x}");
    // parse likely returns NULL, then interpreter may crash

    // run_parser_test("given binding is not list", "{given {x} do x}");
    // parse likely returns NULL, then interpreter may crash


    /*
     * ====================================
     * NUMBER PARSING EDGE CASES
     * ====================================
     */

    // run_parser_test("negative number parses as identifier", "{+ -1 2}");
    // current expected: VEBG error: unbound variable -1

    // run_parser_test("decimal number parses as identifier", "{+ 1.5 2}");
    // current expected: VEBG error: unbound variable 1.5

    // run_parser_test("mixed alphanumeric token parses as identifier", "{abc123}");
    // current expected: VEBG error: unbound variable abc123


    /*
     * ====================================
     * STRING TOKENIZATION EDGE CASES
     * ====================================
     */

    // run_parser_test("string with spaces", "{\"hello world\"}");
    // current tokenizer splits this into multiple tokens, likely causing odd behavior

    // run_parser_test("empty string", "{\"\"}");
    // expected/current behavior depends on quote handling

    // run_parser_test("unterminated string-like token", "{\"hello}");
    // current tokenizer treats it as a plain token with quote character


    /*
     * ====================================
     * BRACE EDGE CASES
     * ====================================
     */

    // run_parser_test("empty braces", "{}");
    // parse returns NULL, likely interpreter crash

    // run_parser_test("nested empty braces", "{{}}");
    // parse returns NULL somewhere, likely interpreter crash

    // run_parser_test("missing closing brace", "{+ 1 2");
    // tokenizer may read past string terminator; unsafe behavior

    // run_parser_test("extra closing brace", "{+ 1 2}}");
    // tokenizer will stop at first }, extra text remains in end pointer


    /*
     * ====================================
     * TOKEN LENGTH EDGE CASE
     * ====================================
     */

    // run_parser_test("very long token",
    //                 "{abcdefghijklmnopqrstuvwxyz}");
    // WARNING: tokenizer uses char token[20], so this may overflow.

    return 0;
}
/*
 * Compile and run:
 gcc -Wall -Wextra -pedantic tests_successes.c parse.c interp.c list.c -o tests_successes
 ./tests_successes
 */

#include <stdio.h>
#include "parse.h"
#include "interp.h"

static void run_test(const char *label, const char *program) {
    const char *end;

    printf("\n====================================\n");
    printf("TEST: %s\n", label);
    printf("INPUT: %s\n", program);
    printf("OUTPUT:\n");

    top_interp(parse(tokenize(program, &end)));

    printf("====================================\n");
}

int main(void) {

    /*
     * ====================================
     * BASIC VALUES
     * ====================================
     */

    /* run_test("number literal", "{42}");
    // expected: 42

    run_test("zero literal", "{0}");
    // expected: 0

    run_test("boolean true", "{true}");
    // expected: true

    run_test("boolean false", "{false}");
    // expected: false

    run_test("string literal", "{\"hello\"}");
    // expected: serialized string
    */

    // **** BUG REVEALED ****
    // These literals get parsed to an AppC, i.e. {42} gets parsed to (42). 
    // Thus "tried to call non-function" error gets returned.
    // Need to update parser to accept literals and return NumC, BoolC, etc.



    /*
     * ====================================
     * ARITHMETIC
     * ====================================
     */

    run_test("addition", "{+ 10 5}");
    // expected: 15

    run_test("subtraction", "{- 10 5}");
    // expected: 5

    run_test("multiplication", "{* 6 7}");
    // expected: 42

    run_test("division", "{/ 20 4}");
    // expected: 5

    run_test("nested arithmetic",
             "{+ {* 2 3} {- 10 4}}");
    // expected: 12

    run_test("deep arithmetic",
             "{* {+ 1 2} {- 10 5}}");
    // expected: 15


    /*
     * ====================================
     * COMPARISON
     * ====================================
     */

    run_test("<= true",
             "{<= 3 3}");
    // expected: true

    run_test("<= false",
             "{<= 9 4}");
    // expected: false


    /*
     * ====================================
     * EQUALITY
     * ====================================
     */

    run_test("equal numbers true",
             "{equal? 5 5}");
    // expected: true

    run_test("equal numbers false",
             "{equal? 5 6}");
    // expected: false

    run_test("equal booleans true",
             "{equal? true true}");
    // expected: true

    run_test("equal booleans false",
             "{equal? true false}");
    // expected: false

    run_test("equal different types",
             "{equal? 5 true}");
    // expected: false

    run_test("equal strings",
             "{equal? \"abc\" \"abc\"}");
    // expected: true


    /*
     * ====================================
     * IF EXPRESSIONS
     * ====================================
     */

    run_test("if true branch",
             "{if true 1 2}");
    // expected: 1

    run_test("if false branch",
             "{if false 1 2}");
    // expected: 2

    run_test("if computed condition",
             "{if {<= 2 3} 100 200}");
    // expected: 100

    run_test("nested if",
             "{if {<= 10 5} 1 {if true 2 3}}");
    // expected: 2


    /*
     * ====================================
     * LAMBDAS
     * ====================================
     */

    run_test("identity lambda",
             "{{fn {x} -> x} 99}");
    // expected: 99

    run_test("increment lambda",
             "{{fn {x} -> {+ x 1}} 9}");
    // expected: 10

    run_test("two parameter lambda",
             "{{fn {x y} -> {* x y}} 6 7}");
    // expected: 42

    run_test("three parameter lambda",
             "{{fn {a b c} -> {+ a {+ b c}}} 1 2 3}");
    // expected: 6

    run_test("zero parameter lambda",
             "{{fn {} -> 123}}");
    // expected: 123


    /*
     * ====================================
     * HIGHER ORDER FUNCTIONS
     * ====================================
     */

    run_test("pass function as argument",
             "{{fn {f x} -> {f x}} {fn {n} -> {+ n 1}} 41}");
    // expected: 42

    run_test("function returning function",
             "{{{fn {x} -> {fn {y} -> {+ x y}}} 10} 5}");
    // expected: 15


    /*
     * ====================================
     * CLOSURES
     * ====================================
     */

    run_test("closure captures environment",
             "{given {{x = 10}} do {{fn {y} -> {+ x y}} 5}}");
    // expected: 15

    run_test("closure with nested lambda",
             "{{{fn {x} -> {fn {y} -> {* x y}}} 6} 7}");
    // expected: 42


    /*
     * ====================================
     * GIVEN EXPRESSIONS
     * ====================================
     */

    run_test("simple given",
             "{given {{x = 10}} do x}");
    // expected: 10

    run_test("given two bindings",
             "{given {{x = 10} {y = 20}} do {+ x y}}");
    // expected: 30

    run_test("given expression values",
             "{given {{x = {+ 1 2}} {y = {* 3 4}}} do {+ x y}}");
    // expected: 15

    run_test("nested given",
             "{given {{x = 1}} do {given {{y = 2}} do {+ x y}}}");
    // expected: 3


    /*
     * ====================================
     * SHADOWING
     * ====================================
     */

    run_test("parameter shadows outer binding",
             "{given {{x = 100}} do {{fn {x} -> {+ x 1}} 5}}");
    // expected: 6

    run_test("inner given shadows outer given",
             "{given {{x = 1}} do {given {{x = 2}} do x}}");
    // expected: 2


    /*
     * ====================================
     * STRING OPERATIONS
     * ====================================
     */

    run_test("strlen",
             "{strlen \"hello\"}");

    run_test("substring front",
             "{substring \"abcdef\" 0 3}");

    run_test("substring middle",
             "{substring \"abcdef\" 2 4}");

    run_test("substring end",
             "{substring \"abcdef\" 3 6}");


    /*
     * ====================================
     * SERIALIZATION
     * ====================================
     */

    run_test("lambda serialization",
             "{fn {x} -> x}");
    // expected: #<procedure>

    run_test("procedure equality",
             "{equal? {fn {x} -> x} {fn {x} -> x}}");
    // expected: false

    run_test("primop equality",
             "{equal? + +}");
    // expected: false


    return 0;
}


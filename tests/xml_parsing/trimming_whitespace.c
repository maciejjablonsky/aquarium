#include <criterion/criterion.h>
#include "../../src/config_parser/config_parser.c"

TestSuite(trim_whitespace);
Test(trim_whitespace, no_trim) {
    cr_expect_str_eq(trim_outer_whitespace("no_whitespace"), "no_whitespace");
    cr_expect_str_eq(trim_outer_whitespace(""), "");
}

Test(trim_whitespace, trim_front) {
    cr_expect_str_eq(trim_outer_whitespace("  whitespace"), "whitespace");
    cr_expect_str_eq(trim_outer_whitespace("\n\t \fwhitespace"), "whitespace");
}

Test(trim_whitespace, trim_back) {
    cr_expect_str_eq(trim_outer_whitespace("whitespace  "), "whitespace");
    cr_expect_str_eq(trim_outer_whitespace("whitespace\n\t \f"), "whitespace");
}
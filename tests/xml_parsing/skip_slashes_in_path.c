#include <criterion/criterion.h>

#include "../../src/config_parser/config_parser.c"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpointer-sign"
TestSuite(skip_slashes);

Test(skip_slashes, zero_slashes) {
    cr_expect_eq(skip_slashes("node"), 0);
    cr_expect_eq(skip_slashes("node/"), 0);
    cr_expect_eq(skip_slashes("a//"), 0);
}

Test(skip_slashes, one_slash) {
    cr_expect_eq(skip_slashes("/node"), 1);
    cr_expect_eq(skip_slashes("/node/"), 1);
}

Test(skip_slashes, only_slashes) {
    cr_expect_eq(skip_slashes("////"), 4);
}

#pragma clang diagnostic pop
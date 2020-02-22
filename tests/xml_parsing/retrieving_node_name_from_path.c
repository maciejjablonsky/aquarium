#include <criterion/criterion.h>
#include "../../src/config_parser/config_parser.c"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpointer-sign"

TestSuite(node_name_from_path);

Test(node_name_from_path, root_path) {
    cr_assert_str_eq(_next_node_name("/"), "");
}

Test(node_name_from_path, empty_path) {
    cr_assert_str_eq(_next_node_name(""), "");
}

Test(node_name_from_path, node_with_child) {
    cr_expect_str_eq(_next_node_name("/parent/child"), "parent");
    cr_expect_str_eq(_next_node_name("parent/child"), "parent");
    cr_expect_str_eq(_next_node_name("parent/child/"), "parent");
}

Test(node_name_from_path, node_without_child) {
    cr_expect_str_eq(_next_node_name("/node"), "node");
    cr_expect_str_eq(_next_node_name("node"), "node");
    cr_expect_str_eq(_next_node_name("node/"), "node");
}
#pragma clang diagnostic pop
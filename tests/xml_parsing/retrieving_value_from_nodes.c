#include <criterion/criterion.h>
#include "../../src/config_parser/config_parser.c"

config_parser_t * parser;
void setup(void) {
    parser = new_config_parser("resources/testable_xml_file.xml");
    cr_assert_not_null(parser);
}

void teardown(void) {

}

TestSuite(retrieve_value);

Test(retrieve_value, empty_node, .init = setup, .fini=teardown) {
    cr_assert_null(config_xml_get_value_with_xpath(parser, "parent/fifth_child"));
}

Test(retrieve_value, whitespace_only, .init=setup, .fini=teardown) {
    cr_assert_null(config_xml_get_value_with_xpath(parser, "parent/fourth_child"));

}

Test(retrieve_value, node_without_content_with_children, .init=setup, .fini=teardown) {
    cr_assert_null(config_xml_get_value_with_xpath(parser, "parent/first_child"));
}

Test(retrieve_value, node_with_content, .init=setup, .fini=teardown) {
    cr_assert_str_eq(config_xml_get_value_with_xpath(parser, "//first_grandchild"), "content1");
}

Test(retrieve_value, node_with_content_and_children, .init=setup, .fini=teardown) {
    cr_assert_str_eq(config_xml_get_value_with_xpath(parser, "//second_child"), "content5");
    cr_assert_str_eq(config_xml_get_value_with_xpath(parser, "//third_child"), "content6");
}


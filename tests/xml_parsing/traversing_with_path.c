#include <criterion/criterion.h>
#include <criterion/options.h>
#include "../../src/config_parser/config_parser.c"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpointer-sign"
config_parser_t * config_parser;

void setup(void) {
    config_parser = new_config_parser("resources/testable_xml_file.xml");
    cr_assert_not_null(config_parser);
}

void teardown(void) {

}

TestSuite(traversing_with_path);

Test(traversing_with_path, go_to_root, .init = setup, .fini = teardown) {
    cr_expect(config_xml_eval_xpath(config_parser, "/"));
    cr_expect_null(get_current_node_name(config_parser));
    cr_assert(config_xml_eval_xpath(config_parser, "//first_grandchild"));
    cr_expect(config_xml_eval_xpath(config_parser, "/"));
    cr_expect_null(get_current_node_name(config_parser));
}

Test(traversing_with_path, go_to_child, .init = setup, .fini = teardown) {
    cr_expect(config_xml_eval_xpath(config_parser, "first_child"));
    cr_assert_str_eq(get_current_node_name(config_parser), "first_child");

    cr_expect(config_xml_eval_xpath(config_parser, "first_grandchild"));
    cr_assert_str_eq(get_current_node_name(config_parser), "first_grandchild");
}

Test(traversing_with_path, go_to_parent, .init = setup, .fini=teardown) {
    cr_assert(config_xml_eval_xpath(config_parser, "//first_grandchild"));

    cr_expect(config_xml_eval_xpath(config_parser, ".."));
    cr_assert_str_eq(get_current_node_name(config_parser), "first_child");

    cr_assert(config_xml_eval_xpath(config_parser, "//first_grandchild"));

    cr_expect(config_xml_eval_xpath(config_parser, "../.."));
    cr_expect_str_eq(get_current_node_name(config_parser), "parent");
}

Test(traversing_with_path, go_to_cousin, .init = setup, .fini=teardown) {
    cr_assert(config_xml_eval_xpath(config_parser, "//first_grandchild"));
    cr_expect(config_xml_eval_xpath(config_parser, "../../second_child/third_grandchild"));
    cr_assert_str_eq(get_current_node_name(config_parser), "third_grandchild");
}

Test(traversing_with_path, stay_at_the_same_node, .init= setup, .fini = teardown) {
    cr_assert(config_xml_eval_xpath(config_parser, "//first_grandchild"));
    cr_assert(config_xml_eval_xpath(config_parser, "."));
    cr_expect_str_eq(get_current_node_name(config_parser), "first_grandchild");
}

TestSuite(node_not_found);

Test(node_not_found, not_existing_child, .init=setup, .fini=teardown) {
    xmlNode * current = config_parser->context->node;
    cr_assert_not(config_xml_eval_xpath(config_parser, "not_existing_child"));
    cr_assert_eq(config_parser->context->node, current);
}

Test(node_not_found, not_existing_cousin, .init=setup, .fini=teardown) {
    xmlNode * current = config_parser->context->node;
    cr_assert_eq(config_xml_eval_xpath(config_parser, "../not_existing_cousin"), false);
    cr_assert_eq(config_parser->context->node, current);
}
#pragma clang diagnostic pop
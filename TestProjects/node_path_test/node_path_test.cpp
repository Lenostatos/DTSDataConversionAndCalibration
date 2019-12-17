#include "../../DTS_DataProcessingRoutine/node_path.hpp"

#include <fstream>

#include "UnitTest++.h"
#include "rapidxml.hpp"

#include "../../DTS_DataProcessingRoutine/globals.hpp"

SUITE(invalid_paths)
{
    TEST(no_root_node)
    {
        CHECK_THROW(node_path("foo"), std::invalid_argument);
    }

    // not yet implemented
//    TEST(invalid_node_name)
//    {
//        const int result = node_path::count_path_elements("/foo  baa");
//        CHECK_EQUAL(1, result);
//    }
}


class path_test_set
{
public:
    std::vector<std::string> path_strings;

    std::vector<node_path> node_paths;

    std::vector<int> element_counts_expected;
    std::vector<int> element_counts_actual;

    std::vector<std::vector<std::string>> node_names_expected;
    std::vector<std::vector<std::string>> node_names_actual;

    path_test_set()
    {
        path_strings = {
        "",
        "/",
        "/root",
        "/root/",
        "/root/child",
        "/root/child/",
        "//child/grandchild/",
        "/root//grandchild/",
        "/root/child//",
        "////"};

        // initialize the node_path objects
        for (size_t i = 0; i < path_strings.size(); ++i)
            { node_paths.emplace_back(node_path(path_strings[i])); }

        // element counts
        element_counts_expected = {0, 1, 1, 2, 2, 3, 4, 4, 4, 4};
        for (const node_path & n_path : node_paths)
            { element_counts_actual.push_back(n_path.elements_.size()); }

        // node names
        node_names_expected = {
        {""},
        {"root"},
        {"root", ""},
        {"root", "child"},
        {"root", "child", ""},
        {"", "child", "grandchild", ""},
        {"root", "", "grandchild", ""},
        {"root", "child", "", ""},
        {"", "", "", ""}};
        auto n_path_iter = begin(node_paths);
        ++n_path_iter;
        while (n_path_iter != end(node_paths))
        {
            std::vector<std::string> temp_names;

            for (const auto & n_path_elem : n_path_iter->elements_)
                { temp_names.push_back(n_path_elem); }

            node_names_actual.push_back(temp_names);

            ++n_path_iter;
        }
    }
};


SUITE(count_path_elements)
{
//    TEST(test_CHECK_ARRAY_EQUAL_output)
//    {
//        std::array<std::string, 3> arr1 = {"foo", "ba", "baa"};
//        std::array<std::string, 3> arr2 = {"foo", "bu", "baa"};
//        CHECK_ARRAY_EQUAL(arr1, arr2, 3);
//    }

    TEST_FIXTURE(path_test_set, element_counting)
    {
        REQUIRE CHECK(element_counts_actual.size() == element_counts_expected.size());
        CHECK_ARRAY_EQUAL(element_counts_expected, element_counts_actual, element_counts_actual.size());
    }
}


SUITE(extract_path_elements)
{
    TEST_FIXTURE(path_test_set, empty_path)
    {
        CHECK(node_paths[0].elements_.empty());
    }

    TEST_FIXTURE(path_test_set, other_paths)
    {
        REQUIRE CHECK(node_names_actual.size() == (node_names_expected.size()));
        for (size_t i = 0; i < node_names_actual.size(); ++i)
        {
            REQUIRE CHECK(node_names_actual[i].size() == (node_names_expected[i].size()));
            CHECK_ARRAY_EQUAL(node_names_expected[i], node_names_actual[i], node_names_actual[i].size());
        }
    }
}

class real_xml_file
{
private:
    const std::string xml_string; // The string object has to persist for the lifetime of the XML-document!

public:
    rapidxml::xml_document<> xml_parsed;

    real_xml_file() : xml_string(read_in_file("./Bsp_XML_files/channel 1_20170904182938279.xml"))
    {
        xml_parsed.parse<rapidxml::parse_non_destructive | rapidxml::parse_trim_whitespace> (const_cast<char *>(xml_string.c_str()));
        // the const_cast is okay in this case because the rapidxml::parse_non_destructive flag is set which promises to not alter the content of xml_file_string
    }
};

class made_up_xml_file
{
private:
    const std::string xml_string; // The string object has to persist for the lifetime of the XML-document!

public:
    rapidxml::xml_document<> xml_parsed;

    made_up_xml_file() : xml_string(read_in_file("./Bsp_XML_files/made_up.xml"))
    {
        xml_parsed.parse<rapidxml::parse_non_destructive | rapidxml::parse_trim_whitespace> (const_cast<char *>(xml_string.c_str()));
        // the const_cast is okay in this case because the rapidxml::parse_non_destructive flag is set which promises to not alter the content of xml_file_string
    }
};

SUITE(get_rapidxml_node)
{
    SUITE(real_xml_file)
    {
        TEST_FIXTURE(real_xml_file, get_root_node)
        {
            node_path npath("/logs");
            const rapidxml::xml_node<> * xml_n = npath.find_node(xml_parsed);

            CHECK_EQUAL("logs", std::string(xml_n->name(), xml_n->name_size()));
        }
    }

    SUITE(made_up_xml_file)
    {
        TEST_FIXTURE(made_up_xml_file, get_root_node)
        {
            node_path npath("/root");
            const rapidxml::xml_node<> * xml_n = npath.find_node(xml_parsed);

            CHECK_EQUAL("root", std::string(xml_n->name(), xml_n->name_size()));
        }

        TEST_FIXTURE(made_up_xml_file, get_nameless_child_node)
        {
            node_path npath("/root/");
            const rapidxml::xml_node<> * xml_n = npath.find_node(xml_parsed);

            CHECK_EQUAL("child", std::string(xml_n->name(), xml_n->name_size()));
        }

        TEST_FIXTURE(made_up_xml_file, get_child_node)
        {
            node_path npath("/root/child");
            const rapidxml::xml_node<> * xml_n = npath.find_node(xml_parsed);

            CHECK_EQUAL("child", std::string(xml_n->name(), xml_n->name_size()));
        }
    }
}

#include "../../DTS_DataProcessingRoutine/user_configuration.hpp"

#include "UnitTest++.h"

#include "../../DTS_DataProcessingRoutine/filesystem_utilities.hpp"
#include "../../DTS_DataProcessingRoutine/my_exceptions.hpp"


SUITE (content_extraction_)
{
    SUITE (extract_single_tag_values_)
    {
        using namespace user_configuration::extraction_utilities;

        TEST (simple_tag_value)
        {
            CHECK_EQUAL("value", extract_tag_value("tag", "tag=value", '#'));
        }

        TEST (tag_value_with_whitespaces)
        {
            CHECK_EQUAL("value", extract_tag_value("tag", "tag = value", '#'));
            CHECK_EQUAL("value", extract_tag_value("tag", " tag = value", '#'));
            CHECK_EQUAL("value", extract_tag_value("tag", "\ttag = value", '#'));
            CHECK_EQUAL("value", extract_tag_value("tag", "\ttag=value", '#'));
            CHECK_EQUAL("value", extract_tag_value("tag", "\ttag= value", '#'));
            CHECK_EQUAL("value", extract_tag_value("tag", "\ttag =value", '#'));
            CHECK_EQUAL("value", extract_tag_value("tag", "\ttag\t=value", '#'));
        }

        TEST (multiline_situations)
        {
            CHECK_EQUAL("value", extract_tag_value("tag", "\ntag = value", '#'));
            CHECK_EQUAL("value", extract_tag_value("tag", "\n tag = value", '#'));
            CHECK_EQUAL("value", extract_tag_value("tag", "\n\ttag = value", '#'));
            CHECK_EQUAL("value", extract_tag_value("tag", "#\ntag = value", '#'));
            CHECK_EQUAL("value", extract_tag_value("tag", "#\ttag = wrong_value\ntag = value", '#'));
            CHECK_EQUAL("value", extract_tag_value("tag", "#tag = tag\ntag = value", '#'));
            CHECK_EQUAL("value", extract_tag_value("tag", "tag_two = tag\ntag = value", '#'));
            CHECK_EQUAL("value", extract_tag_value("tag", "label = tag\ntag = value", '#'));
            CHECK_EQUAL("value", extract_tag_value("tag", "\r\ntag = value", '#'));
            CHECK_EQUAL("value", extract_tag_value("tag", "tag\ntag = value", '#'));
            CHECK_EQUAL("value", extract_tag_value("tag", "tag \n tag = value", '#'));
        }

        TEST (exception_throwing)
        {
            using namespace my_exceptions;

            CHECK_THROW(extract_tag_value("tag with whitespaces", "\ntag = value", '#'), tag_error);
            CHECK_THROW(extract_tag_value("tag", "\nother_tag = value", '#'), tag_error);
        }
    }

    SUITE (extract_multiple_tags_and_values_)
    {
        class section
        {
        public:
            std::string sec_beg;
            std::string sec_end;

            std::unordered_multimap<std::string, std::string> actual_result;

            section()
            : sec_beg("test_section_begin:"), sec_end(":test_section_end")
            {}

            std::string sectionize(const std::string & section_content)
                { return sec_beg + section_content + sec_end; }
        };

        using namespace user_configuration::extraction_utilities;

        TEST_FIXTURE (section, empty_section)
        {
            CHECK(extract_tags_and_values(sec_beg, sec_end, sectionize("tag=value"), '#').empty());

            CHECK(extract_tags_and_values(sec_beg, sec_end, sectionize(""), '#').empty());
            CHECK(extract_tags_and_values(sec_beg, sec_end, sectionize(" "), '#').empty());
            CHECK(extract_tags_and_values(sec_beg, sec_end, sectionize("\t"), '#').empty());
            CHECK(extract_tags_and_values(sec_beg, sec_end, sectionize("\r"), '#').empty());
            CHECK(extract_tags_and_values(sec_beg, sec_end, sectionize("\r\n"), '#').empty());
            CHECK(extract_tags_and_values(sec_beg, sec_end, sectionize("\r \t \n"), '#').empty());
            CHECK(extract_tags_and_values(sec_beg, sec_end, sectionize("\n"), '#').empty());
            CHECK(extract_tags_and_values(sec_beg, sec_end, sectionize("\n\n"), '#').empty());
            CHECK(extract_tags_and_values(sec_beg, sec_end, sectionize("\n \n"), '#').empty());
            CHECK(extract_tags_and_values(sec_beg, sec_end, sectionize("\n\t\n"), '#').empty());

            CHECK(extract_tags_and_values(sec_beg, sec_end, sectionize("tag = value"), '#').empty());
            CHECK(extract_tags_and_values(sec_beg, sec_end, sectionize("tag=value"), '#').empty());
            CHECK(extract_tags_and_values(sec_beg, sec_end, sectionize(" tag=value"), '#').empty());
            CHECK(extract_tags_and_values(sec_beg, sec_end, sectionize(" tag=value "), '#').empty());
            CHECK(extract_tags_and_values(sec_beg, sec_end, sectionize("\ttag=value "), '#').empty());
            CHECK(extract_tags_and_values(sec_beg, sec_end, sectionize("\ttag=value\t"), '#').empty());

            CHECK(extract_tags_and_values(sec_beg, sec_end, sectionize("tag\n=value"), '#').empty());
            CHECK(extract_tags_and_values(sec_beg, sec_end, sectionize("tag=\nvalue"), '#').empty());

            CHECK(extract_tags_and_values(sec_beg, sec_end, sectionize("\ntag = value"), '#').empty());
            CHECK(extract_tags_and_values(sec_beg, sec_end, sectionize("\n tag = value"), '#').empty());
            CHECK(extract_tags_and_values(sec_beg, sec_end, sectionize("\n\ttag = value"), '#').empty());

            CHECK(extract_tags_and_values(sec_beg, sec_end, sectionize("tag = value\n"), '#').empty());
            CHECK(extract_tags_and_values(sec_beg, sec_end, sectionize("tag = value \n"), '#').empty());
            CHECK(extract_tags_and_values(sec_beg, sec_end, sectionize("tag = value\t\n"), '#').empty());
            CHECK(extract_tags_and_values(sec_beg, sec_end, sectionize("tag = value\n\t"), '#').empty());
        }

        TEST_FIXTURE (section, one_tag_and_value)
        {
            actual_result = extract_tags_and_values(sec_beg, sec_end, sectionize("\ntag=value\n"), '#');
            CHECK(actual_result.find("tag") != actual_result.end());    CHECK_EQUAL("value", actual_result.find("tag")->second);
            actual_result = extract_tags_and_values(sec_beg, sec_end, sectionize("\rtag=value\n"), '#');
            CHECK(actual_result.find("tag") != actual_result.end());    CHECK_EQUAL("value", actual_result.find("tag")->second);
            actual_result = extract_tags_and_values(sec_beg, sec_end, sectionize("\rtag=value\r"), '#');
            CHECK(actual_result.find("tag") != actual_result.end());    CHECK_EQUAL("value", actual_result.find("tag")->second);
            actual_result = extract_tags_and_values(sec_beg, sec_end, sectionize("\r\ntag=value\r\n"), '#');
            CHECK(actual_result.find("tag") != actual_result.end());    CHECK_EQUAL("value", actual_result.find("tag")->second);

            actual_result = extract_tags_and_values(sec_beg, sec_end, sectionize("\r\n tag=value\r\n"), '#');
            CHECK(actual_result.find("tag") != actual_result.end());    CHECK_EQUAL("value", actual_result.find("tag")->second);
            actual_result = extract_tags_and_values(sec_beg, sec_end, sectionize("\r\n tag=value \r\n"), '#');
            CHECK(actual_result.find("tag") != actual_result.end());    CHECK_EQUAL("value", actual_result.find("tag")->second);
            actual_result = extract_tags_and_values(sec_beg, sec_end, sectionize("\r\n tag =value \r\n"), '#');
            CHECK(actual_result.find("tag") != actual_result.end());    CHECK_EQUAL("value", actual_result.find("tag")->second);
            actual_result = extract_tags_and_values(sec_beg, sec_end, sectionize("\r\n tag = value \r\n"), '#');
            CHECK(actual_result.find("tag") != actual_result.end());    CHECK_EQUAL("value", actual_result.find("tag")->second);

            actual_result = extract_tags_and_values(sec_beg, sec_end, sectionize("\r\n\ttag = value \r\n"), '#');
            CHECK(actual_result.find("tag") != actual_result.end());    CHECK_EQUAL("value", actual_result.find("tag")->second);
            actual_result = extract_tags_and_values(sec_beg, sec_end, sectionize("\r\n\ttag\t= value \r\n"), '#');
            CHECK(actual_result.find("tag") != actual_result.end());    CHECK_EQUAL("value", actual_result.find("tag")->second);
            actual_result = extract_tags_and_values(sec_beg, sec_end, sectionize("\r\n\ttag\t=\tvalue\t\r\n"), '#');
            CHECK(actual_result.find("tag") != actual_result.end());    CHECK_EQUAL("value", actual_result.find("tag")->second);

            actual_result = extract_tags_and_values(sec_beg, sec_end, sectionize("\n\n\n\ttag\t=\tvalue\t\n\n\n"), '#');
            CHECK(actual_result.find("tag") != actual_result.end());    CHECK_EQUAL("value", actual_result.find("tag")->second);
        }

        TEST_FIXTURE (section, multiple_tags_and_values)
        {
            actual_result = extract_tags_and_values(sec_beg, sec_end, sectionize("\ntag=value\ntag2=value\n"), '#');
            CHECK(actual_result.find("tag") != actual_result.end());    CHECK_EQUAL("value", actual_result.find("tag")->second);
            CHECK(actual_result.find("tag2") != actual_result.end());    CHECK_EQUAL("value", actual_result.find("tag2")->second);
            actual_result = extract_tags_and_values(sec_beg, sec_end, sectionize("\ntag=value\ntag2=value2\n"), '#');
            CHECK(actual_result.find("tag") != actual_result.end());    CHECK_EQUAL("value", actual_result.find("tag")->second);
            CHECK(actual_result.find("tag2") != actual_result.end());    CHECK_EQUAL("value2", actual_result.find("tag2")->second);

            actual_result = extract_tags_and_values(sec_beg, sec_end, sectionize("\ntag=value\ntag=value\n"), '#');
            CHECK_EQUAL(2, actual_result.count("tag"));
            for (const auto & tag : actual_result)
            {
                CHECK_EQUAL("tag", tag.first);
                CHECK_EQUAL("value", tag.second);
            }

            actual_result = extract_tags_and_values(sec_beg, sec_end, sectionize("\ntag=value\n\ntag2=value2\n"), '#');
            CHECK(actual_result.find("tag") != actual_result.end());    CHECK_EQUAL("value", actual_result.find("tag")->second);
            CHECK(actual_result.find("tag2") != actual_result.end());    CHECK_EQUAL("value2", actual_result.find("tag2")->second);
            actual_result = extract_tags_and_values(sec_beg, sec_end, sectionize("\ntag=value\n \ntag2=value2\n"), '#');
            CHECK(actual_result.find("tag") != actual_result.end());    CHECK_EQUAL("value", actual_result.find("tag")->second);
            CHECK(actual_result.find("tag2") != actual_result.end());    CHECK_EQUAL("value2", actual_result.find("tag2")->second);
            actual_result = extract_tags_and_values(sec_beg, sec_end, sectionize("\ntag=value\n\t\ntag2=value2\n"), '#');
            CHECK(actual_result.find("tag") != actual_result.end());    CHECK_EQUAL("value", actual_result.find("tag")->second);
            CHECK(actual_result.find("tag2") != actual_result.end());    CHECK_EQUAL("value2", actual_result.find("tag2")->second);
            actual_result = extract_tags_and_values(sec_beg, sec_end, sectionize("\ntag=value\n \t\ntag2=value2\n"), '#');
            CHECK(actual_result.find("tag") != actual_result.end());    CHECK_EQUAL("value", actual_result.find("tag")->second);
            CHECK(actual_result.find("tag2") != actual_result.end());    CHECK_EQUAL("value2", actual_result.find("tag2")->second);
            actual_result = extract_tags_and_values(sec_beg, sec_end, sectionize("\ntag=value\n\r\t\ntag2=value2\n"), '#');
            CHECK(actual_result.find("tag") != actual_result.end());    CHECK_EQUAL("value", actual_result.find("tag")->second);
            CHECK(actual_result.find("tag2") != actual_result.end());    CHECK_EQUAL("value2", actual_result.find("tag2")->second);
            actual_result = extract_tags_and_values(sec_beg, sec_end, sectionize("\ntag=value\n\r\n\ntag2=value2\n"), '#');
            CHECK(actual_result.find("tag") != actual_result.end());    CHECK_EQUAL("value", actual_result.find("tag")->second);
            CHECK(actual_result.find("tag2") != actual_result.end());    CHECK_EQUAL("value2", actual_result.find("tag2")->second);
        }

        TEST_FIXTURE (section, exception_throwing)
        {
            CHECK_THROW(extract_tags_and_values(sec_end, sec_beg, sectionize(""), '#'), my_exceptions::tag_error);
            CHECK_THROW(extract_tags_and_values(sec_end, sec_beg, sectionize(" "), '#'), my_exceptions::tag_error);
            CHECK_THROW(extract_tags_and_values(sec_end, sec_beg, sectionize("\t"), '#'), my_exceptions::tag_error);
            CHECK_THROW(extract_tags_and_values(sec_end, sec_beg, sectionize("\n"), '#'), my_exceptions::tag_error);
            CHECK_THROW(extract_tags_and_values(sec_end, sec_beg, sectionize("\r\n"), '#'), my_exceptions::tag_error);
            CHECK_THROW(extract_tags_and_values(sec_end, sec_beg, sectionize("\r\n\n"), '#'), my_exceptions::tag_error);

            CHECK_THROW(extract_tags_and_values(sec_beg, sec_end, sec_beg, '#'), my_exceptions::tag_error);
            CHECK_THROW(extract_tags_and_values(sec_beg, sec_end, sec_beg + "tag = value", '#'), my_exceptions::tag_error);
            CHECK_THROW(extract_tags_and_values(sec_beg, sec_end, sec_beg + "\ntag = value", '#'), my_exceptions::tag_error);
            CHECK_THROW(extract_tags_and_values(sec_beg, sec_end, sec_beg + "\ntag = value\n", '#'), my_exceptions::tag_error);

            CHECK_THROW(extract_tags_and_values(sec_beg, sec_end, "", '#'), my_exceptions::tag_error);
            CHECK_THROW(extract_tags_and_values(sec_beg, sec_end, sec_end, '#'), my_exceptions::tag_error);
            CHECK_THROW(extract_tags_and_values(sec_beg, sec_end, sec_beg, '#'), my_exceptions::tag_error);

            CHECK_THROW(extract_tags_and_values(sec_beg, sec_end, "tag = value" + sec_end, '#'), my_exceptions::tag_error);
            CHECK_THROW(extract_tags_and_values(sec_beg, sec_end, "\ntag = value" + sec_end, '#'), my_exceptions::tag_error);
            CHECK_THROW(extract_tags_and_values(sec_beg, sec_end, "\ntag = value\n" + sec_end, '#'), my_exceptions::tag_error);
        }
    }

    SUITE (extract_)
    {
        using namespace user_configuration;

        TEST (extract_content)
        {
            content test_content = content::extract
            (
                filesystem_utilities::read_file("test_config.txt"),
                '#',

                "input_dir",
                "output_dir",

                "output_file_name",

                "calculate_calibrated_temperatures",

                "time",
                "cold_bath",
                "warm_bath",
                "data",

                "value_order",

                {
                    {"channel", TYPE_INT}
                },
                {
                    std::make_tuple("paths_to_text_nodes:", ":paths_to_text_nodes", TYPE_STRING),
                    std::make_tuple("paths_to_floating_number_nodes:", ":paths_to_floating_number_nodes", TYPE_REAL),
                    std::make_tuple("paths_to_integer_value_nodes:", ":paths_to_integer_value_nodes", TYPE_INT)
                },

                "cold_bath_begin",
                "warm_bath_begin",
                "cold_bath_end",
                "warm_bath_end",

                {},

                {"mapping_tags:", ":mapping_tags"}
            );

            CHECK_EQUAL("test_input_dir", test_content.input_dir);
            CHECK_EQUAL("test_output_dir", test_content.output_dir);

            CHECK_EQUAL("test_output_file_name", test_content.output_file_name);

            CHECK_EQUAL("y", test_content.do_calculations_str);

            CHECK_EQUAL("/logs/log/startDateTimeIndex", test_content.time_node_path_str);
            CHECK_EQUAL("/logs/log/customData/probe2Temperature", test_content.cold_bath_node_path_str);
            CHECK_EQUAL("/logs/log/customData/probe1Temperature", test_content.warm_bath_node_path_str);
            CHECK_EQUAL("/logs/log/logData/", test_content.data_node_path_str);

            CHECK_EQUAL("1,2,3,4", test_content.data_order_str);

            std::map<std::string, std::pair< std::string, node_data_type >> expected_named_node_path_strings_w_types
            ({
                {"channel", {"/logs/log/customData/forwardMeasurementChannel", TYPE_INT}},
                {"duration", {"/logs/log/customData/acquisitionTime", TYPE_REAL}},
                {"fiber_status", {"/logs/log/customData/fibreStatusOk", TYPE_INT}}
            });

            CHECK_EQUAL(expected_named_node_path_strings_w_types.size(), test_content.named_node_path_str_w_types.size());

            for (const auto & expec_n_np_str_w_t : expected_named_node_path_strings_w_types)
            {
                CHECK_EQUAL(1, test_content.named_node_path_str_w_types.count(expec_n_np_str_w_t.first));

                CHECK_EQUAL(expec_n_np_str_w_t.second.first, test_content.named_node_path_str_w_types.at(expec_n_np_str_w_t.first).first);
                CHECK_EQUAL(expec_n_np_str_w_t.second.second, test_content.named_node_path_str_w_types.at(expec_n_np_str_w_t.first).second);
            }

            CHECK_EQUAL("0,0", test_content.cold_bath_begin_mapping_str);
            CHECK_EQUAL("2.3,2.3", test_content.warm_bath_begin_mapping_str);
            CHECK_EQUAL("231.0,231.0", test_content.cold_bath_end_mapping_str);
            CHECK_EQUAL("777,777", test_content.warm_bath_end_mapping_str);

            std::unordered_multimap<std::string, std::string> expected_mapping
            ({
                {"test_mapping","3,6"},
                {"test_mapping_2", "1234.567,1234.891"},
                {"test_mapping_3", "0.000,0.0001"}
            });

            CHECK_EQUAL(test_content.mapping_tags_and_value_str.size(), expected_mapping.size());

            for (const auto & expec_mapping_elem : expected_mapping)
            {
                // Checking for the expected mapping tags to exists exactly once in the actually obtained content
                CHECK_EQUAL(1, test_content.mapping_tags_and_value_str.count(expec_mapping_elem.first));

                // Checking for equality of expected and actually obtained mapping value pairs
                CHECK_EQUAL(expec_mapping_elem.second, test_content.mapping_tags_and_value_str.find(expec_mapping_elem.first)->second);
            }
        }
    }
}

SUITE (content_interpretation_)
{
    using namespace user_configuration;

    class config_content
    {
    public:
        content config_content = content::extract
        (
            filesystem_utilities::read_file("test_config.txt"),
            '#',

            "input_dir",
            "output_dir",

            "output_file_name",

            "calculate_calibrated_temperatures",

            "time",
            "cold_bath",
            "warm_bath",
            "data",

            "value_order",

            {
                {"channel", TYPE_INT}
            },
            {
                std::make_tuple("paths_to_text_nodes:", ":paths_to_text_nodes", TYPE_STRING),
                std::make_tuple("paths_to_floating_number_nodes:", ":paths_to_floating_number_nodes", TYPE_REAL),
                std::make_tuple("paths_to_integer_value_nodes:", ":paths_to_integer_value_nodes", TYPE_INT)
            },

            "cold_bath_begin",
            "warm_bath_begin",
            "cold_bath_end",
            "warm_bath_end",

            {},

            {"mapping_tags:", ":mapping_tags"}
        );
    };

    TEST_FIXTURE (config_content, interpret)
    {
        data config_data = data::interpret(config_content);

        CHECK_EQUAL(config_content.input_dir, config_data.input_dir);
        CHECK_EQUAL(config_content.output_dir, config_data.output_dir);

        CHECK_EQUAL(config_content.output_file_name, config_data.output_file_name);

        CHECK_EQUAL(true, config_data.do_calculations);

        CHECK_EQUAL(config_content.time_node_path_str, config_data.time_npath.path_string());
        CHECK_EQUAL(config_content.cold_bath_node_path_str, config_data.cold_bath_npath.path_string());
        CHECK_EQUAL(config_content.warm_bath_node_path_str, config_data.warm_bath_npath.path_string());
        CHECK_EQUAL(config_content.data_node_path_str, config_data.data_npath.path_string());

        std::vector<int> expected_data_order = {0,1,2,3};

        CHECK_EQUAL(expected_data_order.size(), config_data.data_order.size());
        CHECK_ARRAY_EQUAL(expected_data_order, config_data.data_order, expected_data_order.size());

        for (const auto & n_n_path_str_w_t : config_content.named_node_path_str_w_types)
        {
            // Checking for existence of node_tag map entries in config_data
            CHECK_EQUAL(1, config_data.named_npaths_w_types.count(n_n_path_str_w_t.first));

            auto n_path_w_t_it = config_data.named_npaths_w_types.find(n_n_path_str_w_t.first);

            // Checking for correspondence of config_data node_path map with config_content node_path map
            CHECK_EQUAL(n_n_path_str_w_t.second.first, n_path_w_t_it->second.first.path_string());
            CHECK_EQUAL(n_n_path_str_w_t.second.second, n_path_w_t_it->second.second);
        }

        CHECK_EQUAL(0, config_data.cold_bath_begin_mapping.first);
        CHECK_EQUAL(0, config_data.cold_bath_begin_mapping.second);

        CHECK_EQUAL(2.3, config_data.warm_bath_begin_mapping.first);
        CHECK_EQUAL(2.3, config_data.warm_bath_begin_mapping.second);

        CHECK_EQUAL(231.0, config_data.cold_bath_end_mapping.first);
        CHECK_EQUAL(231.0, config_data.cold_bath_end_mapping.second);

        CHECK_EQUAL(777, config_data.warm_bath_end_mapping.first);
        CHECK_EQUAL(777, config_data.warm_bath_end_mapping.second);

        std::unordered_multimap<std::string, std::pair< double, double >> expected_mappping =
        {
            {"test_mapping", {3, 6}},
            {"test_mapping_2", {1234.567,1234.891}},
            {"test_mapping_3", {0.000,0.0001}}
        };

        for (const auto & exp_map : expected_mappping)
        {
            CHECK_EQUAL(1, config_data.mapping.count(exp_map.first));

            CHECK_EQUAL(exp_map.second.first, config_data.mapping.find(exp_map.first)->second.first);
            CHECK_EQUAL(exp_map.second.second, config_data.mapping.find(exp_map.first)->second.second);
        }
    }
}

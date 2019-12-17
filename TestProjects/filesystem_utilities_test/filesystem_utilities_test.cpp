#include "../../DTS_DataProcessingRoutine/filesystem_utilities.hpp"

#include <vector>
#include <set>

#include "UnitTest++.h"

using namespace filesystem_utilities;

SUITE(test_read_file_)
{
    TEST(invalid_file_path_)
    {
        CHECK_THROW(read_file("abcdefgh"), std::ios_base::failure);
    }

    TEST(emtpy_file_path_)
    {
        CHECK_THROW(read_file(""), std::ios_base::failure);
    }

    TEST(one_line_text_)
    {
        CHECK_EQUAL("This file contains one line of text.\n", read_file("test_input_files/one_line_text"));
        // on Windows there might be an additional '\r' before the '\n'!
    }

    TEST(two_line_text_)
    {
        CHECK_EQUAL("This file contains\ntwo lines of text.\n", read_file("test_input_files/two_line_text"));
    }

    TEST(longer_file_paths_)
    {
        CHECK_EQUAL("This file contains one line of text.\n", read_file("./test_input_files/relative_paths/one_line_text"));
        CHECK_EQUAL("This file contains\ntwo lines of text.\n", read_file("./test_input_files/relative_paths/two_line_text"));

        CHECK_EQUAL("This file contains one line of text.\n", read_file("/home/leon/HDDdata/Geooekologie/BA_Bachelorarbeit/Programm/TestProjects/filesystem_utilities_test/test_input_files/absolute_paths/one_line_text"));
        CHECK_EQUAL("This file contains\ntwo lines of text.\n", read_file("/home/leon/HDDdata/Geooekologie/BA_Bachelorarbeit/Programm/TestProjects/filesystem_utilities_test/test_input_files/absolute_paths/two_line_text"));
    }
}

SUITE (file_paths_iterator_)
{
    TEST (simple_test_)
    {
        std::vector<std::string> expected_file_paths
        {
            "test_file_paths/test_file_1.txt",
            "test_file_paths/test_file_2.xml",
            "test_file_paths/test_file_3.csv",
        };

        std::set<std::string> actual_file_paths;

        for (auto fp_iter = file_paths_iterator("test_file_paths"); fp_iter != file_paths_iterator(); ++fp_iter)
            { actual_file_paths.insert(*fp_iter); }

        for (const std::string & expec_fp : expected_file_paths)
        {
            CHECK_EQUAL(1, actual_file_paths.count(expec_fp));
        }
    }
}

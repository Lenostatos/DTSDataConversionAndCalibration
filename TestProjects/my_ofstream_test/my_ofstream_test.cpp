#include "../../DTS_DataProcessingRoutine/my_ofstream.hpp"

#include "UnitTest++.h"

#include "boost/filesystem.hpp"

#include "../../DTS_DataProcessingRoutine/globals.hpp"

namespace boost_fs = boost::filesystem;

SUITE(test_open)
{
    TEST(simple_file)
    {
        my_ofstream ofs;
        ofs.open("test_name", 0, "test_output", 0);
        CHECK(boost_fs::exists("test_output/test_name_1.txt"));
        CHECK(boost_fs::is_regular_file("test_output/test_name_1.txt"));
    }
}

SUITE(test_writing_to_file)
{
    TEST(one_character)
    {
        my_ofstream ofs;
        ofs.open("test_name", 0, "test_output", 0);
        ofs.stream_ << '.';
        ofs.addFileDone();
        CHECK_EQUAL(".", read_in_file("test_output/test_name_1.txt"));
    }

    TEST(one_string)
    {
        my_ofstream ofs;
        ofs.open("test_name", 0, "test_output", 0);
        ofs.stream_ << "Hello";
        ofs.addFileDone();
        CHECK_EQUAL("Hello", read_in_file("test_output/test_name_1.txt"));
    }

    TEST(one_number)
    {
        my_ofstream ofs;
        ofs.open("test_name", 0, "test_output", 0);
        ofs.stream_ << 1234;
        ofs.addFileDone();
        CHECK_EQUAL("1234", read_in_file("test_output/test_name_1.txt"));
    }
}

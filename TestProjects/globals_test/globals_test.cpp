#include "../../DTS_DataProcessingRoutine/globals.hpp"

#include <vector>
#include <iostream>

#include "UnitTest++.h"

using namespace globals;

SUITE (csv_int_class_)
{
    class single_value_strings
    {
    public:
        std::vector<std::string> csv_strs;

        single_value_strings()
        : csv_strs(
            {
                "-1",
                "-1.",
                "-1.0",
                "-1.000000",
                "-1.12344"
            })
        {}
    };

    TEST_FIXTURE (single_value_strings, one_value_)
    {
        std::vector<int> actual_csv(1);

        csv_int::parse("0", 1, actual_csv.begin(), ',');
        CHECK_EQUAL(0, actual_csv[0]);

        csv_int::parse("1", 1, actual_csv.begin(), ',');
        CHECK_EQUAL(1, actual_csv[0]);

        csv_int::parse("12344", 1, actual_csv.begin(), ',');
        CHECK_EQUAL(12344, actual_csv[0]);


        csv_int csv_i("0", ',');
        CHECK_EQUAL(1, csv_i.values.size());
        CHECK_EQUAL(0, csv_i.values[0]);

        csv_i = csv_int("1", ',');
        CHECK_EQUAL(1, csv_i.values.size());
        CHECK_EQUAL(1, csv_i.values[0]);

        csv_i = csv_int("12344", ',');
        CHECK_EQUAL(1, csv_i.values.size());
        CHECK_EQUAL(12344, csv_i.values[0]);

        for (const std::string & csv_str : csv_strs)
        {
            try
            {
                csv_int::parse(csv_str.c_str(), 1, actual_csv.begin(), ',');
                CHECK_EQUAL(-1, actual_csv[0]);

                csv_int csv_i(csv_str, ',');
                CHECK_EQUAL(1, csv_i.values.size());
                CHECK_EQUAL(-1, csv_i.values[0]);
            }
            catch (const std::exception & exc)
            {
                std::cout << "Error: With string \"" << csv_str << "\"!" << std::endl;
                std::cout << exc.what() << std::endl;
            }
        }
    }

    class multiple_value_strings
    {
    public:
        std::vector<std::string> csv_strs;
        std::vector<std::vector<int>> csv_vals;

        multiple_value_strings()
        : csv_strs(
            {
                "0,0",
                "1,1",
                "-1,-1",
                "-1.,-1.",
                "-1.0,-1.0",
                "-1.000000,-1.00000",
                "1,2,3",
                "1.,2.,3.",
                "1.,2,3.",
                "1.0,2,3.",
                "1.,2,3.0",
                "1.0,2,3.0",
                "-1.0,-2,-3.0",
                "-1.0,-2,3.0",
                "-1.12,-2.,3.0234",
                "-1.12,-2.1231265167245,3.0234"
            }),
          csv_vals(
            {
               {0, 0},
               {1, 1},
               {-1, -1},
               {-1, -1},
               {-1, -1},
               {-1, -1},
               {1, 2, 3},
               {1, 2, 3},
               {1, 2, 3},
               {1, 2, 3},
               {1, 2, 3},
               {1, 2, 3},
               {-1, -2, -3},
               {-1, -2, 3},
               {-1, -2, 3},
               {-1, -2, 3}
            })
        {}
    };

    TEST_FIXTURE (multiple_value_strings, multiplemultiple_values_)
    {
        auto csv_val_iter = csv_vals.begin();

        for (const std::string & csv_str : csv_strs)
        {
            try
            {
                std::vector<int> actual_csvs(csv_val_iter->size());
                csv_int::parse(csv_str.c_str(), csv_val_iter->size(), actual_csvs.begin(), ',');
                CHECK_ARRAY_EQUAL(*csv_val_iter, actual_csvs, csv_val_iter->size());

                csv_int csv_i(csv_str, ',');
                CHECK_EQUAL(csv_val_iter->size(), csv_i.values.size());
                CHECK_ARRAY_EQUAL(*csv_val_iter, csv_i.values, csv_val_iter->size());
            }
            catch (const std::exception & exc)
            {
                std::cout << "Error: With string \"" << csv_str << "\"!" << std::endl;
                std::cout << exc.what() << std::endl;
            }


            ++csv_val_iter;
        }
    }
}

SUITE (csv_double_class_)
{
    class single_value_strings
    {
    public:
        std::vector<std::string> csv_strs;
        std::vector<double> expected_csvs;

        single_value_strings()
        : csv_strs(
            {
                "-1",
                "-1.",
                "-1.0",
                "-1.000000",
                "-1.12344"
            }),
          expected_csvs(
            {
              -1,
              -1,
              -1,
              -1,
              -1.12344
            })
        {}
    };

    TEST_FIXTURE (single_value_strings, one_value_)
    {
        std::vector<double> actual_csv(1);

        csv_double::parse("0", 1, actual_csv.begin(), ',');
        CHECK_EQUAL(0, actual_csv[0]);

        csv_double::parse("1", 1, actual_csv.begin(), ',');
        CHECK_EQUAL(1, actual_csv[0]);

        csv_double::parse("12344", 1, actual_csv.begin(), ',');
        CHECK_EQUAL(12344, actual_csv[0]);

        csv_double::parse("1.2344", 1, actual_csv.begin(), ',');
        CHECK_EQUAL(1.2344, actual_csv[0]);

        csv_double::parse("12.344", 1, actual_csv.begin(), ',');
        CHECK_EQUAL(12.344, actual_csv[0]);

        csv_double::parse("-12.344", 1, actual_csv.begin(), ',');
        CHECK_EQUAL(-12.344, actual_csv[0]);


        csv_double csv_i("0", ',');
        CHECK_EQUAL(1, csv_i.values.size());
        CHECK_EQUAL(0, csv_i.values[0]);

        csv_i = csv_double("1", ',');
        CHECK_EQUAL(1, csv_i.values.size());
        CHECK_EQUAL(1, csv_i.values[0]);

        csv_i = csv_double("12344", ',');
        CHECK_EQUAL(1, csv_i.values.size());
        CHECK_EQUAL(12344, csv_i.values[0]);

        csv_i = csv_double("1.2344", ',');
        CHECK_EQUAL(1, csv_i.values.size());
        CHECK_EQUAL(1.2344, csv_i.values[0]);

        csv_i = csv_double("12.344", ',');
        CHECK_EQUAL(1, csv_i.values.size());
        CHECK_EQUAL(12.344, csv_i.values[0]);

        csv_i = csv_double("-12.344", ',');
        CHECK_EQUAL(1, csv_i.values.size());
        CHECK_EQUAL(-12.344, csv_i.values[0]);

        auto expec_csvs_iter = expected_csvs.begin();

        for (const std::string & csv_str : csv_strs)
        {
            try
            {
                csv_double::parse(csv_str.c_str(), 1, actual_csv.begin(), ',');
                CHECK_EQUAL(*expec_csvs_iter, actual_csv[0]);

                csv_double csv_i(csv_str, ',');
                CHECK_EQUAL(1, csv_i.values.size());
                CHECK_EQUAL(*expec_csvs_iter, csv_i.values[0]);
            }
            catch (const std::exception & exc)
            {
                std::cout << "Error: With string \"" << csv_str << "\"!" << std::endl;
                std::cout << exc.what() << std::endl;
            }

            ++expec_csvs_iter;
        }
    }

    class multiple_value_strings
    {
    public:
        std::vector<std::string> csv_strs;
        std::vector<std::vector<double>> csv_vals;

        multiple_value_strings()
        : csv_strs(
            {
                "0,0",
                "1,1",
                "-1,-1",
                "-1.,-1.",
                "-1.0,-1.0",
                "-1.000000,-1.00000",
                "1,2,3",
                "1.,2.,3.",
                "1.,2,3.",
                "1.0,2,3.",
                "1.,2,3.0",
                "1.0,2,3.0",
                "-1.0,-2,-3.0",
                "-1.0,-2,3.0",
                "-1.12,-2.,3.0234",
                "-1.12,-2.1231265167245,3.0234"
            }),
          csv_vals(
            {
               {0, 0},
               {1, 1},
               {-1, -1},
               {-1, -1},
               {-1, -1},
               {-1, -1},
               {1, 2, 3},
               {1, 2, 3},
               {1, 2, 3},
               {1, 2, 3},
               {1, 2, 3},
               {1, 2, 3},
               {-1, -2, -3},
               {-1, -2, 3},
               {-1.12, -2, 3.0234},
               {-1.12, -2.1231265167245, 3.0234}
            })
        {}
    };

    TEST_FIXTURE (multiple_value_strings, multiple_values_)
    {
        auto csv_val_iter = csv_vals.begin();

        for (const std::string & csv_str : csv_strs)
        {
            try
            {
                std::vector<double> actual_csvs(csv_val_iter->size());
                csv_double::parse(csv_str.c_str(), csv_val_iter->size(), actual_csvs.begin(), ',');
                CHECK_ARRAY_EQUAL(*csv_val_iter, actual_csvs, csv_val_iter->size());

                csv_double csv_i(csv_str, ',');
                CHECK_EQUAL(csv_val_iter->size(), csv_i.values.size());
                CHECK_ARRAY_EQUAL(*csv_val_iter, csv_i.values, csv_val_iter->size());
            }
            catch (const std::exception & exc)
            {
                std::cout << "Error: With string \"" << csv_str << "\"!" << std::endl;
                std::cout << exc.what() << std::endl;
            }

            ++csv_val_iter;
        }
    }
}

SUITE(test_decomp_csv_)
{
    class csv
    {
    public:
        int num_csv_values;
        std::vector<std::string> csv_strings;

        std::vector<double> normal_double_vector;
        std::vector<double> expected_normal_double_vector;

        std::vector<double*> pointer_double_vector;
        std::vector<double*> expected_pointer_double_vector;

        std::vector<double> to_be_pointed_to_double_vector;

        csv()
        : num_csv_values(4),
        csv_strings(
        {
            "1,2,3,4",
            "1.,2.,3.,4.",
            "1.0,2.0,3.0,4.0",
            "1.00,2.00,3.00,4.00",
            "1.0,2.00,3.000,4.0000"
        }),
        expected_normal_double_vector({1.0,2.0,3.0,4.0})
        {
            normal_double_vector.resize(num_csv_values);
            pointer_double_vector.resize(num_csv_values);
            to_be_pointed_to_double_vector.resize(num_csv_values);
            expected_pointer_double_vector.resize(num_csv_values);

            for(int i = 0; i < num_csv_values; ++i)
            {
                pointer_double_vector[i] = &(to_be_pointed_to_double_vector[i]);
                expected_pointer_double_vector[i] = &(expected_normal_double_vector[i]);
            }
        }
    };

    TEST_FIXTURE(csv, normal_vector_)
    {
        for (const auto & csv_string : csv_strings)
        {
            globals::decomp_csv(csv_string.c_str(), num_csv_values, begin(normal_double_vector));
            CHECK_ARRAY_EQUAL(expected_normal_double_vector, normal_double_vector, num_csv_values);

            globals::decomp_csv(csv_string, num_csv_values, begin(normal_double_vector));
            CHECK_ARRAY_EQUAL(expected_normal_double_vector, normal_double_vector, num_csv_values);
        }
    }

    TEST_FIXTURE(csv, pointer_vector_)
    {
        for (const auto & csv_string : csv_strings)
        {
            globals::decomp_csv_into_iterator_container(csv_string.c_str(), num_csv_values, begin(pointer_double_vector));
            for (int i = 0; i < num_csv_values; ++i)
            {
                CHECK_EQUAL(*(expected_pointer_double_vector[i]), *(pointer_double_vector[i]));
            }
        }

        for (const auto & csv_string : csv_strings)
        {
            globals::decomp_csv_into_iterator_container(csv_string, num_csv_values, begin(pointer_double_vector));
            for (int i = 0; i < num_csv_values; ++i)
            {
                CHECK_EQUAL(*(expected_pointer_double_vector[i]), *(pointer_double_vector[i]));
            }
        }
    }

    // more tests needed...
}

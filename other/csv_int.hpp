#ifndef _CSV_INT_H
#define _CSV_INT_H


#include <vector>
#include <string>
#include <cstring>

class csv_int
{
    public:
    std::vector<int> values;

    csv_int(const std::string & csv_str, char separator = ',');

    template<typename I>
    static void parse(const char * csv_string, I dest_iterator, int num_values, char separator = ',')
    {
        *dest_iterator = strtol(csv_string, nullptr, 10);

        const char * next_number_string = strchr(csv_string, separator);
        --num_values;

        while (num_values > 0)
        {
            ++dest_iterator;
            *dest_iterator = strtol(++next_number_string, nullptr, 10);

            next_number_string = strchr(next_number_string, separator);
            --num_values;
        }

        // Alternative Design
        // TODO: test speed difference
        // disadvantage: doesn't work when separator is longer than one character
//            char * next_number_string;
//
//            *dest_iterator = strtol(csv_string, &next_number_string, 10);
//            ++dest_iterator;
//
//            for (int i = 1; i < num_values - 1; ++i, ++dest_iterator) // string may not be null terminated, therefore stopping at num_values - 1
//                { *dest_iterator = strtol(++next_number_string, &next_number_string, 10); }
//
//            *dest_iterator = strtol(++next_number_string, nullptr, 10);
    }

};

#endif

#ifndef MAIN_HPP
#define MAIN_HPP

#include <iostream>

template <typename iterator_type>
void print_array(iterator_type arr_begin, const iterator_type & arr_end, size_t arr_length);
template <typename iterator_type>
void print_ptr_array(iterator_type arr_begin, const iterator_type & arr_end, size_t arr_length);

template <typename iterator_type>
void print_array(iterator_type arr_begin, const iterator_type & arr_end, size_t arr_length)
{
    std::cout << "-> ";
    size_t counter = 0;
    for (; arr_begin != arr_end; ++arr_begin, ++counter)
    {
        std::cout << *arr_begin << " ";

        // multiline output for better readability
        if ((counter+1) % 10 == 0 && counter > 0 && counter < arr_length - 1)
        {
            std::cout << "->\n-> ";
        }
    }
    std::cout << std::endl;
}

template <typename iterator_type>
void print_ptr_array(iterator_type arr_begin, const iterator_type & arr_end, size_t arr_length)
{
    std::cout << "-> ";
    size_t counter = 0;
    for (; arr_begin != arr_end; ++arr_begin, ++counter)
    {
        std::cout << **arr_begin << " ";

        // multiline output for better readability
        if ((counter+1) % 10 == 0 && counter > 0 && counter < arr_length - 1)
        {
            std::cout << "->\n-> ";
        }
    }
    std::cout << std::endl;
}


#endif // MAIN_HPP

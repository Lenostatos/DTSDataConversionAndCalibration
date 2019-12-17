#include <iostream>
#include <valarray>
#include <vector>

#include "main.hpp"

using namespace std;

int main()
{
    // Test with simple arrays

    // Two test_arrays
    valarray<int> val_array = {1, 2, 3, 4}; // a value array holding values
    vector<int*> ptr_array(4); // a pointer array holding pointers that shall be pointing to values of the value array

    // Printing both arrays before making changes
    cout << "Before everything:\n";
    print_array(begin(val_array), end(val_array), val_array.size());
    print_array(ptr_array.begin(), ptr_array.end(), ptr_array.size()); // The null pointers of the pointer array must not be dereferenced!
    // (Therefore the print_array function and not the print_ptr_array function is used here.)

    ptr_array[0] = &val_array[0];
    ptr_array[1] = &val_array[1];
    ptr_array[2] = &val_array[2];
    ptr_array[3] = &val_array[3];

    cout << "After passing the pointers:\n";
    print_array(begin(val_array), end(val_array), val_array.size());
    print_ptr_array(ptr_array.begin(), ptr_array.end(), ptr_array.size());

    val_array[1] = 34;

    cout << "After changing the value array:\n";
    print_array(begin(val_array), end(val_array), val_array.size());
    print_ptr_array(ptr_array.begin(), ptr_array.end(), ptr_array.size());


    // Test with two-dimensional array

    // Test-matrix
    vector<valarray<int>> example_matrix({{1, 2, 3, 4, 5},
                                          {6, 7, 8, 9, 10},
                                          {11, 12, 13, 14, 15},
                                          {16, 17, 18, 19, 20}});

    cout << "The test matrix:\n";
    for (auto iter = begin(example_matrix); iter != end(example_matrix); ++iter)
        {print_array(begin(*iter), end(*iter), iter->size());}

    // Test-Slicing
    valarray<int> test_slice(2);

    test_slice = example_matrix[2][slice(1, 2, 3)];

    cout << "A slice from one row of the matrix:\n";
    print_array(begin(test_slice), end(test_slice), test_slice.size());

    // Test-cross-referencing
    auto ptr_array_iter = begin(ptr_array); // Don't forget to reset this iterator before each use!
    for (auto iter = begin(example_matrix); iter != end(example_matrix); ++iter, ++ptr_array_iter)
        {*ptr_array_iter = &((*iter)[1]);}

    cout << "The pointer array with one value from every row:\n";
    print_ptr_array(ptr_array.begin(), ptr_array.end(), ptr_array.size());

    // Test value assignment via the pointer array
    *(ptr_array[0]) = 99; // the braces aren't necessary but i like them...

    cout << "The pointer array after changing one of it's values:\n";
    print_ptr_array(ptr_array.begin(), ptr_array.end(), ptr_array.size());

    // Testing the use of not constructed objects (doesn't really belong here but whatever)
    valarray<int> valarr1;
    valarray<int> valarr2;

    valarr1 += valarr2;

    cout << "A valarray that was never constructed explicitly:\n";
    print_array(begin(valarr1), end(valarr1), valarr1.size());


    return 0;
}

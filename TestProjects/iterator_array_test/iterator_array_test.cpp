#include "../../DTS_DataProcessingRoutine/iterator_array.hpp"

#include <vector>

#include "UnitTest++.h"

class matrix
{
public:
    std::vector<std::vector<int>> mat;

    matrix()
    : mat(
    {
        {1, 2, 3},
        {1, 2, 3},
        {1, 2, 3}
    })
    {}
};

TEST_FIXTURE(matrix, constructor)
{
    iterator_array<std::vector<int>::iterator> iter_array(mat.begin(), mat.end());

    for (const auto & iter : iter_array)
    {
        CHECK_EQUAL(1, iter);
    }
}

TEST_FIXTURE(matrix, advance_operator)
{
    iterator_array<std::vector<int>::iterator> iter_array(mat.begin(), mat.end());

    iter_array.advance_if_possible();

    for (const auto & iter : iter_array)
    {
        CHECK_EQUAL(2, iter);
    }
}

/** \file
 *  \brief Header for the *iterator_array* class.
 */

#ifndef MULTI_ARRAY_ITERATOR_HPP_INCLUDED
#define MULTI_ARRAY_ITERATOR_HPP_INCLUDED

#include <vector>
#include <cassert>

template <typename I>
class iterator_array
{
private:
    std::vector<I> iterators_;      /**< Iterators to different containers' elements. */
    std::vector<I> end_iterators_;  /**< End iterators of different containers. */
    const size_t cont_size_;              /**< Size of each of the containers that supply the iterators. */

public:
    template <typename M_I>
    iterator_array(M_I begin_matrix_iterator, const M_I & end_matrix_iterator, const std::vector< int > * column_order = nullptr)
    : cont_size_(begin_matrix_iterator->size())
    {
        while (begin_matrix_iterator != end_matrix_iterator)
        {
            assert(cont_size_ == begin_matrix_iterator->size() && "An iterator array can only be initialized with iterators from containers of equal size!");

            iterators_.push_back(begin_matrix_iterator->begin());
            end_iterators_.push_back(begin_matrix_iterator->end());

            ++begin_matrix_iterator;
        }

        // Reorder the iterators and end iterators
        if (column_order)
        {
            assert(column_order->size() == iterators_.size() && "Attempted to order an iterator array with an unfitting number of ordering indices!");

            auto iterators_copy = iterators_;
            auto end_iterators_copy = end_iterators_;

            for (size_t i = 0; i < column_order->size(); ++i)
            {
                iterators_[i] = iterators_copy[(*column_order)[i]];
                end_iterators_[i] = end_iterators_copy[(*column_order)[i]];
            }
        }
    }

    /// Inserts begin and end iterator of \a container before the element at position \a pos.
    /// This function is currently not used!
    template <typename C>
    void insert(size_t pos, C & container)
    {
        assert(cont_size_ == container.size() && "Attempted to insert iterator of container with size not equal to size of already inserted containers!");

        auto it = iterators_.begin();
        it += pos;
        auto end_it = end_iterators_.begin();
        end_it += pos;

        iterators_.insert(it, container.begin());
        end_iterators_.insert(end_it, container.end());
    }

    class iterator
    {
    private:
        typename std::vector<I>::iterator iter_array_iter_;

    public:
        iterator(const typename std::vector<I>::iterator & iter)
        : iter_array_iter_(iter)
        {}

        /// Prefix-Advance operator.
        iterator & operator ++ () { ++iter_array_iter_; return *this; }
        /// Postfix-Advance operator.
        iterator operator ++ (int)
            { iterator tmp(*this); operator++(); return tmp; }

        /// Equality operator.
        bool operator == (const iterator& rhs) const
            { return this->iter_array_iter_== rhs.iter_array_iter_; }
        /// Inequality operator.
        bool operator != (const iterator& rhs) const
            { return this->iter_array_iter_!= rhs.iter_array_iter_; }

        /// Dereference operator.
        auto & operator*() { return **iter_array_iter_; }
    };

    iterator begin() { return iterator(iterators_.begin()); }
    const iterator end() { return iterator(iterators_.end()); }

    bool advance_if_possible()
    {
        bool res = true;

        for (size_t i = 0; i < iterators_.size(); ++i)
        {
            if (iterators_[i] != end_iterators_[i]) { ++iterators_[i]; }
            else { res = false; }
        }

        return res;
    }
};

// deprecated version:
//template <typename I>
//class iterator_array
//{
//public:
//    std::vector<I> iterators;       /**< Iterators to different containers' elements. */
//    std::vector<I> end_iterators_;  /**< End iterators of different containers. */
//
//    template <typename M_I>
//    iterator_array(M_I begin_matrix_iterator, const M_I & end_matrix_iterator)
//    {
//        size_t array_size = begin_matrix_iterator->size();
//
//        while (begin_matrix_iterator != end_matrix_iterator)
//        {
//            assert(array_size == begin_matrix_iterator->size() && "An iterator array can only be initialized with iterators from containers of equal size!");
//            array_size = begin_matrix_iterator->size();
//
//            iterators.push_back(begin(*begin_matrix_iterator));
//            end_iterators_.push_back(end(*begin_matrix_iterator));
//
//            ++begin_matrix_iterator;
//        }
//    }
//
//    bool advance_if_possible()
//    {
//        bool res = true;
//
//        for (size_t i = 0; i < iterators.size(); ++i)
//        {
//            if (iterators[i] != end_iterators_[i]) { ++iterators[i]; }
//            else { res = false; }
//        }
//
//        return res;
//    }
//};

#endif // MULTI_ARRAY_ITERATOR_HPP_INCLUDED

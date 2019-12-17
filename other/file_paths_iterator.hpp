#ifndef _FILE_PATHS_ITERATOR_H
#define _FILE_PATHS_ITERATOR_H


/**
 *  Recommended for new code by the library authors.
 *  Prevents the library from offering and using deprecated parts.
 */
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include "boost/filesystem.hpp"

#include <string>

/** \brief An iterator over the entries of a directory, that returns an entry's path when dereferenced.
 *
 *  \todo Store the stem of the path in a private variable and return "stem /= file_name_only".
 */
class file_paths_iterator
{
    private:
    boost::filesystem::directory_iterator dir_iter_;


    public:
    /// Constructs the end iterator.
    file_paths_iterator()
    : dir_iter_(boost::filesystem::directory_iterator()) {}

    /// Constructs a file_paths_iterator for a directory.
    file_paths_iterator(const std::string & directory_path)
    : dir_iter_(boost::filesystem::directory_iterator(directory_path)) {}

    /// Prefix-Advance operator.
    file_paths_iterator & operator ++() { ++dir_iter_; return *this; }
    /// Postfix-Advance operator.
    file_paths_iterator operator ++(int) { file_paths_iterator tmp(*this); operator++(); return tmp; }

    /// Equality operator.
    bool operator ==(const file_paths_iterator & rhs) const
        {return dir_iter_== rhs.dir_iter_;}
    /// Inequality operator.
    bool operator !=(const file_paths_iterator & rhs) const
        {return dir_iter_!= rhs.dir_iter_;}

    /// Dereference operator.
    const std::string & operator *() const {return dir_iter_->path().string();}

};

#endif

#ifndef _FILESYSTEM_UTILITIES_H
#define _FILESYSTEM_UTILITIES_H


#include <string>

class filesystem_utilities
{
    public:
    filesystem_utilities() = delete;

    /// Checks whether the construction of a boost::filesystem::path object from \a path throws an exception.
    static bool constructable(const std::string & path);

    /// Checks whether something at \a path exists in the file system.
    static bool exists(const std::string & path);

    /// Checks whether the place that \a path leads to is a directory.
    static bool is_directory(const std::string & path);

    /// Creates a directory at \a directory_path.
    static bool create_directory(const std::string & directory_path);

    /// Renames a filesystem element.
    static void rename(const std::string old_p, const std::string new_p);

    /// Appends a new element to \a path together with a separator (if \a path doesn't end with one).
    static std::string append_to_path(const std::string & path, const std::string & appendix);

    /// Reads the content of a file into a string.
    static std::string read_file(const std::string & file_path);

    /// Counts the elements within a directory.
    static long count_files(const std::string & directory_path);

    static std::string native_path(const std::string & path);

};

#endif


#include "filesystem_utilities.hpp"

#include <iostream>

/**
 *  Recommended for new code by the library authors.
 *  Prevents the library from offering and using deprecated parts.
 */
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include "boost/filesystem.hpp"

namespace boost_fs = boost::filesystem;

/// Checks whether the construction of a boost::filesystem::path object from \a path throws an exception.
bool filesystem_utilities::constructable(const std::string & path)
{
  // Bouml preserved body begin 0002E70D
    try { boost_fs::path p(path); }
    catch (const boost_fs::filesystem_error & fs_exc)
    {
        std::cout << fs_exc.what() << std::endl;

        return false;
    }

    return true;
  // Bouml preserved body end 0002E70D

}

/// Checks whether something at \a path exists in the file system.
bool filesystem_utilities::exists(const std::string & path)
{
  // Bouml preserved body begin 0002E78D
    { return boost_fs::exists(path); }
  // Bouml preserved body end 0002E78D

}

/// Checks whether the place that \a path leads to is a directory.
bool filesystem_utilities::is_directory(const std::string & path)
{
  // Bouml preserved body begin 0002E80D
    { return boost_fs::is_directory(path); }
  // Bouml preserved body end 0002E80D

}

/// Creates a directory at \a directory_path.
bool filesystem_utilities::create_directory(const std::string & directory_path)
{
  // Bouml preserved body begin 0002E88D
    { return boost_fs::create_directory(directory_path); }
  // Bouml preserved body end 0002E88D

}

/**
 *  \warning    An existing file of the same name as \a new_p gets destroyed.
 */
void filesystem_utilities::rename(const std::string old_p, const std::string new_p)
{
    try { boost_fs::rename(old_p, new_p); }
    catch (const std::exception & exc)
    {
        try { std::cerr << "\nError: Couldn't rename a file from \"" << old_p << "\" to \"" << new_p << "\"!\n" << std::endl; }
        catch (...) {}
        throw;
    }
}

/// Appends a new element to \a path together with a separator (if \a path doesn't end with one).
std::string filesystem_utilities::append_to_path(const std::string & path, const std::string & appendix)
{
  // Bouml preserved body begin 0002E90D
    { return boost_fs::path(path).append(appendix).string(); }
  // Bouml preserved body end 0002E90D

}

/// Reads the content of a file into a string.
/**
 *  A path to the file is passed to a std::ifstream object
 *  which copies the contents of the file to a std::string object.
 *
 *  \param[in]  file_path   A string containing the path to the file.
 *
 *  \exception std::ios_base::failure Thrown if the file could not be read.
 *
 *  \return A string with the contents of the file at \a file_path.
 */
std::string filesystem_utilities::read_file(const std::string & file_path)
{
  // Bouml preserved body begin 0002E98D
//    if (file_path.empty()) {throw std::invalid_argument("Error: A file name with length 0 was passed!\n");}
    std::ifstream ifs(file_path.c_str(), std::ios::in | std::ios::binary | std::ios::ate);

    std::ifstream::pos_type file_size;

    if (ifs)
    {
        file_size = ifs.tellg();

        if (file_size == -1)
        {
            std::cerr << "\nError with file \"" << file_path << "\"!\n" << std::endl;
            throw std::ios_base::failure("Error: The file_size couldn't be calculated. Is the file empty?\n");
        }
        else
        {
            ifs.seekg(0, std::ios::beg);

            std::string file(file_size,'\0');
            ifs.read(&file[0], file_size);

            return file;
        }
    }
    else
    {
        std::cerr << "\nError with file \"" << file_path << "\"!\n" << std::endl;
        throw std::ios_base::failure("Error: Couldn't establish a file stream!\n Maybe the file doesn't exist?\nMaybe the program needs further permissions?\n");
    }
  // Bouml preserved body end 0002E98D

}

/// Counts the elements within a directory.
/**
 *  \param[in]  directory_path The path to the desired directory.
 *
 *  \exception  std::invalid_argument   Thrown if \a directory_path doesn't lead to a directory.
 *
 *  \return The number of elements within the directory.
 */
long filesystem_utilities::count_files(const std::string & directory_path)
{
  // Bouml preserved body begin 0002EA0D
    using namespace boost_fs;

    path p = path(directory_path);

    if (boost_fs::is_directory(p))
    {
        directory_iterator dir(p);

        long numFiles = 0;

        directory_iterator end_iterator = directory_iterator();
        // iterating through the directory  with the data-files to determine their number
        while (dir != end_iterator) { // directory_iterator() returns a null-iterator which is the same as the end()-iterator
            numFiles++;
            dir++;
        }

        return numFiles;
    }
    else
    {
//            std::cerr << "\nError: Directory \"" << p << "\" could not be found!\n" << std::endl;
        throw std::invalid_argument("");
    }
  // Bouml preserved body end 0002EA0D

}

std::string filesystem_utilities::native_path(const std::string & path)
{
  // Bouml preserved body begin 0003060D
    return boost_fs::path(path).native();
  // Bouml preserved body end 0003060D

}


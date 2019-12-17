#ifndef INPUT_DIRECTORY_H
#define INPUT_DIRECTORY_H


#include <string>

#include "../other/file_paths_iterator.hpp"

namespace input { class configuration; }

namespace input {

/// Represents the directory holding the input files.
class directory
{
    public:
    std::string path_;


    private:
    long num_files_;


    public:
    explicit directory(const configuration & config);

    const std::string & path() const {return path_;};

    long num_files() const {return num_files_;}

    file_paths_iterator begin() const {return file_paths_iterator(path_);}

    file_paths_iterator end() const {return file_paths_iterator();}
};

} // namespace input

#endif

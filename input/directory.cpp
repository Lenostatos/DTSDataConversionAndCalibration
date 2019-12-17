
#include <iostream>

#include "directory.hpp"
#include "configuration.hpp"
#include "../other/file_paths_iterator.hpp"

#include "../other/filesystem_utilities.hpp"
#include "../other/my_exceptions/file_error.hpp"

namespace input {

directory::directory(const configuration & config)
{
  // Bouml preserved body begin 0001FB8D

    try { config.values().at(INPUT_DIR); }
    catch(const std::out_of_range & exc)
    {
        std::cerr << "\nError: The input directory tag ID couldn't be found!\n" << std::endl;
        throw;
    }

    path_ = filesystem_utilities::native_path(config.values().at(INPUT_DIR));

    if (!filesystem_utilities::exists(path_))
    {
        std::cerr << "\nError: Input data folder \"" << path_ << "\" doesn't exist!\n" << std::endl;
        throw my_exceptions::file_error();
    }
    else if (!filesystem_utilities::is_directory(path_))
    {
        std::cerr << "\nError: Path to input data folder \"" << path_ << "\" doesn't lead to a folder!\n" << std::endl;
        throw my_exceptions::file_error();
    }

    num_files_ = filesystem_utilities::count_files(path_);

  // Bouml preserved body end 0001FB8D
}

} // namespace input

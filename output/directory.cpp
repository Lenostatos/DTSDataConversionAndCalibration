
#include "directory.hpp"

#include <iostream>

#include "../input/configuration.hpp"
#include "../other/filesystem_utilities.hpp"
#include "../input/tag_id.hpp"
#include "../other/my_exceptions/file_error.hpp"

namespace output {

directory::directory(const input::configuration & config)
{
  // Bouml preserved body begin 0001FC0D

    try { config.values().at(input::tag_id::OUTPUT_FILE); }
    catch(const std::out_of_range & exc)
    {
        std::cerr << "\nError: The output file tag couldn't be found!\n" << std::endl;
        throw;
    }

    path_ = filesystem_utilities::native_path(config.values().at(input::tag_id::OUTPUT_FILE));

    if (!filesystem_utilities::exists(path_))
    {
        std::cout << "The folder at \"" << path_ << "\" doesn't exist yet.\n"
                  << "Trying to create the folder..." << std::endl;

        if (filesystem_utilities::create_directory(path_))
            { std::cout << "Successfully created the folder!" << std::endl; }
        else
        {
            std::cerr << "\nError: Folder creation failed!\n" << std::endl;
            throw my_exceptions::file_error();
        }
    }
    else if (!filesystem_utilities::is_directory(path_))
    {
        std::cerr << "\nError: Path to output data folder leads to something that is not a folder!\n" << std::endl;
        throw my_exceptions::file_error();
    }

  // Bouml preserved body end 0001FC0D

}

} // namespace output

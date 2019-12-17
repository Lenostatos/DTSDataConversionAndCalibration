
#include "switches.hpp"

#include <vector>
#include <iostream>

#include "configuration.hpp"
#include "tagging.hpp"
#include "fiber_map.hpp"
#include "../other/my_exceptions/tag_error.hpp"

namespace input {

switches::switches(const configuration & config, const tagging & tags, const fiber_map & map)
{
  // Bouml preserved body begin 0002178D

    // As the tags in the configuration file are not good variable names for the output file
    // the switch tags are assigned new names here
    std::map < tag_id, std::string > switch_ids_w_name =
    {
        {SIMPLE_CALIB, "simple_calibration"},
        {SINGLE_END_CALIB, "single_ended_calibration"},
    };

    for (const auto & sw_id_w_na : switch_ids_w_name)
    {
        try { config.values().at(sw_id_w_na.first); }
        catch(const std::out_of_range & exc)
        {
            std::cerr << "\nError: A switch tag ID couldn't be found!\n" << std::endl;
            throw;
        }

        if (config.values().at(sw_id_w_na.first) == "y")
            { data_.insert({sw_id_w_na.first, {sw_id_w_na.second, true}}); }
        else if (config.values().at(sw_id_w_na.first) == "n")
            { data_.insert({sw_id_w_na.first, {sw_id_w_na.second, false}}); }
        else
        {
            std::cerr << "\nError: The value of tag \"" << tags.values.at(sw_id_w_na.first) << "\" has to be either \"y\" or \"n\"!\n" << std::endl;
            throw my_exceptions::tag_error();
        }
    }

    // Special treatment for the do_a_calibration switches as they depend on the mapping of the baths!
    int num_baths = 4;

    for (const auto & elem : map.bath_exists())
    { if (!elem.second) { --num_baths; } }

    if (data_.at(SINGLE_END_CALIB).second && num_baths < 3)
    {
        std::cerr << "\nError: There have to be at least three mapped baths to perform the single ended calibration of temperature values!\n" << std::endl;
        throw my_exceptions::tag_error();
    }

    if (data_.at(SIMPLE_CALIB).second)
    {
        if (num_baths < 2 ||
            (
             !(map.bath_exists().at(C_B_BEGIN) && map.bath_exists().at(W_B_END)) &&
             !(map.bath_exists().at(W_B_BEGIN) && map.bath_exists().at(C_B_END))
            )
           )
        {
            std::cerr << "\nError: There has to be at least one warm and one cold bath mapping on opposing sides of the fiber to perform the simple calibration of temperature values!\n" << std::endl;
            throw my_exceptions::tag_error();
        }
    }
  // Bouml preserved body end 0002178D
}

} // namespace input

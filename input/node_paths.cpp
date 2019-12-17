
#include "node_paths.hpp"

#include <iostream>

#include "../other/node_path.hpp"
#include "configuration.hpp"
#include "tagging.hpp"
#include "../other/xml_file.hpp"
#include "../other/my_exceptions/tag_error.hpp"
#include "../other/my_exceptions/node_path_error.hpp"

namespace input {

node_paths::node_paths(const configuration & config, const tagging & tags)
{
  // Bouml preserved body begin 0001FA0D

    std::map < tag_id, node_value_type > single_ids_w_types
    ({
        {CHANNEL, INT},
        {TIME, SPECIAL},
        {COLD_BATH, REAL},
        {WARM_BATH, REAL},
        {DATA, SPECIAL}
    });

    for (const auto & id_w_type : single_ids_w_types)
    {
        node_path np;

        try { np = node_path(config.values().at(id_w_type.first)); }
        catch (const my_exceptions::node_path_error & exc)
        {
            std::cerr << "\nError: Couldn't create a node_path from \"" << config.values().at(id_w_type.first) << "\"!\n" << std::endl;
            throw;
        }
        catch (const std::out_of_range & exc)
        {
            std::cerr << "\nError: A single node path's tag ID couldn't be found!\n" << std::endl;
            throw;
        }

        if (with_tag_.insert({tags.values.at(id_w_type.first), {id_w_type.second, np}}).second == false)
        {
            std::cerr << "\nError: Tag \"" << tags.values.at(id_w_type.first) << "\" appears more than once!\n" << std::endl;
            throw my_exceptions::tag_error();
        }
    }

    std::map < tag_id, node_value_type > section_ids_w_types
    ({
        {INT_NODES, INT},
        {REAL_NODES, REAL},
        {STRING_NODES, STRING}
    });

    for (const auto & id_w_type : section_ids_w_types)
    {
        try { config.sections().at(id_w_type.first); }
        catch(const std::out_of_range & exc)
        {
            std::cerr << "\nError: A node path section's tag ID couldn't be found!\n" << std::endl;
            throw;
        }

        for (const auto & tag_w_val : config.sections().at(id_w_type.first))
        {
            node_path np;

            try { np = node_path(tag_w_val.second); }
            catch (const my_exceptions::node_path_error & exc)
            {
                std::cerr << "\nError: Couldn't create a node_path from \"" << tag_w_val.second << "\"!\n" << std::endl;
                throw;
            }

            if (with_tag_.insert({tag_w_val.first, {id_w_type.second, np}}).second == false)
            {
                std::cerr << "\nError: Tag \"" << tag_w_val.first << "\" appears more than once!\n" << std::endl;
                throw my_exceptions::tag_error();
            }
        }
    }

    if (with_tag_.insert({"time_string", {STRING, with_tag_.at(tags.values.at(TIME)).second}}).second == false)
    {
        std::cerr << "\nError: Tag \"time_string\" is a reserved tag!\n" << std::endl;
        throw my_exceptions::tag_error();
    }

  // Bouml preserved body end 0001FA0D

}

/**
 * \todo Implement! Also test node values for interpretability with their specified types.
 */
void node_paths::validate_against(const ::xml_file & xml) const
{
  // Bouml preserved body begin 0002CD0D
  // Bouml preserved body end 0002CD0D
}


} // namespace input

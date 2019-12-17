
#include "fiber_map.hpp"

#include <iostream>
#include <algorithm>

#include "configuration.hpp"
#include "tagging.hpp"
#include "../other/csv_double.hpp"
#include "../other/my_exceptions/tag_error.hpp"

namespace input {

/**
 *  \todo Ensure that the value-pairs are valid (e.g. not negative, smaller value first, etc.)
 */
fiber_map::fiber_map(const configuration & config, const tagging & tags)
{
  // Bouml preserved body begin 0001FB0D

    try { config.multi_sections().at(MAPPING); }
    catch(const std::out_of_range & exc)
    {
        std::cerr << "\nError: The mapping section tag ID couldn't be found!\n" << std::endl;
        throw;
    }

    for (const auto & elem : config.multi_sections().at(MAPPING))
    {
        csv_double csv;
        try { csv = csv_double(elem.second); }
        catch (const std::exception & exc)
        {
            std::cerr << "\nError: Couldn't interpret the mapping value-pair \"" << elem.second << "\" at mapping tag \"" << elem.first << ":\n"
                      << exc.what() << "\n" << std::endl;
            throw;
        }
        if (csv.values.size() != 2)
        {
            std::cerr << "Error: " << csv.values.size() << " instead of two values could be read at the mapping tag \"" << elem.first << "\"!\n" << std::endl;
            throw my_exceptions::tag_error();
        }

        data_.insert({elem.first, {csv.values[0], csv.values[1]}});
    }

    std::vector< tag_id > single_maps =
    {
        C_B_BEGIN,
        W_B_BEGIN,
        C_B_END,
        W_B_END
    };

    for (tag_id single : single_maps)
    {
        try { config.values().at(single); }
        catch(const std::out_of_range & exc)
        {
            std::cerr << "\nError: A single mapping tag's ID couldn't be found!\n" << std::endl;
            throw;
        }

        if (data_.count(tags.values.at(single)) > 0)
        {
            std::cerr << "\nError: The bath tag \"" << tags.values.at(single) << "\" mustn't be used inside the mapping tag section!\n" << std::endl;
            throw my_exceptions::tag_error();
        }

        csv_double csv;
        try { csv = csv_double(config.values().at(single)); }
        catch (const std::exception & exc)
        {
            std::cerr << "\nError: Couldn't interpret the mapping value-pair \"" << config.values().at(single) << "\" at mapping tag \"" << tags.values.at(single) << ":\n"
                      << exc.what() << "\n" << std::endl;
            throw;
        }
        if (csv.values.size() != 2)
        {
            std::cerr << "Error: " << csv.values.size() << " instead of two values could be read at the mapping tag \"" << tags.values.at(single) << "\"!\n" << std::endl;
            throw my_exceptions::tag_error();
        }

        data_.insert({tags.values.at(single), {csv.values[0], csv.values[1]}});
    }

    // Initialize the bath_exists_ variable
    std::vector < std::pair < tag_id, std::string >> bath_tagging =
    {
        {C_B_BEGIN, tags.values.at(C_B_BEGIN)},
        {W_B_BEGIN, tags.values.at(W_B_BEGIN)},
        {C_B_END, tags.values.at(C_B_END)},
        {W_B_END, tags.values.at(W_B_END)}
    };

    for (const auto & bath_tag : bath_tagging)
    {
        if (data_.find(bath_tag.second)->second.first == data_.find(bath_tag.second)->second.second)
        {
            bath_exists_.insert({bath_tag.first, false});
        }
        else { bath_exists_.insert({bath_tag.first, true}); }
    }

  // Bouml preserved body end 0001FB0D
}

/** \brief  Creates a vector of mapping tags at the positions where the \a reference_fiber's values are inside the tag's value-pair.
 *          - At the positions where no mapping value-pair matches the \a reference_fiber's value, empty strings are inserted.
 *          - Missing value-pairs that have the same beginning and end value are not represented in the vector.
 *          - Tags of which the value-pairs partially lie outside the \a reference_fiber are still printed to the vector where they overlap with the \a reference_fiber.
 */
std::vector < std::string > fiber_map::string_vector(const std::vector < laf_t > & reference_fiber) const
{
  // Bouml preserved body begin 0001F88D

    double max_map_val = 0;

    for (const auto & map_val_pair : data_)
        { if (map_val_pair.second.second > max_map_val) { max_map_val = map_val_pair.second.second; } }
    if (max_map_val > *std::max_element(reference_fiber.begin(), reference_fiber.end()))
        { std::cout << "\nWarning: The maximum mapping value is bigger than the biggest LAF value!\n" << std::endl; }

    std::vector<std::string> mapping_vector(reference_fiber.size(), "");

    for (const auto & map_val_pair : data_)
    {
        if (map_val_pair.second.first != map_val_pair.second.second)
        {
            size_t i = 0;
            while (i < reference_fiber.size() && reference_fiber[i] < map_val_pair.second.second)
            {
                if (reference_fiber[i] >= map_val_pair.second.first)
                    { mapping_vector[i] = map_val_pair.first; }

                ++i;
            }
            if (i <= reference_fiber.size() && mapping_vector[i - 1] != map_val_pair.first)
                { std::cout << "\nWarning: Mapping section at tag \"" << map_val_pair.first << "\" lies in between two contiguous LAF values and won't be included!\n" << std::endl; }
        }
    }

    return mapping_vector;

  // Bouml preserved body end 0001F88D

}

/** \brief  Maps beginning and end indices to the mapping tags.
 *          - Missing value-pairs that have the same beginning and end value are not represented.
 */
std::multimap < tag, std::pair < size_t, size_t >> fiber_map::indices(const std::vector < laf_t > & reference_fiber) const
{
  // Bouml preserved body begin 0003AB0D

    double max_map_val = 0;

    for (const auto & map_val_pair : data_)
        { if (map_val_pair.second.second > max_map_val) { max_map_val = map_val_pair.second.second; } }
    if (max_map_val > *std::max_element(reference_fiber.begin(), reference_fiber.end()))
        { std::cout << "\nWarning: The maximum mapping value is bigger than the biggest LAF value!\n" << std::endl; }

    std::multimap < tag, std::pair < size_t, size_t >> map_indices;

    for (const auto & map_val_pair : data_)
    {
        if (map_val_pair.second.first != map_val_pair.second.second)
        {
            size_t start_ind = 0;
            size_t i = 0;

            while (i < reference_fiber.size())
            {
                if (reference_fiber[i] >= map_val_pair.second.first)
                    { start_ind = i; break; }
                else { ++i; }
            }
            while (i < reference_fiber.size())
            {
                if (reference_fiber[i] > map_val_pair.second.second)
                    { break; }
                else { ++i; }
            }

            if (start_ind == i)
                { std::cout << "\nWarning: Mapping section at tag \"" << map_val_pair.first << "\" lies in between two contiguous LAF values and won't be included!\n" << std::endl; }
            else
                { map_indices.insert({map_val_pair.first, {start_ind, i - 1}}); }
        }
    }

    return map_indices;

  // Bouml preserved body end 0003AB0D
}


} // namespace input

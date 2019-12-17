
#include "data_node_value_order.hpp"

#include <iostream>

#include "configuration.hpp"
#include "tagging.hpp"
#include "../other/csv_int.hpp"

namespace input {

/**
 *  \todo Ensure that there are exactly 4 ordering values and that all are one of the numbers between 0 and 3!
 */
data_node_value_order::data_node_value_order(const configuration & config, const tagging & tags)
{
  // Bouml preserved body begin 0001FA8D

    try { config.values().at(DATA_ORDER); }
    catch (const std::out_of_range & exc)
    {
        std::cerr << "\nError: The data order tag ID couldn't be found!\n" << std::endl;
        throw;
    }

    try { order_ = std::move(csv_int(config.values().at(DATA_ORDER)).values); }
    catch (const std::exception & exc)
    {
        std::cerr << "\nError: The value of the tag \"" << tags.values.at(DATA_ORDER) << "\" is not readable!\n"
                  << exc.what() << "\n" << std::endl;
        throw;
    }

    for (auto & elem : order_) { --elem; }

  // Bouml preserved body end 0001FA8D
}

} // namespace input

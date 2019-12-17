#ifndef _FILE_DATA_H
#define _FILE_DATA_H


#include <vector>
#include <map>
#include <string>
#include <memory>

#include "laf_t.hpp"
#include "data_t.hpp"

namespace input { class node_paths; }
class xml_file;
class data_nodes;
namespace input { class data_node_value_order; }
namespace input { class tagging; }

/**
 *  \todo Test performance and memory size differences between float and double as data types for \a array_data_.
 *  \todo Replace std::string with input::tag for strings_, ints_ and reals_
 */
class file_data {
    private:
    std::pair<long, int> time_;

    std::vector<laf_t> laf_;

    /// Contains three arrays with data of the stokes, anti-stokes and temperature in that order.
    std::vector<std::vector < data_t >> array_data_;

    std::map<std::string, std::string> strings_;

    std::map<std::string, int> ints_;

    std::map<std::string, double> reals_;


    public:
    file_data(const input::node_paths & n_paths,
              const std::shared_ptr<xml_file> & file,
              const input::data_node_value_order & data_order,
              const input::tagging & tags,
              const data_nodes & d_nodes);

    const std::pair<long, int> & time() const
        { return time_; }

    const std::vector < laf_t > & laf() const
        { return laf_; }

    const std::vector < std::vector < data_t >> & array_data() const
        { return array_data_; }

    const std::map < std::string, std::string > & strings() const
        { return strings_; }

    const std::map < std::string, int > & ints() const
        { return ints_; }

    const std::map < std::string, double > & reals() const
        { return reals_; }


    private:
    /** \brief Converts a time string into a value-pair with seconds and fractional seconds since 1970-01-01 00:00:00. */
    static std::pair<long, int> parse_time_string(const std::string & time_str);

};

#endif

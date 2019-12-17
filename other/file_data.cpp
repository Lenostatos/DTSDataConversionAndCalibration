
#include "file_data.hpp"

#include "boost/date_time.hpp"

#include "../input/node_paths.hpp"
#include "xml_file.hpp"
#include "../input/data_node_value_order.hpp"
#include "../input/tagging.hpp"
#include "data_nodes.hpp"
#include "iterator_array.hpp"
#include "csv_float.hpp"

file_data::file_data(const input::node_paths & n_paths,
                     const std::shared_ptr<xml_file> & file,
                     const input::data_node_value_order & data_order,
                     const input::tagging & tags,
                     const data_nodes & d_nodes)
: laf_(d_nodes.num_nodes()), array_data_(3, std::vector< data_t >(d_nodes.num_nodes()))
{
  // Bouml preserved body begin 00021B0D

    // Get the time value-pair
    time_ = parse_time_string(n_paths.with_tag().at(tags.values.at(input::TIME)).second.node_value(*file));


    // Get laf and array data
    // Setup the data nodes iterator
    const rapidxml::xml_node<> * data_node_iter = d_nodes.begin();

    // Set up the iterator array
    size_t laf_ind = data_order.order()[0];

    input::data_node_value_order d_order_cpy(data_order);
    d_order_cpy.order().erase(d_order_cpy.order().begin());
    for (auto & elem : d_order_cpy.order()) { if (elem > laf_ind) { --elem; } }

    iterator_array < std::vector< data_t >::iterator > cross_ref_array(std::begin(array_data_), std::end(array_data_), &d_order_cpy.order());
    cross_ref_array.insert(laf_ind, laf_);

    // Extract the data of the data nodes
    if (d_nodes.name.empty())
    {
        do
        {
            csv_float::parse(data_node_iter->value(), cross_ref_array.begin(), 4);

            data_node_iter = data_node_iter->next_sibling();
        }
        while (data_node_iter != d_nodes.end() && cross_ref_array.advance_if_possible());
    }
    else
    {
        do
        {
            csv_float::parse(data_node_iter->value(), cross_ref_array.begin(), 4);

            data_node_iter = data_node_iter->next_sibling(d_nodes.name.c_str());
        }
        while (data_node_iter && cross_ref_array.advance_if_possible());
    }


    // Get the single values
    for (const auto & n_nt_w_np : n_paths.with_tag())
    {
        switch (n_nt_w_np.second.first)
        {
    case input::STRING:
            strings_.insert({n_nt_w_np.first, n_nt_w_np.second.second.node_value(*file)});
            break;
        case input::INT:
            ints_.insert({n_nt_w_np.first, stoi(n_nt_w_np.second.second.node_value(*file))});
            break;
        case input::REAL:
            reals_.insert({n_nt_w_np.first, stod(n_nt_w_np.second.second.node_value(*file))});
            break;
        default:
            break;
        }
    }

  // Bouml preserved body end 00021B0D
}

/**
 *  The time string must have the format YYYY-mm-dd[T| ]hh:mm:ss.fffffff. Any characters after that will be ommitted!
 *
 *  \warning    Timezone- or daytime-saving-information won't be included in the result.
 */
std::pair < long, int > file_data::parse_time_string(const std::string & time_str)
{
  // Bouml preserved body begin 00033C0D
    using namespace boost::posix_time;
    using namespace boost::date_time;

    std::string simple_time_string = time_str;

    size_t T_ind = simple_time_string.find_first_of('T');

    if (T_ind != std::string::npos)
        { simple_time_string.replace(T_ind, 1, " "); }

    // reuse T_ind to find first character after fractional seconds
    T_ind = simple_time_string.find_first_of(':');
    T_ind = simple_time_string.find_first_not_of(":.0123456789", T_ind);

    if (T_ind != std::string::npos)
        { simple_time_string.erase(T_ind); }

    ptime time(time_from_string(simple_time_string));

//    time += parse_delimited_time_duration<boost::posix_time::time_duration>(time_str.substr(sign_ind + 1));

    ptime utc_epoch(boost::gregorian::date(1970,1,1));
    boost::posix_time::time_duration diff(time - utc_epoch);

    return {diff.total_seconds(), diff.total_milliseconds() - diff.total_seconds() * 1000};
  // Bouml preserved body end 00033C0D
}


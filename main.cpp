#include <iostream>

#include "boost/timer/timer.hpp"

#include "input/tagging.hpp"
#include "input/configuration.hpp"
#include "other/filesystem_utilities.hpp"
#include "input/directory.hpp"
#include "output/directory.hpp"
#include "input/fiber_map.hpp"
#include "input/node_paths.hpp"
#include "input/switches.hpp"
#include "input/data_node_value_order.hpp"
#include "other/file_data.hpp"
#include "other/xml_file.hpp"
#include "other/data_nodes.hpp"
#include "output/file_setup.hpp"
#include "output/netCDF_file.hpp"
#include "other/temperature_calibration.hpp"

int main()
{
    // This starts measuring time from here on and prints information about it to the console at the end of execution
    boost::timer::auto_cpu_timer t;

    input::tagging tags
    (
    {
        {input::tag_id::INPUT_DIR, "input_dir"},
        {input::tag_id::OUTPUT_FILE, "output_dir"},
        {input::tag_id::OUTPUT_FILE_NAME, "output_file_name"},
        {input::tag_id::CHANNEL, "channel"},
        {input::tag_id::TIME, "time"},
        {input::tag_id::COLD_BATH, "cold_bath"},
        {input::tag_id::WARM_BATH, "warm_bath"},
        {input::tag_id::DATA, "data"},
        {input::tag_id::DATA_ORDER, "value_order"},
        {input::tag_id::C_B_BEGIN, "cold_bath_begin"},
        {input::tag_id::W_B_BEGIN, "warm_bath_begin"},
        {input::tag_id::C_B_END, "cold_bath_end"},
        {input::tag_id::W_B_END, "warm_bath_end"},
        {input::tag_id::SIMPLE_CALIB, "do_simple_calibration"},
        {input::tag_id::SINGLE_END_CALIB, "do_single_ended_calibration"}
    },
    {
        {input::tag_id::INT_NODES, {"paths_to_integer_value_nodes:", ":paths_to_integer_value_nodes"}},
        {input::tag_id::REAL_NODES, {"paths_to_floating_number_nodes:", ":paths_to_floating_number_nodes"}},
        {input::tag_id::STRING_NODES, {"paths_to_text_nodes:", ":paths_to_text_nodes"}}
    },
    {
        {input::tag_id::MAPPING, {"mapping_tags:", ":mapping_tags"}}
    }
    );

    input::configuration config(filesystem_utilities::read_file("config.txt"), tags);

    input::directory in_dir(config);

    output::directory out_dir(config);

    input::fiber_map mapping(config, tags);

    input::node_paths n_paths(config, tags);

    input::switches swits(config, tags, mapping);

    input::data_node_value_order data_order(config, tags);

    std::shared_ptr<xml_file> file(new xml_file(filesystem_utilities::read_file(*(in_dir.begin()))));

    try { tags.values.at(input::DATA); }
    catch(const std::out_of_range & exc)
    {
        std::cerr << "\nError: The data nodes' tag_id couldn't be found!\n" << std::endl;
        throw;
    }

    data_nodes d_nodes(n_paths.with_tag().at(tags.values.at(input::DATA)).second, file);

    file_data f_data(n_paths, file, data_order, tags, d_nodes);

    std::string finishing_appendix = f_data.strings().at("time_string");

    size_t erase_pos = finishing_appendix.find_first_of(":-./+");

    while (erase_pos != std::string::npos)
    {
        finishing_appendix.erase(erase_pos, 1);
        erase_pos = finishing_appendix.find_first_of(":-./+", erase_pos);
    }

    output::file_setup f_setup(n_paths, swits, tags, config, out_dir, mapping, f_data.laf(), finishing_appendix, in_dir.num_files());

    output::netCDF_file nc_file(f_setup);

    temperature_calibration calib(mapping.indices(f_data.laf()), tags);

//    int i = 0;  && i < 1
    for (auto f_iter = file_paths_iterator(in_dir.path()); f_iter != file_paths_iterator(); ++f_iter)
    {
        file->new_file(filesystem_utilities::read_file(*f_iter));

        f_data = file_data(n_paths, file, data_order, tags, d_nodes);

        nc_file.write(f_data);

        if (swits.data().at(input::SIMPLE_CALIB).second)
            { nc_file.write(calib.simple_calibration(f_data, tags), input::SIMPLE_CALIB, swits); }

        if (swits.data().at(input::SINGLE_END_CALIB).second)
            { nc_file.write(calib.single_ended_calibration(f_data, tags), input::SINGLE_END_CALIB, swits); }

        nc_file.finished_file();

//        ++i;
    }

    nc_file.close_and_rename();

    return 0;
}

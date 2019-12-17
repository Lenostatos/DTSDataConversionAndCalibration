
#include "file_setup.hpp"

#include <iostream>
#include <type_traits>

#include "../input/node_paths.hpp"
#include "../input/switches.hpp"
#include "../input/tagging.hpp"
#include "../input/configuration.hpp"
#include "../input/fiber_map.hpp"
#include "directory.hpp"
#include "../other/filesystem_utilities.hpp"
#include "../other/laf_t.hpp"
#include "../other/data_t.hpp"

namespace output {

file_setup::file_setup(const input::node_paths & n_paths,
                       const input::switches & swits,
                       const input::tagging & tags,
                       const input::configuration & config,
                       const directory & out_dir,
                       const input::fiber_map & mapping,
                       const std::vector < laf_t > & laf_vals,
                       const std::string & finishing_appendix,
                       size_t num_in_files)
: laf_values_(laf_vals), mapping_strings_(mapping.string_vector(laf_vals)), mapping_indices_(mapping.indices(laf_vals)),
  file_name(filesystem_utilities::append_to_path(out_dir.path(), config.values().at(input::OUTPUT_FILE_NAME) + "_unfinished.nc")),
  finishing_appendix(finishing_appendix)
{
  // Bouml preserved body begin 0002168D

    dimensions_.insert({TIME, {tags.values.at(input::TIME), num_in_files}});
    dimensions_.insert({LAF, {"length_along_fiber", laf_vals.size()}});
    dimensions_.insert({TIME_VALUE, {"time_value", 2}});
    dimensions_.insert({MAPPING_TAGS, {"mapping_tags", mapping.data().size()}});
    dimensions_.insert({MAPPING_INDICES, {"mapping_indices", 2}});

    for (const auto & tag_w_nt_and_np : n_paths.with_tag())
    {
        switch (tag_w_nt_and_np.second.first)
        {
        case input::STRING:
            variables_.insert({tag_w_nt_and_np.first, {STRING, {TIME}}});
            break;
        case input::INT:
            variables_.insert({tag_w_nt_and_np.first, {INT, {TIME}}});
            break;
        case input::REAL:
            variables_.insert({tag_w_nt_and_np.first, {DOUBLE, {TIME}}});
            break;
        case input::SPECIAL:
            if (tag_w_nt_and_np.first == tags.values.at(input::TIME))
                { variables_.insert({tag_w_nt_and_np.first, {INT64, {TIME, TIME_VALUE}}}); }
            else if (tag_w_nt_and_np.first == tags.values.at(input::DATA))
            {
                variables_.insert({dimensions_.at(LAF).first, {(typeid(laf_t) == typeid(float) ? FLOAT : DOUBLE), {LAF}}});
                variables_.insert({"stokes", {(typeid(data_t) == typeid(float) ? FLOAT : DOUBLE), {TIME, LAF}}});
                variables_.insert({"anti-stokes", {(typeid(data_t) == typeid(float) ? FLOAT : DOUBLE), {TIME, LAF}}});
                variables_.insert({"temperature", {(typeid(data_t) == typeid(float) ? FLOAT : DOUBLE), {TIME, LAF}}});
            }
            else {}
            break;
        }
    }

    variables_.insert({"mapping_strings", {STRING, {LAF}}});
    variables_.insert({dimensions_.at(MAPPING_TAGS).first, {STRING, {MAPPING_TAGS}}});
    variables_.insert({dimensions_.at(MAPPING_INDICES).first, {UINT, {MAPPING_TAGS, MAPPING_INDICES}}});

    if (swits.data().at(input::SIMPLE_CALIB).second)
        { variables_.insert({swits.data().at(input::SIMPLE_CALIB).first, {(typeid(data_t) == typeid(float) ? FLOAT : DOUBLE), {TIME, LAF}}}); }

    if (swits.data().at(input::SINGLE_END_CALIB).second)
        { variables_.insert({swits.data().at(input::SINGLE_END_CALIB).first, {(typeid(data_t) == typeid(float) ? FLOAT : DOUBLE), {TIME, LAF}}}); }

  // Bouml preserved body end 0002168D
}

} // namespace output

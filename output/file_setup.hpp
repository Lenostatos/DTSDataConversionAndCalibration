#ifndef OUTPUT_FILE_SETUP_H
#define OUTPUT_FILE_SETUP_H


#include <map>
#include <string>
#include <vector>

#include "var_type_id.hpp"
#include "dimension_id.hpp"
#include "../other/laf_t.hpp"
#include "../input/tag.hpp"

namespace input { class node_paths; }
namespace input { class switches; }
namespace input { class tagging; }
namespace input { class configuration; }
namespace input { class fiber_map; }
namespace output { class directory; }

namespace output {

class file_setup
{
    private:
    std::map<dimension_id, std::pair< std::string, size_t >> dimensions_;

    std::map<std::string, std::pair < var_type_id, std::vector < dimension_id >>> variables_;

    std::vector < laf_t > laf_values_;
    std::vector < std::string > mapping_strings_;
    std::multimap < input::tag, std::pair < size_t, size_t >> mapping_indices_;


    public:
    const std::string file_name;

    const std::string finishing_appendix;

    file_setup(const input::node_paths & n_paths,
               const input::switches & swits,
               const input::tagging & tags,
               const input::configuration & config,
               const directory & out_dir,
               const input::fiber_map & mapping,
               const std::vector < laf_t > & laf_vals,
               const std::string & finishing_appendix,
               size_t num_in_files);

    const std::map < dimension_id, std::pair< std::string, size_t >> & dimensions() const
        { return dimensions_; }

    const std::map < std::string, std::pair < var_type_id, std::vector < dimension_id >> > & variables() const
        { return variables_; }

    const std::vector < laf_t > & laf_values() const
        { return laf_values_; }
    const std::vector < std::string > & mapping_strings() const
        { return mapping_strings_; }
    const std::multimap < input::tag, std::pair < size_t, size_t >> & mapping_indices() const
        { return mapping_indices_; }

};

} // namespace output

#endif

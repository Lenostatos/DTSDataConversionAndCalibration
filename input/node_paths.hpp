#ifndef INPUT_NODE_PATHS_H
#define INPUT_NODE_PATHS_H


#include <map>

#include "tag.hpp"
#include "node_value_type.hpp"
#include "tagging.hpp"
#include "../other/node_path.hpp"

namespace input { class configuration; }
class xml_file;

namespace input {

/// Holds the node paths specified in the configuration.
class node_paths
{
    private:
    std::map<tag, std::pair < node_value_type, node_path >> with_tag_;


    public:
    node_paths(const configuration & config, const tagging & tags);

    const std::map < tag, std::pair < node_value_type, node_path >> & with_tag() const
        { return with_tag_; }

    void validate_against(const ::xml_file & xml) const;

};

} // namespace input

#endif

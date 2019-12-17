#ifndef INPUT_TAGGING_H
#define INPUT_TAGGING_H


#include <map>
#include "tag_id.hpp"
#include "tag.hpp"
#include <string>

namespace input {

/// Maps tags and pairs of tag section begin and end markers to their IDs.
class tagging
{
    public:
    const std::map<tag_id, tag> values;

    const std::map<tag_id, std::pair < std::string, std::string >> sections;

    const std::map<tag_id, std::pair < std::string, std::string >> multi_sections;

    tagging(const std::map < tag_id, tag > & values, const std::map < tag_id, std::pair < std::string, std::string >> & sections, const std::map < tag_id, std::pair < std::string, std::string >> & multi_sections)
    : values(values), sections(sections), multi_sections(multi_sections) {}

};

} // namespace input

#endif

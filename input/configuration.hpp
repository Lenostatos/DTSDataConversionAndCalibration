#ifndef INPUT_CONFIGURATION_H
#define INPUT_CONFIGURATION_H


#include <map>
#include "tag_id.hpp"
#include "tag_value.hpp"
#include "tag.hpp"
#include <string>

namespace input { class tagging; }

namespace input {

/// Maps the tag values and the tag sections' tags and tag values of a configuration file to their respective tag IDs.
class configuration
{
    private:
    std::map<tag_id, tag_value> values_;

    std::map<tag_id, std::map < tag, tag_value >> sections_;

    std::map<tag_id, std::multimap < tag, tag_value >> multi_sections_;


    public:
    configuration(const std::string & config_file, const tagging & tags);

    const std::map < tag_id, tag_value > & values() const
        {return values_;};

    const std::map < tag_id, std::map < tag, tag_value >> & sections() const
        {return sections_;};

    const std::map < tag_id, std::multimap < tag, tag_value >> & multi_sections() const
        {return multi_sections_;};


    private:
    /// Gets the value of a tag in a configuration file.
    static tag_value extract_tag_value(const std::string & tag, const std::string & config_file, char comment_character = '#');

    /// Gets tags and their values in a marked section of a configuration file.
    static std::multimap < tag, tag_value > extract_tags_and_values(const std::pair < std::string, std::string > & section_begin_and_end, const std::string & config_file, char comment_character = '#');

};

} // namespace input

#endif

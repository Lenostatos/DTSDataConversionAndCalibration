#ifndef INPUT_SWITCHES_H
#define INPUT_SWITCHES_H


#include <map>
#include "tag_id.hpp"

namespace input { class configuration; }
namespace input { class tagging; }
namespace input { class fiber_map; }

namespace input {

/// Aggregates all labels with true or false values.
class switches
{
    private:
    std::map < tag_id, std::pair < std::string, bool >> data_;


    public:
    switches(const configuration & config, const tagging & tags, const fiber_map & map);

    const std::map < tag_id, std::pair < std::string, bool >> & data() const
        { return data_; }

};

} // namespace input

#endif

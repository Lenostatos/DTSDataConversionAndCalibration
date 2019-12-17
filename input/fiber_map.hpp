#ifndef INPUT_FIBER_MAP_H
#define INPUT_FIBER_MAP_H


#include <map>
#include <string>
#include <vector>

#include "tag.hpp"
#include "tag_id.hpp"
#include "../other/laf_t.hpp"

namespace input { class configuration; }
namespace input { class tagging; }

namespace input {

class fiber_map
{
    private:
    std::multimap<tag, std::pair < double, double >> data_;

    std::map < tag_id, bool > bath_exists_;


    public:
    fiber_map(const configuration & config, const tagging & tags);

    const std::multimap < tag, std::pair < double, double >> & data() const
        { return data_; }

    const std::map < tag_id, bool > & bath_exists() const
        { return bath_exists_; }

    std::vector < std::string > string_vector(const std::vector < laf_t > & reference_fiber) const;

    std::multimap < tag, std::pair < size_t, size_t >> indices(const std::vector < laf_t > & reference_fiber) const;

};

} // namespace input

#endif

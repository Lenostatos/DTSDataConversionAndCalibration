#ifndef _TEMPERATURE_CALIBRATION_H
#define _TEMPERATURE_CALIBRATION_H


#include <vector>
#include <map>

#include "../input/tag.hpp"
#include "data_t.hpp"

class file_data;
namespace input { class fiber_map; }
namespace input { class tagging; }

class temperature_calibration
{
private:
    // If C_B_BEGIN and W_B_END exist this vector contains indices for the existing baths in this order: C_B_BEGIN W_B_END [C_B_END] [W_B_BEGIN]
    // Else if W_B_BEGIN and C_B_END exist: W_B_BEGIN C_B_END [W_B_END] [C_B_BEGIN]
    // Else this vector is empty
    std::vector < std::pair < std::string, std::pair < size_t, size_t >>> bath_inds_;

    std::map < input::tag, input::tag > refs_;


public:
    temperature_calibration(const std::multimap < input::tag, std::pair < size_t, size_t >> & map_inds, const input::tagging & tags);

    std::vector < data_t > simple_calibration(const file_data & data, const input::tagging & tags);

    std::vector < data_t > single_ended_calibration(const file_data & data, const input::tagging & tags);

};

#endif

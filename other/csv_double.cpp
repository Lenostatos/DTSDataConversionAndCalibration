
#include "csv_double.hpp"

csv_double::csv_double(const std::string & csv_str, char separator)
{
  // Bouml preserved body begin 0003080D

    size_t next_sep_ind = 0;
    size_t temp_sep_ind = 0;

    do
    {
        values.push_back(std::stod(&csv_str[next_sep_ind], &temp_sep_ind));
        next_sep_ind += temp_sep_ind;
        next_sep_ind = csv_str.find(separator, next_sep_ind);
    }
    while (next_sep_ind++ != std::string::npos);

  // Bouml preserved body end 0003080D

}


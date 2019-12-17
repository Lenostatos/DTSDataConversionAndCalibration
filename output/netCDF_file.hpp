#ifndef OUTPUT_NETCDF_FILE_H
#define OUTPUT_NETCDF_FILE_H


#include <map>
#include <string>

#include "netcdf"

#include "dimension_id.hpp"
#include "../input/tag_id.hpp"
#include "../other/data_t.hpp"

namespace output { class file_setup; }
class file_data;
namespace input { class tagging; }
namespace input { class switches; }
class temperature_calibration;

namespace output {

class netCDF_file
{
    private:
    std::string file_name_;
    /// String to be appended to file name after finishing writing of data and after closing the file.
    std::string finishing_appendix_;

    netCDF::NcFile file_;

    std::map<dimension_id, netCDF::NcDim> dimensions_;

    std::map<std::string, netCDF::NcVar> variables_;

    size_t num_writes_ = 0;


    public:
    explicit netCDF_file(const file_setup & setup);

    const std::map < dimension_id, netCDF::NcDim > & dimensions() const
        { return dimensions_; }

    const std::map < std::string, netCDF::NcVar > & variables() const
        { return variables_; }

    /// Writes extracted data to the output file.
    void write(const ::file_data & data);

    /// Writes calculated data to the output file.
    void write(const std::vector < data_t > & calc_data, input::tag_id which, const input::switches & swits);

    /// Shall be called after writing all the data of an input file.
    void finished_file() noexcept { ++num_writes_; };

    /// Closes the output file and changes it's appendix from "unfinished" to the appendix specified in finishing_appendix.
    void close_and_rename();

};

} // namespace output

#endif

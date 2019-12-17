
#include "netCDF_file.hpp"

#include "file_setup.hpp"
#include "../other/file_data.hpp"
#include "../input/tagging.hpp"
#include "../input/switches.hpp"
#include "../other/temperature_calibration.hpp"
#include "../other/filesystem_utilities.hpp"

namespace output {

netCDF_file::netCDF_file(const file_setup & setup)
: file_name_(setup.file_name), finishing_appendix_(setup.finishing_appendix), file_(setup.file_name, netCDF::NcFile::replace),//, netCDF::NcFile::FileFormat::nc4classic)
  num_writes_(0)
{
  // Bouml preserved body begin 0002188D

    for (const auto & dim : setup.dimensions())
        { dimensions_.insert({dim.first, file_.addDim(dim.second.first, dim.second.second)}); }

    std::map< var_type_id, netCDF::NcType > nc_types =
    {
        {BYTE, netCDF::NcType::nc_BYTE},
        {CHAR, netCDF::NcType::nc_CHAR},
        {SHORT, netCDF::NcType::nc_SHORT},
        {INT, netCDF::NcType::nc_INT},
        {FLOAT, netCDF::NcType::nc_FLOAT},
        {DOUBLE, netCDF::NcType::nc_DOUBLE},
        {UBYTE, netCDF::NcType::nc_UBYTE},
        {USHORT, netCDF::NcType::nc_USHORT},
        {UINT, netCDF::NcType::nc_UINT},
        {INT64, netCDF::NcType::nc_INT64},
        {UINT64, netCDF::NcType::nc_UINT64},
        {STRING, netCDF::NcType::nc_STRING}
    };

    for (const auto & var : setup.variables())
    {
        std::vector< netCDF::NcDim > nc_dims;
        for (dimension_id dim_id : var.second.second)
            { nc_dims.push_back(dimensions_.at(dim_id)); }

        variables_.insert({var.first, file_.addVar(var.first, nc_types.at(var.second.first), nc_dims)});
    }

    // Add attributes here!

    // Write LAF values to file
    variables_.at(dimensions_.at(LAF).getName()).putVar(std::vector<size_t>({0}), std::vector<size_t>({dimensions_.at(LAF).getSize()}), setup.laf_values().data());

    // Write mapping strings to file
    for (size_t i = 0; i < dimensions_.at(LAF).getSize(); ++i)
        { variables_.at("mapping_strings").putVar(std::vector<size_t>({i}), setup.mapping_strings()[i]); }

    // Write mapping tags and indices to file
    size_t i = 0;
    for (const auto & map_tag_w_ind : setup.mapping_indices())
    {
        variables_.at(dimensions_.at(MAPPING_TAGS).getName()).putVar(std::vector<size_t>({i}), map_tag_w_ind.first);

        variables_.at(dimensions_.at(MAPPING_INDICES).getName()).putVar(std::vector<size_t>({i,0}), (long long unsigned int) map_tag_w_ind.second.first); // type conversion is done because netCDF doesn't take long unsigned int (aka size_t)
        variables_.at(dimensions_.at(MAPPING_INDICES).getName()).putVar(std::vector<size_t>({i,1}), (long long unsigned int) map_tag_w_ind.second.second);

        ++i;
    }

  // Bouml preserved body end 0002188D
}

void netCDF_file::write(const ::file_data & data)
{
  // Bouml preserved body begin 00021E0D

    variables_.at(dimensions_.at(TIME).getName()).putVar(std::vector<size_t>({num_writes_,0}), data.time().first);
    variables_.at(dimensions_.at(TIME).getName()).putVar(std::vector<size_t>({num_writes_,1}), data.time().second);

    variables_.at("stokes").putVar(std::vector<size_t>({num_writes_,0}), std::vector<size_t>({1,dimensions_.at(LAF).getSize()}), data.array_data()[0].data());
    variables_.at("anti-stokes").putVar(std::vector<size_t>({num_writes_,0}), std::vector<size_t>({1,dimensions_.at(LAF).getSize()}), data.array_data()[1].data());
    variables_.at("temperature").putVar(std::vector<size_t>({num_writes_,0}), std::vector<size_t>({1,dimensions_.at(LAF).getSize()}), data.array_data()[2].data());

    for (const auto & str_datum : data.strings())
        { variables_.at(str_datum.first).putVar(std::vector<size_t>({num_writes_}), str_datum.second); }

    for (const auto & int_datum : data.ints())
        { variables_.at(int_datum.first).putVar(std::vector<size_t>({num_writes_}), int_datum.second); }

    for (const auto & real_datum : data.reals())
        { variables_.at(real_datum.first).putVar(std::vector<size_t>({num_writes_}), real_datum.second); }

  // Bouml preserved body end 00021E0D
}

void netCDF_file::write(const std::vector < data_t > & calc_data, input::tag_id which, const input::switches & swits)
{
  // Bouml preserved body begin 00021E8D

    if (which == input::SIMPLE_CALIB)
    {
        variables_.at(swits.data().at(which).first).putVar(std::vector<size_t>({num_writes_,0}), std::vector<size_t>({1, dimensions_.at(LAF).getSize()}), calc_data.data());
    }

    if (which == input::SINGLE_END_CALIB)
    {
        variables_.at(swits.data().at(which).first).putVar(std::vector<size_t>({num_writes_,0}), std::vector<size_t>({1, dimensions_.at(LAF).getSize()}), calc_data.data());
    }

  // Bouml preserved body end 00021E8D
}

void netCDF_file::close_and_rename()
{
  // Bouml preserved body begin 0003768D

    file_.close();

    std::string new_path = file_name_;

    size_t end_pos = new_path.rfind("unfinished.nc");

    new_path.replace(end_pos, std::string::npos, finishing_appendix_ + ".nc");

    try { filesystem_utilities::rename(file_name_, new_path); }
    catch (const std::exception & exc)
    {
        std::cerr << "\nError: Couldn't rename the output file when finished!:" << std::endl;
        std::cerr << exc.what() << '\n' << std::endl;
    }

  // Bouml preserved body end 0003768D
}


} // namespace output

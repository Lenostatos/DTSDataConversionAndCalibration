
#include "temperature_calibration.hpp"

#include <iostream>
#include <cassert>
#include <cmath>

#include "lapackpp/gmd.h"
#include "lapackpp/laslv.h"

#include "file_data.hpp"
#include "../input/fiber_map.hpp"
#include "../input/tagging.hpp"
#include "my_exceptions/tag_error.hpp"

temperature_calibration::temperature_calibration(const std::multimap < input::tag, std::pair < size_t, size_t >> & map_inds, const input::tagging & tags)
{
    if (map_inds.count(tags.values.at(input::C_B_BEGIN)) == 1 && map_inds.count(tags.values.at(input::W_B_END)) == 1)
    {
        bath_inds_.push_back({tags.values.at(input::C_B_BEGIN), map_inds.find(tags.values.at(input::C_B_BEGIN))->second});
        bath_inds_.push_back({tags.values.at(input::W_B_END), map_inds.find(tags.values.at(input::W_B_END))->second});

        if (map_inds.count(tags.values.at(input::C_B_END)) == 1)
            { bath_inds_.push_back({tags.values.at(input::C_B_END), map_inds.find(tags.values.at(input::C_B_END))->second}); }

        if (map_inds.count(tags.values.at(input::W_B_BEGIN)) == 1)
            { bath_inds_.push_back({tags.values.at(input::W_B_BEGIN), map_inds.find(tags.values.at(input::W_B_BEGIN))->second}); }
    }
    else if (map_inds.count(tags.values.at(input::W_B_BEGIN)) == 1 && map_inds.count(tags.values.at(input::C_B_END)) == 1)
    {
        bath_inds_.push_back({tags.values.at(input::W_B_BEGIN), map_inds.find(tags.values.at(input::W_B_BEGIN))->second});
        bath_inds_.push_back({tags.values.at(input::C_B_END), map_inds.find(tags.values.at(input::C_B_END))->second});

        if (map_inds.count(tags.values.at(input::W_B_END)) == 1)
            { bath_inds_.push_back({tags.values.at(input::W_B_END), map_inds.find(tags.values.at(input::W_B_END))->second}); }

        if (map_inds.count(tags.values.at(input::C_B_BEGIN)) == 1)
            { bath_inds_.push_back({tags.values.at(input::C_B_BEGIN), map_inds.find(tags.values.at(input::C_B_BEGIN))->second}); }
    }
    else {}

    refs_ =
    {
        {tags.values.at(input::C_B_BEGIN), tags.values.at(input::COLD_BATH)},
        {tags.values.at(input::C_B_END), tags.values.at(input::COLD_BATH)},
        {tags.values.at(input::W_B_BEGIN), tags.values.at(input::WARM_BATH)},
        {tags.values.at(input::W_B_END), tags.values.at(input::WARM_BATH)}
    };
}

std::vector < data_t > temperature_calibration::simple_calibration(const file_data & data, const input::tagging & tags)
{
  // Bouml preserved body begin 00021F8D

    // first get the mean temperature of the bath at the beginning of the fiber
    data_t begin_bath_mean = data_t(0);

    for (size_t i = bath_inds_[0].second.first; i <= bath_inds_[0].second.second; ++i)
        { begin_bath_mean += data.array_data()[2][i]; }

    begin_bath_mean /= bath_inds_[0].second.second - bath_inds_[0].second.first + 1;

    // then calculate that bath's offset from the reference temperature
    data_t begin_bath_offset = data.reals().at(refs_.at(bath_inds_[0].first)) - begin_bath_mean;

    // and add it to all temperatures
    std::vector < data_t > calib_temp(data.array_data()[2].size());

    for (size_t i = 0; i < calib_temp.size(); ++i)
        { calib_temp[i] = data.array_data()[2][i] + begin_bath_offset; }

    // now get the mean temperature of the bath at the end of the fiber
    data_t end_bath_mean = data_t(0);

    for (size_t i = bath_inds_[1].second.first; i <= bath_inds_[1].second.second; ++i)
        { end_bath_mean += calib_temp[i]; }

    end_bath_mean /= bath_inds_[1].second.second - bath_inds_[1].second.first + 1;

    // then get the offset there
    data_t end_bath_offset = data.reals().at(refs_.at(bath_inds_[1].first)) - end_bath_mean;

    // finally add a fraction of that offset to all temperatures in between the baths
    size_t fraction = end_bath_offset / (bath_inds_[1].second.first - bath_inds_[0].second.second);
    size_t fraction_multi = 1; // multiplier for the fraction

    for (size_t i = bath_inds_[0].second.second + 1; i < bath_inds_[1].second.first; ++i, ++fraction_multi)
        { calib_temp[i] += fraction_multi * fraction; }

    // as a little extra also subtract such fractions from the temperatures before the bath at the beginning...
    fraction_multi = 1;
    for (size_t i = bath_inds_[0].second.first - 1; i < bath_inds_[0].second.first; --i, ++fraction_multi) // I can't do "i >= 0" here because i is unsigned and turnes positive when reaching zero - 1
        { calib_temp[i] -= fraction_multi * fraction; }

    // ...and add them to the temperatures after the bath at the end
    fraction_multi = bath_inds_[1].second.first - bath_inds_[0].second.second + 1;
    for (size_t i = bath_inds_[1].second.second + 1; i < calib_temp.size(); ++i, ++fraction_multi)
        { calib_temp[i] += fraction_multi * fraction; }

    // also add the offset to all temperatures at the bath at the end
    for (size_t i = bath_inds_[1].second.first; i <= bath_inds_[1].second.second; ++i)
        { calib_temp[i] += end_bath_offset; }

    return calib_temp;

  // Bouml preserved body end 00021F8D
}

std::vector < data_t > temperature_calibration::single_ended_calibration(const file_data & data, const input::tagging & tags)
{
  // Bouml preserved body begin 0002200D

    // I have to ensure that the fourth bath is somewhere in between the other baths so that i can use it for validation!
    // -> Just exclude the validation bath in the config file!

    // Get the mean bath positions
    std::vector < laf_t > mean_bath_laf;

    for (const auto & bath_ind : bath_inds_)
    {
        laf_t mean_laf = 0;

        for (size_t i = bath_ind.second.first; i <= bath_ind.second.second; ++i)
            { mean_laf += data.laf()[i]; }

        mean_laf /= bath_ind.second.second - bath_ind.second.first + 1;

        mean_bath_laf.push_back(mean_laf);
    }

    // Calculate the ln(P_S/P_aS) for the whole fiber
    assert(data.array_data()[0].size() == data.array_data()[1].size() && "The number of Stokes and Anti-Stokes values differ!");

    std::vector < data_t > ln_PS_PaS(data.array_data()[0].size());

    for (size_t i = 0; i < ln_PS_PaS.size(); ++i)
        { ln_PS_PaS[i] = log(data.array_data()[0][i] / data.array_data()[1][i]); }

    // Get the mean ln(P_S/P_aS) of the bath sections
    std::vector < data_t > mean_bath_ln;

    for (const auto & bath_ind : bath_inds_)
    {
        laf_t mean_ln = 0;

        for (size_t i = bath_ind.second.first; i <= bath_ind.second.second; ++i)
            { mean_ln += ln_PS_PaS[i]; }

        mean_ln /= bath_ind.second.second - bath_ind.second.first + 1;

        mean_bath_ln.push_back(mean_ln);
    }

    // Get the reference temperatures (and convert them to Kelvin)
    std::vector < double > ref_temp;

    for (const auto & bath_ind : bath_inds_)
    {
        ref_temp.push_back(data.reals().at(refs_.at(bath_ind.first)));
        ref_temp.back() += 273.15; // Source: Wikipedia
    }

    // Get the three calibration parameters with the linear equation system from Hausner et al. 2011
    std::vector < double > A_vec;

    for (size_t i = 0; i < 3; ++i)
    {
        A_vec.push_back(1);
        A_vec.push_back(-ref_temp[i]);
        A_vec.push_back(ref_temp[i] * mean_bath_laf[i]);
    }

    LaGenMatDouble A(A_vec.data(), 3, 3, true);

    std::vector < double > calib_param(3);

    LaGenMatDouble x(calib_param.data(), calib_param.size(), 1);

    std::vector < double > b_vec;

    for (size_t i = 0; i < 3; ++i)
        { b_vec.push_back(ref_temp[i] * mean_bath_ln[i]); }

    LaGenMatDouble b(b_vec.data(), b_vec.size(), 1, true);

    LaLinearSolve(A, x, b);

    // Finally calibrate (reusing the ln_PS_PaS vector)
    for (size_t i = 0; i < ln_PS_PaS.size(); ++i)
    {
        ln_PS_PaS[i] = (calib_param[0] / (ln_PS_PaS[i] + calib_param[1] - calib_param[2] * data.laf()[i])) - 273.15;
    }

    return ln_PS_PaS;

  // Bouml preserved body end 0002200D
}

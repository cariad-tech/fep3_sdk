/**
 * @copyright
 * @verbatim
 * Copyright 2023 CARIAD SE.
 *
This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */
#pragma once

#include <string>
#include <exception>

#include <a_util/strings.h>
#include <fep3/core.h>
#include <fep3/base/properties/property_type.h>

struct Delay
{
    // Contains multiplication factors to get from s/min/h to s
    enum Prefix
    {
        s = 1, // default unit
        min = 60,
        h = 3600
    };

    // CTOR's
    Delay() = default;
    Delay(double value) : _value(value)
    {
    }

    // Return current value in seconds
    double getValue() const { return _value; }

    // Set value based on a string which can consist of a number followed by a unit (s/min/h)
    fep3::Result setValue(std::string value_str)
    {
        a_util::strings::trim(value_str); // remove trailing white space

        try
        {
            _value = std::stod(value_str);
        }
        catch (const std::invalid_argument&)
        {
            // hopefully some valid unit can be found
            const std::string unit_s = "s", unit_min = "min", unit_h = "h";
            if (const auto pos = value_str.find(unit_s);
                pos != std::string::npos)
            {
                value_str.erase(pos, unit_s.length());

                return delayToSeconds(value_str, Prefix::s);
            }
            else if (const size_t pos = value_str.find(unit_min);
                pos != std::string::npos)
            {
                value_str.erase(pos, unit_min.length());

                return delayToSeconds(value_str, Prefix::min);
            }
            else if (const size_t pos = value_str.find(unit_h);
                pos != std::string::npos)
            {
                value_str.erase(pos, unit_h.length());

                return delayToSeconds(value_str, Prefix::h);
            }
            else
            { // something went wrong
                return fep3::ERR_FAILED;
            }
        }
        catch (...)
        {
            return fep3::ERR_FAILED;
        }
            
        return {};
    }

private:
    fep3::Result delayToSeconds(const std::string& delay, Prefix unit)
    {
        try
        {
            _value = std::stod(delay) * unit;
        }
        catch (const std::exception& e)
        {
            return CREATE_ERROR_DESCRIPTION(fep3::ERR_INVALID_ARG, e.what());
        }

        return {};
    }

private:
    double _value; // Delay value in seconds
};

// Needs to be defined for custom structs or types that are not yet implemented in the FEP SDK
template <>
struct fep3::base::DefaultPropertyTypeConversion<Delay>
{
    static Delay fromString(const std::string& from)
    {
        Delay delay(0);
        if (!delay.setValue(from))
        {
            std::string msg =
                a_util::strings::format("Cannot convert string '%s' to Delay. Must be a number optionally followed by a unit [s|min|h].", from.c_str());
            throw std::runtime_error(msg);
        }
        return delay;
    }

    static bool fromString(const std::string& from, Delay& delay)
    {
        if (!delay.setValue(from))
        {
            return false;
        }
        return true;
    }

    static std::string toString(const Delay& delay)
    {
        return std::to_string(delay.getValue()) + "s";
    }
};

// We also need to provide a property type definition for custom properties
template <>
struct fep3::base::PropertyType<Delay>
{
    inline static std::string getTypeName()
    {
        return "delay";
    }
};

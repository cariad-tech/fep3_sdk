/**
 * @copyright
 * @verbatim
 * Copyright @ 2023 VW Group. All rights reserved.
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
        h = 3600,
    };

    // CTOR's
    Delay() = default;
    Delay(double value) : _value(value)
    {
    }

    // Return current value in seconds
    double getValue() const { return _value; }

    // Set value based on a string which can consist of a number followed by a unit (s/min/h)
    fep3::Result setValue(const std::string& value_str)
    {
        std::string trimmed = value_str;
        a_util::strings::trim(trimmed); // remove traling white space

        if (a_util::strings::isNumeric<double>(trimmed))
        { // no unit provided, default to seconds
            _value = a_util::strings::toDouble(value_str);
        }
        else
        { // hopefully some valid unit can be found
            if (trimmed.find("s") != std::string::npos)
            {
                a_util::strings::replace(trimmed, "s", "");
                if (!a_util::strings::isNumeric<double>(trimmed))
                {
                    return fep3::ERR_FAILED;
                }
                _value = a_util::strings::toDouble(trimmed) * Prefix::s;
            }
            else if (trimmed.find("min") != std::string::npos)
            {
                a_util::strings::replace(trimmed, "min", "");
                if (!a_util::strings::isNumeric<double>(trimmed))
                {
                    return fep3::ERR_FAILED;
                }
                _value = a_util::strings::toDouble(trimmed) * Prefix::min;
            }
            else if (trimmed.find("h") != std::string::npos)
            {
                a_util::strings::replace(trimmed, "h", "");
                if (!a_util::strings::isNumeric<double>(trimmed))
                {
                    return fep3::ERR_FAILED;
                }
                _value = a_util::strings::toDouble(trimmed) * Prefix::h;
            }
            else
            { // something went wrong
                return fep3::ERR_FAILED;
            }
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
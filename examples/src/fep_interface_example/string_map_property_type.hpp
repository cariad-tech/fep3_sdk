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

#include <a_util/strings.h>
#include <fep3/core.h>
#include <fep3/base/properties/property_type.h>

// Needs to be defined for custom structs or types that are not yet implemented in the FEP SDK
template <>
struct fep3::base::DefaultPropertyTypeConversion<std::map<std::string, std::string>>
{
    static std::map<std::string, std::string> fromString(const std::string& from)
    {
        std::string trimmed(from);
        std::vector<std::string> signal_struct_relation = 
            a_util::strings::split(a_util::strings::trim(trimmed), ";");
        
        std::map<std::string, std::string> out_map;
        for (auto &signal_config : signal_struct_relation)
        {
            std::vector<std::string> signal_config_split = 
                a_util::strings::split(a_util::strings::trim(signal_config), ":");
            out_map[signal_config_split[0]] = signal_config_split[1];
        }
        return out_map;
    }

    static std::string toString(const std::map<std::string, std::string>& sig_map)
    {
        std::string out_str = "";
        for (const auto &[name, ddl_struct] : sig_map)
        {
            out_str += name + ":" + ddl_struct + ";";
        }
        
        if (!out_str.empty())
        {
            out_str.pop_back();
        }
        return out_str;
    }
};

// We also need to provide a property type definition for custom properties
template <>
struct fep3::base::PropertyType<std::map<std::string, std::string>>
{
    inline static std::string getTypeName()
    {
        return "map-string-string";
    }
};
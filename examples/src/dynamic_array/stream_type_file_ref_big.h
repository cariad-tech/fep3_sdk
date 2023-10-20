/**
 * @file
 * @copyright
 * @verbatim
Copyright @ 2023 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#include <fep3/base/sample/data_sample.h>
#include <fep3/base/stream_type/default_stream_type.h>

//Begin(ddl definition of a custom stream type)
class StreamTypeDDLFileRefBig : public fep3::base::arya::StreamType {
public:
    /**
     * @brief Construct a new Stream Type DDL object
     *
     * @param[in] ddlstruct Value for ddl struct this StreamType describes
     * @param[in] fileref Reference to a file which contains the whole ddl description content. Your
     * @p ddlstruct has to be included in this description.
     */
    StreamTypeDDLFileRefBig(const std::string& ddlstruct, const std::string& fileref, size_t max_byte_size)
        : fep3::base::arya::StreamType(
            fep3::base::arya::StreamMetaType("ddl-fileref",
                std::list<std::string>{
            fep3::base::arya::meta_type_prop_name_ddlstruct,
            fep3::base::arya::meta_type_prop_name_ddlfileref,
            fep3::base::arya::meta_type_prop_name_max_byte_size}))
    {
        setProperty(fep3::base::arya::meta_type_prop_name_ddlstruct, ddlstruct, "string");
        setProperty(fep3::base::arya::meta_type_prop_name_ddlfileref, fileref, "string");
        // this is a workaround to get the simulation_bus choose a big QoS profile if using RTI DDS
        // also automatic ddl size calculation and Qos Profile selection is not possible for dynamic arrays
        setProperty(fep3::base::arya::meta_type_prop_name_max_byte_size, std::to_string(max_byte_size), "uint32_t");
    }
};
//End(ddl definition of a custom stream type)


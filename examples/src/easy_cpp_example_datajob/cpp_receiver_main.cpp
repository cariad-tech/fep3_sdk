/**
 * @file
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#include <iostream>
#include <memory>
#include <string>
#include <chrono>

#include <fep3/cpp.h>
#include <ddl/dd/ddstructure.h>

#include "example_ddl_types.h"

using namespace fep3;
using namespace std::chrono_literals;

class EasyCPPReceiverDataJob : public cpp::DataJob
{
public:
    //Implementation of the CTOR!
    //DataJob has no default CTOR
    // you must define job name
    EasyCPPReceiverDataJob()
        : cpp::DataJob("receive_job", 1s )
    {
        //create DataAccess with the reader class
        //this will create a data reader for strings
        _data_reader_string = addDataIn("string_data",
            base::StreamTypeString());

        //create structure data definitions for the structs used within the example
        const auto dd_struct_easy_pos = ddl::DDStructureGenerator<fep3::examples::tEasyPosition>("tEasyPosition")
            .addElement("x_pos", &fep3::examples::tEasyPosition::x_pos)
            .addElement("y_pos", &fep3::examples::tEasyPosition::y_pos)
            .addElement("z_pos", &fep3::examples::tEasyPosition::z_pos);
        const auto dd_struct_easy_struct = ddl::DDStructureGenerator<fep3::examples::tEasyStruct>("tEasyStruct")
            .addElement("tEasyPosition", &fep3::examples::tEasyStruct::pos, dd_struct_easy_pos)
            .addElement("double_value", &fep3::examples::tEasyStruct::double_value);
        //the structure data definition can be used to retrieve the corresponding ddl description
        const auto description = dd_struct_easy_struct.getStructDescription();

        //this will create a data reader for ddl based structures
        _data_reader_ddl = addDataIn("ddl_data",
            base::StreamTypeDDL(fep3::examples::examples_ddl_struct, description));

        //this will create a data reader for dynamic arrays of DDL based structures
        _data_reader_ddl_array = addDataIn("ddl_array",
            base::StreamTypeDDLArray(fep3::examples::examples_ddl_struct, description, 32));

        //this will create a data reader for fixed size uint32_t values
        _data_reader_plain_c_type = addDataIn("plain_c_type_int32_t",
            base::StreamTypePlain<int32_t>());
    }

    fep3::Result process(fep3::Timestamp sim_time_of_execution)
    {
        //print the last value in queue for the plain value
        //the content of the _data_reader_plain_c_type reader queue changes only in processDataIn!
        // processDataIn will receive the content of the data in queues in DataRegistry until they are empty
        Optional<int32_t> received_plain_value;
        *_data_reader_plain_c_type >> received_plain_value;
        if (received_plain_value.has_value())
        {
            FEP3_LOG_INFO("received plain value:" + std::to_string(received_plain_value.value()));
        }

        //receive the string value
        Optional<std::string> received_string_value;
        *_data_reader_string >> received_string_value;
        if (received_string_value.has_value())
        {
            FEP3_LOG_INFO("received string value:" + received_string_value.value());
        }

        //received ddl structure value
        Optional<fep3::examples::tEasyStruct> easy_struct;
        *_data_reader_ddl >> easy_struct;
        if (easy_struct.has_value())
        {
            FEP3_LOG_INFO("received easy struct value: " + easy_to_string(easy_struct.value()));
        }

        //received ddl array structure value
        std::vector<fep3::examples::tEasyStruct> easy_struct_array;
        //if we do not have a fixed size of the array, but a dynamic size
        //we need to get the content with the help of fep3::StdVectorSampleType
        fep3::base::StdVectorSampleType<fep3::examples::tEasyStruct> easy_struct_array_to_receive(easy_struct_array);
        const auto read_sample = _data_reader_ddl_array->popSampleLatest();
        if (read_sample)
        {
            read_sample->read(easy_struct_array_to_receive);
        }
        if (!easy_struct_array.empty())
        {
            FEP3_LOG_INFO(std::string() + "received easy struct array with size of "
                + std::to_string(easy_struct_array.size())
                + " and first value:"
                + easy_to_string(easy_struct_array[0]));
        }
        else
        {
            FEP3_LOG_INFO("not yet received ddl array");
        }

        return {};
    }

    std::string easy_to_string(const fep3::examples::tEasyStruct& easy_struct) const
    {
        std::string res = "{ pos = { " + std::to_string(easy_struct.pos.x_pos) + ", ";
        res += std::to_string(easy_struct.pos.y_pos) + ", ";
        res += std::to_string(easy_struct.pos.z_pos) + " }";
        res += ", double_value = " + std::to_string(easy_struct.double_value) + " }";
        return res;
    }

    //Definitions of the readers you want to use
    core::DataReader* _data_reader_string;
    core::DataReader* _data_reader_ddl;
    core::DataReader* _data_reader_ddl_array;
    core::DataReader* _data_reader_plain_c_type;
};

class EasyCPPReceiverDataJobElement : public cpp::DataJobElement<EasyCPPReceiverDataJob>
{
    fep3::Result load() override
    {
        auto res = cpp::DataJobElement<EasyCPPReceiverDataJob>::load();
        if (res.getErrorCode() != fep3::ERR_NOERROR) return res;
        fep3::arya::IConfigurationService* conf_service = getComponents()->getComponent<fep3::arya::IConfigurationService>();

        if (conf_service)
        {
            int64_t timeout_value = std::chrono::nanoseconds(10s).count();
            FEP3_RETURN_IF_FAILED(base::setPropertyValue(*conf_service, FEP3_RTI_DDS_SIMBUS_DATAWRITER_READY_TIMEOUT, timeout_value));
            FEP3_RETURN_IF_FAILED(base::setPropertyValue(*conf_service, FEP3_RTI_DDS_SIMBUS_MUST_BE_READY_SIGNALS, "*"));
        }
        return {};
    }
};

int main(int argc, const char* argv[])
{
    try
    {
        auto part = cpp::createParticipant<EasyCPPReceiverDataJobElement>(
            argc, argv,
            "demo_cpp_receiver",
            "demo_system");
        return part.exec();
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what();
        return 3;
    }
}

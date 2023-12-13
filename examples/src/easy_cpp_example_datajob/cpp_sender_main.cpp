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

class EasyCPPSenderDataJob : public cpp::DataJob
{
public:
    //Implementation of the CTOR!
    //DataJob has no default CTOR
    // you must define a name of the job -> to create the job name in registry
    EasyCPPSenderDataJob()
        : cpp::DataJob("sender_job", 1s)
    {
        //register the properties
        registerPropertyVariable(_prop_to_send_as_integer, "integer_value");
        registerPropertyVariable(_prop_to_send_as_string, "string_value");
        registerPropertyVariable(_prop_to_send_as_double, "double_value");
        registerPropertyVariable(_prop_as_string_array, "string_array_value");

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

        //create DataAccess with the writer class
        //this will create a data writer for strings
        _data_writer_string = addDataOut("string_data",
            base::StreamTypeString());

        //this will create a data writer for ddl based structures
        _data_writer_ddl = addDataOut("ddl_data",
            base::StreamTypeDDL(fep3::examples::examples_ddl_struct, description));

        //this will create a data writer for dynamic arrays of DDL based structures
        _data_writer_ddl_array = addDataOut("ddl_array",
            base::StreamTypeDDLArray(fep3::examples::examples_ddl_struct, description, 32));

        //this will create a data writer for fixed size uint32_t values
        _data_writer_plain_c_type = addDataOut("plain_c_type_int32_t",
            base::StreamTypePlain<int32_t>());
    }

    fep3::Result process(fep3::Timestamp sim_time_of_execution) override
    {
        //we want to read the current properties to our propertyvariables
        //so we can use them for sending the data
        updatePropertyVariables();

        //write the data to the string signal queue
        std::string string_value_to_write = _prop_to_send_as_string;
        *_data_writer_string << string_value_to_write;

        //write the data to the DDL signal
        fep3::examples::tEasyStruct my_easy_struct{};
        my_easy_struct.double_value = _prop_to_send_as_double;
        my_easy_struct.pos = { static_cast<uint32_t>(_prop_to_send_as_integer) ,
                               static_cast<uint32_t>(_prop_to_send_as_integer) ,
                               static_cast<uint32_t>(_prop_to_send_as_integer) };
        *_data_writer_ddl << my_easy_struct;

        //prepare and write the data to the DDL Arraysignal
        std::vector<fep3::examples::tEasyStruct> easy_array;
        int fill_idx = 32;
        while (fill_idx > 0)
        {
            easy_array.emplace_back(my_easy_struct);
            --fill_idx;
        }
        //currently we need that helper class to write it
        fep3::base::StdVectorSampleType<fep3::examples::tEasyStruct> easy_array_wrapper(easy_array);
        easy_array_wrapper.setTime(sim_time_of_execution);

        _data_writer_ddl_array->write(easy_array_wrapper);

        //write the plain value
        int32_t value_to_write = _prop_to_send_as_integer;
        *_data_writer_plain_c_type << value_to_write;

        FEP3_LOG_INFO("Data sent");

        return {};
    }


    //use the PropertyVariable as easy readable configuration element
    //but do NOT forget to register the variables in the CTOR with a name
    cpp::PropertyVariable<int32_t> _prop_to_send_as_integer{ 1 };
    cpp::PropertyVariable<std::string> _prop_to_send_as_string{ "Hello FEP3 World from CPP!" };
    cpp::PropertyVariable<double> _prop_to_send_as_double{ 0.1 };
    //it is possible to use vector properties
    cpp::PropertyVariable<std::vector<std::string>> _prop_as_string_array{ {"value1", "value2", "value3"} };

    //in core API you need to deal with everything by yourself
    //have a look at the fep3::cpp::DataJob in cpp API
    cpp::DataWriter* _data_writer_string;
    cpp::DataWriter* _data_writer_ddl;
    cpp::DataWriter* _data_writer_ddl_array;
    cpp::DataWriter* _data_writer_plain_c_type;
};

int main(int argc, const char* argv[])
{
    try
    {
        auto part = cpp::createParticipant<cpp::DataJobElement<EasyCPPSenderDataJob>>(
            argc, argv,
            "demo_cpp_sender",
            "demo_system");
        return part.exec();
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what();
        return 3;
    }
}

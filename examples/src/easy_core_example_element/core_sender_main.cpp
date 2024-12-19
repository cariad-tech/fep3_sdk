/**
 * Copyright 2023 CARIAD SE.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "example_ddl_types.h"

#include <fep3/core.h>

#include <ddl/dd/ddstructure.h>

#include <chrono>
#include <iostream>
#include <string>

using namespace fep3;
using namespace std::chrono_literals;

class EasySenderJob : public fep3::core::DefaultJob {
public:
    EasySenderJob() : fep3::core::DefaultJob("sender_job")
    {
    }

    void createDataIOs(const fep3::arya::IComponents&,
                       fep3::core::IDataIOContainer& io_container,
                       const fep3::catelyn::JobConfiguration&) override
    {
        // create DataAccess with the writer class
        // this will create a data writer for strings
        _data_writer_string = io_container.addDataOut("string_data", base::StreamTypeString(), 1);

        // create structure data definitions for the structs used within the example
        const auto dd_struct_easy_pos =
            ddl::DDStructureGenerator<fep3::examples::tEasyPosition>("tEasyPosition")
                .addElement("x_pos", &fep3::examples::tEasyPosition::x_pos)
                .addElement("y_pos", &fep3::examples::tEasyPosition::y_pos)
                .addElement("z_pos", &fep3::examples::tEasyPosition::z_pos);
        const auto dd_struct_easy_struct =
            ddl::DDStructureGenerator<fep3::examples::tEasyStruct>("tEasyStruct")
                .addElement("tEasyPosition", &fep3::examples::tEasyStruct::pos, dd_struct_easy_pos)
                .addElement("double_value", &fep3::examples::tEasyStruct::double_value);
        // the structure data definition can be used to retrieve the corresponding ddl description
        const auto description = dd_struct_easy_struct.getStructDescription();

        // this will create a data writer for ddl based structures
        _data_writer_ddl = io_container.addDataOut(
            "ddl_data", base::StreamTypeDDL(fep3::examples::examples_ddl_struct, description), 1);

        // this will create a data writer for dynamic arrays of DDL based structures
        _data_writer_ddl_array = io_container.addDataOut(
            "ddl_array",
            base::StreamTypeDDLArray(fep3::examples::examples_ddl_struct, description, 32),
            1);

        // this will create a data writer for fixed size uint32_t values
        _data_writer_plain_c_type =
            io_container.addDataOut("plain_c_type_int32_t", base::StreamTypePlain<int32_t>(), 1);
    }

    fep3::Result registerPropertyVariables() override
    {
        FEP3_RETURN_IF_FAILED(registerPropertyVariable(_prop_to_send_as_integer, "integer_value"));
        FEP3_RETURN_IF_FAILED(registerPropertyVariable(_prop_to_send_as_string, "string_value"));
        FEP3_RETURN_IF_FAILED(registerPropertyVariable(_prop_to_send_as_double, "double_value"));
        FEP3_RETURN_IF_FAILED(
            registerPropertyVariable(_prop_as_string_array, "string_array_value"));
        return {};
    }

    fep3::Result unregisterPropertyVariables() override
    {
        FEP3_RETURN_IF_FAILED(
            unregisterPropertyVariable(_prop_to_send_as_integer, "integer_value"));
        FEP3_RETURN_IF_FAILED(unregisterPropertyVariable(_prop_to_send_as_string, "string_value"));
        FEP3_RETURN_IF_FAILED(unregisterPropertyVariable(_prop_to_send_as_double, "double_value"));
        FEP3_RETURN_IF_FAILED(
            unregisterPropertyVariable(_prop_as_string_array, "string_array_value"));
        return {};
    }

    fep3::Result execute(fep3::Timestamp sim_time_of_execution)
    {
        // we want to update the values of our propertyvariables
        // so we can use them for sending the data
        updatePropertyVariables();

        // write the data to the string signal queue
        std::string string_value_to_write = _prop_to_send_as_string;
        *_data_writer_string << string_value_to_write;

        // write the data to the DDL signal
        fep3::examples::tEasyStruct my_easy_struct{};
        my_easy_struct.double_value = _prop_to_send_as_double;
        my_easy_struct.pos = {static_cast<uint32_t>(_prop_to_send_as_integer),
                              static_cast<uint32_t>(_prop_to_send_as_integer),
                              static_cast<uint32_t>(_prop_to_send_as_integer)};
        *_data_writer_ddl << my_easy_struct;

        // prepare and write the data to the DDL Arraysignal
        std::vector<fep3::examples::tEasyStruct> easy_array;
        int fill_idx = 32;
        while (fill_idx > 0) {
            easy_array.emplace_back(my_easy_struct);
            --fill_idx;
        }
        // currently we need that helper class to write it
        fep3::base::StdVectorSampleType<fep3::examples::tEasyStruct> easy_array_wrapper(easy_array);
        easy_array_wrapper.setTime(sim_time_of_execution);

        _data_writer_ddl_array->write(easy_array_wrapper);

        // write the plain value
        int32_t value_to_write = _prop_to_send_as_integer;
        *_data_writer_plain_c_type << value_to_write;

        FEP3_LOG_INFO("Data sent");

        return {};
    }

    fep3::base::PropertyVariable<int32_t> _prop_to_send_as_integer{1};
    fep3::base::PropertyVariable<std::string> _prop_to_send_as_string{"Hello FEP3 World!"};
    fep3::base::PropertyVariable<double> _prop_to_send_as_double{0.1};
    // it is possible to use vector properties
    fep3::base::PropertyVariable<std::vector<std::string>> _prop_as_string_array{
        {"value1", "value2", "value3"}};

    core::DataWriter* _data_writer_string = nullptr;
    core::DataWriter* _data_writer_ddl = nullptr;
    core::DataWriter* _data_writer_ddl_array = nullptr;
    core::DataWriter* _data_writer_plain_c_type = nullptr;
};

class EasyCoreSenderElement : public fep3::core::CustomJobElement {
public:
    EasyCoreSenderElement() : CustomJobElement("element")
    {
    }

    std::string getTypename() const override
    {
        return "Demo Element Base Sender Type";
    }
    std::string getVersion() const override
    {
        return FEP3_PARTICIPANT_LIBRARY_VERSION_STR;
    }

    std::tuple<fep3::Result, JobPtr, JobConfigPtr> createJob(const fep3::arya::IComponents&) override
    {
        auto config = std::make_unique<fep3::ClockTriggeredJobConfiguration>(1s);

        auto job = std::make_shared<EasySenderJob>();

        return {fep3::Result{}, job, std::move(config)};
    }

    fep3::Result destroyJob() override
    {
        // nothing to do
        return {};
    }
};

int main(int argc, const char* argv[])
{
    try {
        auto part =
            fep3::core::createParticipant<fep3::core::CustomElementFactory<EasyCoreSenderElement>>(
                argc,
                argv,
                "My Demo Participant Version 1.0",
                {"demo_core_sender", "demo_system", ""});
        return part.exec();
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what();
        return 3;
    }
}

/**
 * Copyright 2023 CARIAD SE.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <iostream>
#include <memory>
#include <string>
#include <chrono>

#include "example_ddl_types.h"

#include <fep3/core.h>
#include <ddl/dd/ddstructure.h>

using namespace fep3;
using namespace std::chrono_literals;

class EasyReceiverJob : public fep3::core::DefaultJob
{
public:
    EasyReceiverJob() : fep3::core::DefaultJob("receiver_job")
    {
    }

    void createDataIOs(const fep3::arya::IComponents&,
        fep3::core::IDataIOContainer& io_container,
        const fep3::catelyn::JobConfiguration& job_config) override
    {
        //create DataAccess with the reader class
        //this will create a data reader for strings
        _data_reader_string = io_container.addDataIn("string_data",
            base::StreamTypeString(), 2, job_config);

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
        _data_reader_ddl = io_container.addDataIn("ddl_data",
            base::StreamTypeDDL(fep3::examples::examples_ddl_struct, description), 2, job_config);

        //this will create a data reader for dynamic arrays of DDL based structures
        _data_reader_ddl_array = io_container.addDataIn("ddl_array",
            base::StreamTypeDDLArray(fep3::examples::examples_ddl_struct, description, 32), 2, job_config);

        //this will create a data reader for fixed size uint32_t values
        _data_reader_plain_c_type = io_container.addDataIn("plain_c_type_int32_t",
            base::StreamTypePlain<int32_t>(), 2, job_config);
    }

    fep3::Result execute(fep3::Timestamp sim_time_of_execution)
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

        //receive string value
        Optional<std::string> received_string_value;
        *_data_reader_string >> received_string_value;
        if (received_string_value.has_value())
        {
            FEP3_LOG_INFO("received string value:" + received_string_value.value());
        }

        //receive ddl structure value
        Optional<fep3::examples::tEasyStruct> easy_struct;
        *_data_reader_ddl >> easy_struct;
        if (easy_struct.has_value())
        {
            FEP3_LOG_INFO("received easy struct value: " + easyToString(easy_struct.value()));
        }

        //receive ddl array structure value
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
                + easyToString(easy_struct_array[0]));
        }
        else
        {
            FEP3_LOG_INFO("not yet received ddl array");
        }

        return {};
    }

    std::string easyToString(const fep3::examples::tEasyStruct& easy_struct) const
    {
        std::string res = "{ pos = { " + std::to_string(easy_struct.pos.x_pos) + ", ";
        res += std::to_string(easy_struct.pos.y_pos) + ", ";
        res += std::to_string(easy_struct.pos.z_pos) + " }";
        res += ", double_value = " + std::to_string(easy_struct.double_value) + " }";
        return res;
    }

    core::DataReader* _data_reader_string = nullptr;
    core::DataReader* _data_reader_ddl = nullptr;
    core::DataReader* _data_reader_ddl_array = nullptr;
    core::DataReader* _data_reader_plain_c_type = nullptr;
};

class EasyCoreReceiverElement : public fep3::core::CustomJobElement
{
public:
    EasyCoreReceiverElement() : CustomJobElement("element")
    {
    }

    std::string getTypename() const override
    {
        return "Demo Element Base Receiver Type";
    }

    std::string getVersion() const override
    {
        return FEP3_PARTICIPANT_LIBRARY_VERSION_STR;
    }

    std::tuple<fep3::Result, JobPtr, JobConfigPtr> createJob(const fep3::arya::IComponents&) override
    {
        auto config = std::make_unique<fep3::ClockTriggeredJobConfiguration>(1s);

        auto job = std::make_shared<EasyReceiverJob>();

        return { fep3::Result{}, job, std::move(config) };
    }

    fep3::Result destroyJob() override
    {
        // nothing to do
        return {};
    }

};


int main(int argc, const char* argv[])
{
    try
    {
        auto part = core::createParticipant<fep3::core::CustomElementFactory<EasyCoreReceiverElement>>(
            argc, argv,
            "My Demo Participant Version 1.0",
            { "demo_core_receiver", "demo_system", "" });
        return part.exec();
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what();
        return 3;
    }
}

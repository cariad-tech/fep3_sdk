/**
 * @file
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

    This Source Code Form is subject to the terms of the Mozilla
    Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

If it is not possible or desirable to put the notice in a particular file, then
You may include the notice in a location (such as a LICENSE file in a
relevant directory) where a recipient would be likely to look for such a notice.

You may add additional accurate notices of copyright ownership.

@endverbatim
 */
#include <iostream>
#include <string>



#include <fep3/core.h>
#include <fep3/core/element_configurable.h>
#include <chrono>

using namespace fep3;
using namespace std::chrono_literals;

class EasySenderJob : public IJob
{
public:
    using ExecuteCall = std::function<fep3::Result(fep3::Timestamp)>;

    explicit EasySenderJob(
        const ExecuteCall& ex_in,
        const ExecuteCall& ex,
        const ExecuteCall& ex_out) :
        _execute_func_data_in(ex_in),
        _execute_func(ex),
        _execute_func_data_out(ex_out)
    {
    }

    fep3::Result executeDataIn(Timestamp time_of_execution)
    {
        return _execute_func_data_in(time_of_execution);
    }
    fep3::Result execute(Timestamp time_of_execution)
    {
        return _execute_func(time_of_execution);
    }
    fep3::Result executeDataOut(Timestamp time_of_execution)
    {
        return _execute_func_data_out(time_of_execution);
    }
    std::function<fep3::Result(fep3::Timestamp)> _execute_func_data_in;
    std::function<fep3::Result(fep3::Timestamp)> _execute_func;
    std::function<fep3::Result(fep3::Timestamp)> _execute_func_data_out;
};

class EasyCoreSenderElement : public core::ElementConfigurable
{
public:
    EasyCoreSenderElement()
        : core::ElementConfigurable("Demo Element Base Sender Type",
                                    FEP3_PARTICIPANT_LIBRARY_VERSION_STR)
    {
        //register the properties
        registerPropertyVariable(_prop_to_send_as_integer, "integer_value");

        _data_writer_plain_c_type = std::make_shared<core::DataWriter>("plain_c_type_int32_t",
            base::StreamTypePlain<int32_t>());

        //this Job will connect the process methods to the scheduler
        //you may also use another option, consider cpp::DataJob i.e.
        _my_job = std::make_shared<EasySenderJob>(
            [this](fep3::Timestamp sim_time)-> fep3::Result { return {}; },
            [this](fep3::Timestamp sim_time)-> fep3::Result { return process(sim_time); },
            [this](fep3::Timestamp sim_time)-> fep3::Result { return processDataOut(sim_time); });
    }

    fep3::Result load() override
    {
        //register the job
        return core::addToComponents("sender_job", _my_job, { 1s }, *getComponents());
    }
    
    fep3::Result initialize() override
    {
        //register the data
        auto data_adding_res = core::addToComponents(*_data_writer_plain_c_type, *getComponents());
        if (isFailed(data_adding_res)) 
        {
            return data_adding_res;
        }
        else
        {
             return {};
        }
    }

    void deinitialize() override
    {
        //very important in the core API ... you need to synchronously register and unregister your data
        core::removeFromComponents(*_data_writer_plain_c_type, *getComponents());
    }
    
    void unload() override
    {
        //very important in the core API ... you need to synchronously register and unregister your jobs
        core::removeFromComponents("sender_job", *getComponents());
    }

    fep3::Result process(fep3::Timestamp sim_time_of_execution)
    {
        //we want to update the values of our propertyvariables
        //so we can use them for sending the data
        updatePropertyVariables();

        //write the plain value
        int32_t value_to_write = _prop_to_send_as_integer;
        *_data_writer_plain_c_type << value_to_write;

        FEP3_LOG_INFO("Data sent");

        return {};
    }

    fep3::Result processDataOut(fep3::Timestamp sim_time_of_execution)
    {
        //this is to flush and write it to the bus
        _data_writer_plain_c_type->flushNow(sim_time_of_execution);
        return {};
    }

    //use the PropertyVariable as easy readable configuration element
    //but do NOT forget to register the variables in the CTOR with a name
    core::PropertyVariable<int32_t> _prop_to_send_as_integer{ 1 };

    //in core API you need to deal with everything by yourself
    //have a look at the fep3::cpp::DataJob in cpp API
    std::shared_ptr<EasySenderJob> _my_job;
    std::shared_ptr<core::DataWriter> _data_writer_plain_c_type;
};


int main(int argc, const char* argv[])
{
    try
    {
        auto part = core::createParticipant<core::ElementFactory<EasyCoreSenderElement>>(
            argc, argv,
            "My Demo Participant Version 1.0",
            { "demo_core_sender", "demo_system", "" });
        return part.exec();
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what();
        return 3;
    }
}

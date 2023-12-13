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

#include <fep3/core.h>
#include <fep3/core/element_configurable.h>
#include <chrono>

using namespace fep3;
using namespace std::chrono_literals;

class EasyReceiverJob : public IJob
{
public:
    using ExecuteCall = std::function<fep3::Result(fep3::Timestamp)>;

    explicit EasyReceiverJob(
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

class EasyCoreReceiverElement : public fep3::core::ElementConfigurable
{
public:
    //Implementation of the CTOR!
    //ElementConfigurable has no default CTOR
    // you must define a type of your element -> to identify your implementation in a system
    // you must define a implementation version -> to identify your implementation version in a system
    // KEEP in MIND THIS IS NOT THE ELEMENT INSTANCE NAME!
    EasyCoreReceiverElement()
        : fep3::core::ElementConfigurable("Demo Element Base Receiver Type",
            FEP3_PARTICIPANT_LIBRARY_VERSION_STR)
    {
        //create DataAccess with the reader class
        //this will create a data reader for fixed size uint32_t values
        _data_reader_plain_c_type = std::make_shared<core::DataReader>("plain_c_type_int32_t",
            base::StreamTypePlain<int32_t>());

        //this Job will connect the process methods to the scheduler
        //you may also use another option, consider cpp::DataJob i.e.
        _my_job = std::make_shared<EasyReceiverJob>(
            [this](fep3::Timestamp sim_time)-> fep3::Result { return processDataIn(sim_time); },
            [this](fep3::Timestamp sim_time)-> fep3::Result { return process(sim_time); },
            [this](fep3::Timestamp sim_time)-> fep3::Result { return {}; });
    }
    
    fep3::Result load() override
    {
        //register the job
        return core::addToComponents("receiver_job", _my_job, { 1s }, *getComponents());
    }
    //End(load)

    fep3::Result initialize() override
    {
        //register the data
        auto data_adding_res = core::addToComponents(*_data_reader_plain_c_type, *getComponents());
        if (!data_adding_res) return data_adding_res;
        return {};
    }
    //End(initialize)

    void deinitialize() override
    {
        //very important in the core API ... you need to synchronously register and unregister your data
        core::removeFromComponents(*_data_reader_plain_c_type, *getComponents());
    }
    
    void unload() override
    {
        //very important in the core API ... you need to synchronously register and unregister your jobs
        core::removeFromComponents("receiver_job", *getComponents());
    }

    fep3::Result processDataIn(fep3::Timestamp sim_time_of_execution)
    {
        //this is to receive samples and store them temporarily in the corresponding data readers
        _data_reader_plain_c_type->receiveNow(sim_time_of_execution);
        return {};
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
        else
        {
            FEP3_LOG_INFO("no value received");
        }

        return {};
    }

    //in core API you need to deal with everything by yourself
    //have a look at the fep3::cpp::DataJob in cpp API
    std::shared_ptr<EasyReceiverJob> _my_job;
    std::shared_ptr<core::DataReader> _data_reader_plain_c_type;
};

int main(int argc, const char* argv[])
{
    try
    {
        auto part = core::createParticipant<core::ElementFactory<EasyCoreReceiverElement>>(
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

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

#include <fep3/core.h>
#include <fep3/core/element_configurable.h>
#include <a_util/system/address_info.h>
#include <fep3/fep3_filesystem.h>
#include <chrono>

#include "example_ddl_types.h"

using namespace fep3;
using namespace std::chrono_literals;

class EasyReceiverJob : public IJob
{
public:
    using ExecuteCall = std::function<Result(Timestamp)>;

    explicit EasyReceiverJob(
        const ExecuteCall& ex_in,
        const ExecuteCall& ex,
        const ExecuteCall& ex_out) :
        _execute_func_data_in(ex_in),
        _execute_func(ex),
        _execute_func_data_out(ex_out)
    {
    }

    Result executeDataIn(Timestamp time_of_execution)
    {
        return _execute_func_data_in(time_of_execution);
    }
    Result execute(Timestamp time_of_execution)
    {
        return _execute_func(time_of_execution);
    }
    Result executeDataOut(Timestamp time_of_execution)
    {
        return _execute_func_data_out(time_of_execution);
    }
    std::function<Result(Timestamp)> _execute_func_data_in;
    std::function<Result(Timestamp)> _execute_func;
    std::function<Result(Timestamp)> _execute_func_data_out;
};

class SignalMappingReceiverElement : public core::ElementConfigurable
{
public:
    SignalMappingReceiverElement()
        : core::ElementConfigurable("Signal Mapping Receiver Type",
            FEP3_PARTICIPANT_LIBRARY_VERSION_STR)
    {
        _my_job = std::make_shared<EasyReceiverJob>(
            [this](Timestamp sim_time)-> Result { return processDataIn(sim_time); },
            [this](Timestamp sim_time)-> Result { return process(sim_time); },
            [this](Timestamp sim_time)-> Result { return {}; });
    }
    
    Result load() override
    {
        return core::addToComponents("receiver_job", _my_job, { 1s }, *getComponents());
    }

    Result initialize() override
    {
        /// Static variable to query information about the memory address of this binary
        static const auto address_info_var = 0;
        const auto address_info = a_util::system::AddressInfo(address_info_var);

        fs::path demo_data_path(address_info.getFilePath().getParent().toString());
        demo_data_path.append("demo_data");

        /// Create the data reader for signal 'c' which shall be
        /// mapped from source signals 'source_signal_a' and 'source_signal_b'.
        _data_reader_c = std::make_shared<core::DataReader>(
            "c", base::StreamTypeDDLFileRef("tTestStructC", demo_data_path.string() + "/c.description"));

        /// Register the data reader at the data registry.
        return core::addToComponents(*_data_reader_c, *getComponents());
    }

    void deinitialize() override
    {
        core::removeFromComponents(*_data_reader_c, *getComponents());
    }
    
    void unload() override
    {
        core::removeFromComponents("receiver_job", *getComponents());
    }

    Result processDataIn(Timestamp sim_time_of_execution)
    {
        _data_reader_c->receiveNow(sim_time_of_execution);

        return {};
    }

    Result process(Timestamp sim_time_of_execution)
    {
        /// Log every time a new sample for 'c' is available.
        Optional<fep3::examples::TestStructC> c;
        *_data_reader_c >> c;
        if (c.has_value())
        {
            FEP3_LOG_INFO("Received 'test struct c': " + toString(c.value()));
        }

        return {};
    }

    std::string toString(const fep3::examples::TestStructC& c) const
    {
        std::string res = "{ first = " + std::to_string(c.first) + ", ";
        res += "second = " + std::to_string(c.second) + ", ";
        res += "third = " + std::to_string(c.third) + ", ";
        res += "fourth = " + std::to_string(c.fourth) + ", ";
        res += "fifth = " + std::to_string(c.fifth) + " }";
        return res;
    }

    std::shared_ptr<EasyReceiverJob> _my_job;
    std::shared_ptr<core::DataReader> _data_reader_c;
};


int main(int argc, const char* argv[])
{
    try
    {
        auto part = core::createParticipant<core::ElementFactory<SignalMappingReceiverElement>>(
            argc, argv,
            "Demo Signal Mapping Receiver Version 1.0",
            { "signal_mapping_receiver", "demo_system", "" });
        return part.exec();
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what();
        return 3;
    }
}

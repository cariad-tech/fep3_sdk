/**
 * @file
 * @copyright
 * @verbatim
Copyright @ 2022 VW Group. All rights reserved.

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
#include <a_util/system/address_info.h>
#include <a_util/filesystem/filesystem.h>
#include <chrono>

#include "example_ddl_types.h"

using namespace fep3;
using namespace std::chrono_literals;

class EasySenderJob : public IJob
{
public:
    using ExecuteCall = std::function<Result(Timestamp)>;

    explicit EasySenderJob(
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

class SignalMappingSenderElement : public core::ElementConfigurable {
public:
    SignalMappingSenderElement()
        : core::ElementConfigurable("Signal Mapping Sender Type",
                                          FEP3_PARTICIPANT_LIBRARY_VERSION_STR)
    {
        _my_job = std::make_shared<EasySenderJob>(
            [this](Timestamp sim_time) -> Result { return {}; },
            [this](Timestamp sim_time) -> Result { return process(sim_time); },
            [this](Timestamp sim_time) -> Result { return processDataOut(sim_time); });
    }

    Result load() override
    {
        return core::addToComponents("sender_job", _my_job, {1s}, *getComponents());
    }

    Result initialize() override
    {
        /// Static variable to query information about the memory address of this binary
        static const auto address_info_var = 0;
        const auto address_info = a_util::system::AddressInfo(address_info_var);
        a_util::filesystem::Path demo_data_path = address_info.getFilePath().getParent();
        demo_data_path.append("demo_data");

        /// Create data writers to transmit signals 'a_odd',
        /// 'a_even' and 'b'.
        /// DDL descriptions are provided via description files and the corresponding
        /// stream type is used to provide the ddl information.       
        _data_writer_a_odd = std::make_shared<core::DataWriter>(
            "a_odd", base::StreamTypeDDLFileRef("tTestStructA", demo_data_path.toString() + "/a.description"));
        _data_writer_a_even = std::make_shared<core::DataWriter>(
            "a_even", base::StreamTypeDDLFileRef("tTestStructA", demo_data_path.toString() + "/a.description"));
        _data_writer_b = std::make_shared<core::DataWriter>(
            "b", base::StreamTypeDDLFileRef("tTestStructB", demo_data_path.toString() + "/b.description"));
        
        /// Register the data writers at the data registry component.
        FEP3_RETURN_IF_FAILED(core::addToComponents(*_data_writer_a_odd, *getComponents()));
        FEP3_RETURN_IF_FAILED(core::addToComponents(*_data_writer_a_even, *getComponents()));
        return core::addToComponents(*_data_writer_b, *getComponents());
    }

    void deinitialize() override
    {
        core::removeFromComponents(*_data_writer_a_odd, *getComponents());
        core::removeFromComponents(*_data_writer_a_even, *getComponents());
        core::removeFromComponents(*_data_writer_b, *getComponents());
    }

    void unload() override
    {
        core::removeFromComponents("sender_job", *getComponents());
    }

    Result process(Timestamp sim_time_of_execution)
    {
        /// Transmit 'a_odd' for every second job trigger.
        /// 'target_signal_c' will only be triggered every time a sample for
        /// 'a' is received. Either 'a_odd' or
        /// 'a_even' has to be renamed accordingly to trigger
        /// creation of 'target_signal_c' on the receiver side.
        if (_global_send_count % 2 == 1)
        {
            fep3::examples::TestStructA a{
                static_cast<uint16_t>(_global_send_count),
                static_cast<int8_t>(_global_send_count + 2) };

            *_data_writer_a_odd << a;
            FEP3_LOG_INFO("'a_odd' sent:" + toString(a));
        }
        /// Transmit 'b' and 'a_even'.
        /// The latest sample for 'b' will be used for
        /// creation of 'target_signal_c' once a sample for 'a' is
        /// received.
        else
        {
            fep3::examples::TestStructB b{
                static_cast<uint64_t>(_global_send_count + 4),
                static_cast<uint32_t>(_global_send_count + 6) };

            *_data_writer_b << b;
            FEP3_LOG_INFO("'b' sent:" + toString(b));

            fep3::examples::TestStructA a{
                static_cast<uint16_t>(_global_send_count),
                static_cast<int8_t>(_global_send_count + 2) };

            *_data_writer_a_even << a;
            FEP3_LOG_INFO("'a_even' sent:" + toString(a));
        }

        _global_send_count++;
        FEP3_LOG_INFO("'global_send_count': " + std::to_string(_global_send_count));

        return {};
    }

    Result processDataOut(Timestamp sim_time_of_execution)
    {
        _data_writer_a_odd->flushNow(sim_time_of_execution);
        _data_writer_a_even->flushNow(sim_time_of_execution);
        _data_writer_b->flushNow(sim_time_of_execution);

        return {};
    }

    std::string toString(const fep3::examples::TestStructA& a) const
    {
        std::string res = "{ first = " + std::to_string(a.first) + ", ";
        res += "second = " + std::to_string(a.second) + " }";
        return res;
    }

    std::string toString(const fep3::examples::TestStructB& b) const
    {
        std::string res = "{ first = " + std::to_string(b.first) + ", ";
        res += "second = " + std::to_string(b.second) + " }";
        return res;
    }

private:
    std::shared_ptr<EasySenderJob> _my_job;
    std::shared_ptr<core::DataWriter> _data_writer_a_odd;
    std::shared_ptr<core::DataWriter> _data_writer_a_even;
    std::shared_ptr<core::DataWriter> _data_writer_b;
    uint64_t _global_send_count{ 0 };
};

int main(int argc, const char* argv[])
{
    try
    {
        auto part = core::createParticipant<core::ElementFactory<SignalMappingSenderElement>>(
            argc, argv,
            "Demo Signal Mapping Sender Version 1.0",
            { "signal_mapping_sender", "demo_system", "" });
        return part.exec();
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what();
        return 3;
    }
}

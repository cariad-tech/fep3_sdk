/**
 * @copyright
 * @verbatim
 * Copyright 2023 CARIAD SE.
 * 
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 * 
 * @endverbatim
 */
#include <fep3/cpp.h>
#include <chrono>
#include <thread>
#include <iostream>

using namespace fep3;
using namespace std::chrono_literals;

class AsyncReceiver : public fep3::ISimulationBus::IDataReceiver 
{
public:
    AsyncReceiver(Duration workload) : _workload(workload)
    {
    }

    void operator()(const fep3::data_read_ptr<const fep3::IStreamType>& type)
    {
    }

    void operator()(const fep3::data_read_ptr<const fep3::IDataSample>& sample)
    {
        int buffer = 0;
        auto raw_buffer = fep3::base::RawMemoryStandardType<int>(buffer);
        sample->read(raw_buffer);
        std::cout << "Data sample read: " << buffer << ", sleep for " << _workload.count() << std::endl;
        std::this_thread::sleep_for(_workload);
    }

private:
    Duration _workload;
};



class ReceiverElement : public core::ElementBase
{
public:

ReceiverElement() : ElementBase("AsyncWaitSetReceiver", "1.0.0")
{

};

fep3::Result load() override
{
    auto* conf_service = getComponents()->getComponent<fep3::arya::IConfigurationService>();

    if (conf_service)
    {
        FEP3_RETURN_IF_FAILED(base::setPropertyValue<bool>(*conf_service, FEP3_RTI_DDS_SIMBUS_ASYNC_WAITSET, true));
        FEP3_RETURN_IF_FAILED(base::setPropertyValue<int32_t>(*conf_service, FEP3_RTI_DDS_SIMBUS_ASYNC_WAITSET_THREADS, 12));
    }
    return {};
}

fep3::Result run() override
{
    auto* sim_bus = getComponents()->getComponent<fep3::ISimulationBus>();
    if (!sim_bus)
    {
        RETURN_ERROR_DESCRIPTION(fep3::ERR_NOT_FOUND, "Simulation Bus interface was not found");
    }

    sim_bus->getReader("topic1", base::StreamTypePlain<int>())->reset(_receiver1);
    sim_bus->getReader("topic2", base::StreamTypePlain<int>())->reset(_receiver2);

    return {};
}

private:
    std::shared_ptr<AsyncReceiver> _receiver1 = std::make_shared<AsyncReceiver>(5s);
    std::shared_ptr<AsyncReceiver> _receiver2 = std::make_shared<AsyncReceiver>(1s);
};

int main(int argc, const char* argv[])
{
    using namespace fep3::core;
    //creating a participant with the element that will send the logs
    auto demo_participant = createParticipant<fep3::core::ElementFactory<ReceiverElement>>(
        "async_waitset_receiver",
        "1.0.0",
        "async_waitset");
    //wait until exit is send
    return demo_participant.exec();
}


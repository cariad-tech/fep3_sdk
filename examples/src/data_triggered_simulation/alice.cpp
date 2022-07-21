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


#include <fep3/cpp.h>
#include <fep3/base/sample/data_sample.h>
#include "notification_data_receiver.hpp"

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

using namespace std::chrono_literals;

class AliceElement : public fep3::core::ElementBase {
public:
    AliceElement()
        : fep3::core::ElementBase("Demo Element Base Alice Type",
            FEP3_PARTICIPANT_LIBRARY_VERSION_STR)
    {
        _signal_a_sender_job = std::make_shared<fep3::cpp::DataJob>(
            "signal_a_sender_job",
            2s,
            [this](fep3::Timestamp sim_time)-> fep3::Result { return processOut(sim_time); }
        );

        _data_writer_signal_a = _signal_a_sender_job->addDataOut(
            "signal_a",
            fep3::base::StreamTypePlain<int32_t>()
        );
    }

    fep3::Result initialize() override {
        // Register the job
        FEP3_RETURN_IF_FAILED(fep3::core::addToComponents({ _signal_a_sender_job }, *getComponents()));

        auto data_registry = getComponents()->getComponent<fep3::IDataRegistry>();
        if (!data_registry) {
            RETURN_ERROR_DESCRIPTION(fep3::ERR_POINTER, "DataRegistry is unavailable");
        }

        // Register the writer
        FEP3_RETURN_IF_FAILED(_signal_a_sender_job->addDataToComponents(*getComponents()));

        // Register the input signal
        FEP3_RETURN_IF_FAILED(data_registry->registerDataIn("signal_b", fep3::base::StreamTypePlain<int32_t>()));

        // Register the data receiver
        _stop = false;
        _signal_b_worker_thread = std::thread([this] {this->processIn();});
        _data_receiver = std::make_shared<NotificationDataReceiver>(_condition_data_received, _mutex_data_received, _data_received, _data_sample);

        FEP3_RETURN_IF_FAILED(data_registry->registerDataReceiveListener("signal_b", _data_receiver));
        return {};
    }

    void deinitialize() override {
        // Stop data receiver thread
        {
            std::lock_guard<std::mutex> lock(_mutex_data_received);
            _stop = true;
        }
        _condition_data_received.notify_one();
        _signal_b_worker_thread.join();

        // Unregister data receiver
        auto data_registry = getComponents()->getComponent<fep3::IDataRegistry>();
        if (data_registry) {
            data_registry->unregisterDataReceiveListener("signal_b", _data_receiver);
        }
        _data_receiver.reset();

        // Unregister input signal
        if (data_registry) {
            data_registry->unregisterDataIn("signal_b");
        }

        // Unregister writer and job
        _signal_a_sender_job->removeDataFromComponents(*getComponents());
        fep3::core::removeFromComponents(_signal_a_sender_job->getJobInfo().getName(), *getComponents());
    }

    fep3::Result processOut(fep3::Timestamp sim_time) {
        *_data_writer_signal_a << ++_counter;
        std::cout << "ST: " << std::chrono::duration_cast<std::chrono::milliseconds>(sim_time).count() << "ms\t Sending:   " << _counter << std::endl;
        return{};
    }

    void processIn() {
        auto clock_service = getComponents()->getComponent<fep3::IClockService>();
        while (true)
        {
            std::unique_lock<std::mutex> lock(_mutex_data_received);
            _condition_data_received.wait(lock, [this] {return _data_received || _stop;});
            if (_stop) break;

            if (clock_service) {
                fep3::Timestamp current_time = clock_service->getTime();
                int32_t data;
                fep3::base::DataSampleType<int32_t> sample_wrapup(data);
                if (_data_sample) {
                    auto copied_bytes = _data_sample->read(sample_wrapup);
                    if (copied_bytes != sample_wrapup.size()) {
                        std::cout << "ST: " << std::chrono::duration_cast<std::chrono::milliseconds>(current_time).count() << "ms Reading received sample failed" << std::endl;
                    }
                    else {
                        std::cout << "ST: " << std::chrono::duration_cast<std::chrono::milliseconds>(current_time).count() << "ms\t Receiving: " << data << std::endl;
                    }
                }
                else {
                    std::cout << "ST: " << std::chrono::duration_cast<std::chrono::milliseconds>(current_time).count() << "ms Received data sample but data_read_ptr is null" << std::endl;
                }
            }

            _data_sample.reset();
            _data_received = false;
        }
    }

private:
    std::shared_ptr<fep3::cpp::DataJob> _signal_a_sender_job;
    fep3::core::DataWriter* _data_writer_signal_a;

    std::shared_ptr<NotificationDataReceiver> _data_receiver;
    fep3::data_read_ptr<const fep3::IDataSample> _data_sample{};
    std::thread _signal_b_worker_thread;
    int32_t _counter{ 0 };
    std::atomic<bool> _stop{ false };

    std::atomic<bool> _data_received{ false };
    std::mutex _mutex_data_received;
    std::condition_variable _condition_data_received;
};

int main(int argc, const char* argv[]) {
    try {
        auto part = fep3::core::createParticipant<fep3::core::ElementFactory<AliceElement>>(
            argc, argv,
            "My Demo Participant Version 1.0",
            { "alice", "demo_system", "" });
        return part.exec();
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what();
        return 3;
    }
}
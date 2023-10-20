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

#include <fep3/cpp.h>
#include <iostream>

using namespace fep3;
using namespace std::chrono_literals;

class OneCPPReceiverDataJob : public cpp::DataJob {
public:
    enum SampleState { sample_received, no_sample_received };
    OneCPPReceiverDataJob() : cpp::DataJob("receive_job", 1s)
    {
        _data_reader_plain_c_type =
            addDataIn("plain_c_type_int32_t", base::StreamTypePlain<int32_t>(), 10);
    }

    template <typename T>
    SampleState readSample(core::DataReader& reader, T& value)
    {
        auto ptr = reader.popSampleOldest();
        if (ptr) {
            fep3::base::DataSampleType<T> sample_wrapup(value);
            auto copied_bytes = ptr->read(sample_wrapup);
            if (copied_bytes != sample_wrapup.size()) {
                throw std::runtime_error(std::string() + "reading sample from reader " +
                                         reader.getName() + " failed");
            }
            return SampleState::sample_received;
        }
        return SampleState::no_sample_received;
    }

    fep3::Result process(Timestamp sim_time_of_execution)
    {
        int32_t received_plain_value = 0;
        while (readSample(*_data_reader_plain_c_type, received_plain_value) ==
               SampleState::sample_received) {
            global_receive_count++;
            FEP3_LOG_INFO("received plain value:" + std::to_string(received_plain_value));
        }

        FEP3_LOG_INFO("Received all samples. Receive count = " +
                      std::to_string(global_receive_count));

        return {};
    }

private:
    core::DataReader* _data_reader_plain_c_type;
    uint64_t global_receive_count = 0;
};

int main(int argc, const char* argv[])
{
    try {
        auto part = cpp::createParticipant<cpp::DataJobElement<OneCPPReceiverDataJob>>(
            argc, argv, "demo_one_receiver", "multi_sender_for_one_signal_system");
        return part.exec();
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what();
        return EXIT_FAILURE;
    }
}
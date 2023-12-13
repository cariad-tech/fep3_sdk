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

using namespace std::literals::chrono_literals;

class AliceJob : public fep3::cpp::DataJob
{
public:
    AliceJob() : DataJob("alice", 100ms)
    {
        my_out_data = addDataOut("my_data", fep3::base::StreamTypeString());
        registerPropertyVariable(_value_to_send, "value_to_send");
    }

    fep3::Result process(fep3::Timestamp time) override
    {
        std::string data_to_write = _value_to_send;
        data_to_write += std::to_string(time.count());
        FEP3_LOG_INFO("Sending: " + data_to_write);
        *my_out_data << data_to_write;
        return {};
    }

private:
    fep3::core::DataWriter* my_out_data;
    fep3::base::PropertyVariable<std::string> _value_to_send{ "value at time:" };
    uint32_t _send_signal_count;
    uint32_t _signals_sent{};
};

int main(int argc, const char* argv[]) {
    try {
        auto part = fep3::cpp::createParticipant<fep3::cpp::DataJobElement<AliceJob>>(
            argc, argv,
            "demo_sender", 
            "demo_system");
        return part.exec();
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what();
        return 3;
    }
}

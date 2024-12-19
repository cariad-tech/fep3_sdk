/**
 * Copyright 2023 CARIAD SE.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <fep3/cpp.h>
#include <iostream>
#include <string>

class BobJob : public fep3::cpp::DataJob
{
public:
    BobJob() : DataJob("bob", fep3::DataTriggeredJobConfiguration({"my_data"}))
    {
        my_out_data = addDataIn("my_data", fep3::base::StreamTypeString());
    }

    fep3::Result process(fep3::Timestamp time) override
    {
        fep3::Optional<std::string> data;
        *my_out_data >> data;
        if(data)
        {
            FEP3_LOG_INFO("Receiving: " + data.value());
        }
        return {};
    }

private:
    fep3::core::DataReader* my_out_data;
    uint32_t _send_signal_count;
    uint32_t _signals_sent{};
};

int main(int argc, const char* argv[]) {
    try {
        auto part = fep3::cpp::createParticipant<fep3::cpp::DataJobElement<BobJob>>(
            argc, argv,
            "demo_receiver", 
            "demo_system");
        return part.exec();
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what();
        return 3;
    }
}

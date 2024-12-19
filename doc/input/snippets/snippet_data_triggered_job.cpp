/**
 * Copyright 2023 CARIAD SE.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

//Begin(DataTriggeredJob)
#include <fep3/cpp.h>
#include <iostream>
#include <string>
using namespace fep3;

class DataTriggeredJob : public cpp::DataJob
{
public:
    DataTriggeredJob() : DataJob("bob", DataTriggeredJobConfiguration({"my_data"}))
    {
        _data_reader = addDataIn("my_data", base::StreamTypeString());
    }

    Result process(Timestamp time) override
    {
        Optional<std::string> data;
        *_data_reader >> data;
        if(data)
        {
            FEP3_LOG_INFO("Receiving: " + data.value());
        }
        return {};
    }

private:
    core::DataReader* _data_reader;
};
//End(DataTriggeredJob)

int main(int argc, const char* argv[]) {
    try {
        auto part = cpp::createParticipant<cpp::DataJobElement<DataTriggeredJob>>(
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


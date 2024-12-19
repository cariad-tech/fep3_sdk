/**
 * Copyright 2023 CARIAD SE.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <fep3/cpp.h>
#include <chrono>
using namespace fep3;
using namespace std::chrono_literals;

class MinimalDataJob : public cpp::DataJob {
public:
    MinimalDataJob() : cpp::DataJob("multiply", 1s) {
        _reader_small = addDataIn("small_value",  base::StreamTypePlain<int32_t>());
        _reader_mid = addDataIn("mid_value",  base::StreamTypePlain<int32_t>());
        _reader_large = addDataIn("large_value",  base::StreamTypePlain<int32_t>());
    }

    //Begin(DataJob_process)
    Result process(Timestamp sim_time_of_execution) {
        Optional<int32_t> received_plain_value_s = 0;
        Optional<int32_t> received_plain_value_m = 0;
        Optional<int32_t> received_plain_value_l = 0;
        *_reader_small >> received_plain_value_s;
        *_reader_mid >> received_plain_value_m;
        *_reader_large >> received_plain_value_l;
        
        return {};
    }
    //End(DataJob_process)

    cpp::DataReader* _reader_small;
    cpp::DataReader* _reader_mid;
    cpp::DataReader* _reader_large;
};

class MinimalDataJobElement : public cpp::DataJobElement<MinimalDataJob>
{
    fep3::Result load() override
    {
        auto res = cpp::DataJobElement<MinimalDataJob>::load();
        if (res.getErrorCode() != fep3::ERR_NOERROR) return res;
        fep3::arya::IConfigurationService* conf_service = getComponents()->getComponent<fep3::arya::IConfigurationService>();

        int64_t timeout_value = std::chrono::nanoseconds(std::chrono::seconds(10)).count();

        // Configuring the signal waiting
        FEP3_RETURN_IF_FAILED(base::setPropertyValue(*conf_service, FEP3_RTI_DDS_SIMBUS_DATAWRITER_READY_TIMEOUT, timeout_value));
        FEP3_RETURN_IF_FAILED(base::setPropertyValue(*conf_service, FEP3_RTI_DDS_SIMBUS_MUST_BE_READY_SIGNALS, "small_value;mid_value"));

        // Configuring the Domain ID
        FEP3_RETURN_IF_FAILED(base::setPropertyValue(*conf_service, FEP3_RTI_DDS_SIMBUS_PARTICIPANT_DOMAIN, 2));

        return {};
    }
};

int main(int argn, const char* argv[]) {
    auto part = cpp::createParticipant<MinimalDataJobElement>(
            argn, argv, "my_participant", "my_system");
    return part.exec();
}

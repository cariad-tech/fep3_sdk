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
#include <fep3/core.h>

#include <chrono>
#include <iostream>

using namespace fep3;
using namespace std::chrono_literals;

class MinimalJob : public fep3::core::DefaultJob {
public:
    MinimalJob(const std::string& name) : fep3::core::DefaultJob(name)
    {
    }

    void createDataIOs(const fep3::arya::IComponents&,
                       fep3::core::IDataIOContainer& io_container,
                       const fep3::catelyn::JobConfiguration& job_config) override
    {
        _reader =
            io_container.addDataIn("small_value", base::StreamTypePlain<int32_t>(), 0, job_config);
        _writer = io_container.addDataOut("big_value", base::StreamTypePlain<int32_t>(), 0);
    }

    fep3::Result execute(fep3::Timestamp) override
    {
        Optional<int32_t> received_plain_value;
        *_reader >> received_plain_value;
        if (received_plain_value.has_value()) {
            FEP3_LOG_INFO("Received: " + std::to_string(received_plain_value.value()));
            auto value = received_plain_value.value() * _factor;
            *_writer << value;
            FEP3_LOG_INFO("Sent: " + std::to_string(value));
        }
        return {};
    }

    fep3::Result registerPropertyVariables() override
    {
        FEP3_RETURN_IF_FAILED(registerPropertyVariable(_factor, "multiplication_factor"));
        return {};
    }

    fep3::Result unregisterPropertyVariables() override
    {
        FEP3_RETURN_IF_FAILED(unregisterPropertyVariable(_factor));
        return {};
    }

private:
    base::PropertyVariable<int32_t> _factor{2};
    core::DataReader* _reader;
    core::DataWriter* _writer;
};

class MinimalJobElement : public fep3::core::CustomJobElement {
public:
    MinimalJobElement() : CustomJobElement("minimal_job_element")
    {
    }

    std::string getTypename() const override
    {
        return "minimal_job_element";
    }
    std::string getVersion() const override
    {
        return "1.0.0";
    }

    std::tuple<fep3::Result, JobPtr, JobConfigPtr> createJob(const fep3::arya::IComponents&) override
    {
        std::vector<std::string> signal_names = {"small_value"};
        return {fep3::Result{},
                std::make_shared<MinimalJob>("minimal_job"),
                std::make_unique<DataTriggeredJobConfiguration>(signal_names)};
    }

    fep3::Result destroyJob() override
    {
        return {};
    }
};

int main(int argc, const char* argv[])
{
    try {
        auto part = fep3::base::createParticipant<core::CustomElementFactory<MinimalJobElement>>(
            "demo_minimal_job", "Demo Version 1.0", "demo_system");
        return part.exec();
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what();
        return 3;
    }
}


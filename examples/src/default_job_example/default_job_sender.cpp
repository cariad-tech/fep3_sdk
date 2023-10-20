/**
 * @file
 * @copyright
 * @verbatim
Copyright @ 2023 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#include <fep3/core.h>

#include <iostream>

using namespace fep3;

static std::string msg_to_send = "send from DefaultJob";

class JobSender : public fep3::core::DefaultJob {
public:
    JobSender(const std::string& name) : fep3::core::DefaultJob(name)
    {
    }

    void createDataIOs(const fep3::arya::IComponents&,
                       fep3::core::IDataIOContainer& io_container,
                       const fep3::catelyn::JobConfiguration&) override
    {
        _data_writer = io_container.addDataOut("signal_msg", fep3::base::StreamTypeString(), 1);
    }

    fep3::Result execute(fep3::Timestamp) override
    {
        *_data_writer << msg_to_send;
        return {};
    }

private:
    fep3::core::DataWriter* _data_writer = nullptr;
};

class JobSenderElement : public fep3::core::CustomJobElement {
public:
    JobSenderElement() : CustomJobElement("job_element")
    {
    }

    std::string getTypename() const override
    {
        return "job_element";
    }
    std::string getVersion() const override
    {
        return "1.0.0";
    }

    std::tuple<fep3::Result, JobPtr, JobConfigPtr> createJob(const fep3::arya::IComponents&) override
    {
        using namespace std::chrono_literals;
        return {fep3::Result{},
                std::make_shared<JobSender>("job_sender"),
                std::make_unique<fep3::ClockTriggeredJobConfiguration>(1s)};
    }

    fep3::Result destroyJob() override
    {
        return {};
    }
};

int main(int argc, const char* argv[])
{
    try {
        auto sender_elem_factory =
            std::make_shared<fep3::core::CustomElementFactory<JobSenderElement>>();

        auto part = fep3::base::createParticipant(
            "demo_job_sender", "Demo Version 1.0", "demo_system", sender_elem_factory);
        return part.exec();
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what();
        return 3;
    }
}


/**
 * Copyright 2023 CARIAD SE.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <fep3/core.h>

#include <chrono>
#include <iostream>
using namespace fep3;

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
        *_data_writer << "Write from default job";
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

class JobReceiver : public fep3::core::DefaultJob {
public:
    JobReceiver(const std::string& name) : fep3::core::DefaultJob(name)
    {
    }

    void createDataIOs(const fep3::arya::IComponents&,
                       fep3::core::IDataIOContainer& io_container,
                       const fep3::catelyn::JobConfiguration& job_config) override
    {
        _data_reader =
            io_container.addDataIn("signal_msg", fep3::base::StreamTypeString(), 1, job_config);
    }

    fep3::Result execute(fep3::Timestamp) override
    {
        fep3::Optional<std::string> value;
        *_data_reader >> value;
        if (value.has_value())
        {
            FEP3_LOG_INFO(value.value());
            FEP3_LOG_INFO("Property signal_name: " + _prop_string_signal_name.toString());
            FEP3_LOG_INFO("Property factor: " + _prop_int_factor.toString());
        }
        else
        {
            FEP3_LOG_INFO("No data available.");
        }

        return {};
    }

    fep3::Result registerPropertyVariables() override
    {
        FEP3_LOG_INFO(a_util::strings::format("job registerPropertyVariables"));
        FEP3_RETURN_IF_FAILED(registerPropertyVariable(_prop_string_signal_name, "signal_name"));
        FEP3_RETURN_IF_FAILED(registerPropertyVariable(_prop_int_factor, "factor"));
        return {};
    }

    fep3::Result unregisterPropertyVariables() override
    {
        FEP3_LOG_INFO(a_util::strings::format("job unregisterPropertyVariables"));
        FEP3_RETURN_IF_FAILED(unregisterPropertyVariable(_prop_string_signal_name));
        FEP3_RETURN_IF_FAILED(unregisterPropertyVariable(_prop_int_factor));
        return {};
    }

    // Begin(DefaultJob_optional_methods)
    fep3::Result initialize(const fep3::arya::IComponents&) override
    {
        // Do something in initialize phase.
        FEP3_LOG_INFO("job initialized");
        return {};
    };

    fep3::Result start() override
    {
        // Do something in start phase.
        FEP3_LOG_INFO("job started");
        return {};
    }

    fep3::Result stop() override
    {
        // Do something in stop phase.
        FEP3_LOG_INFO("job stopped");
        return {};
    }
    // End(DefaultJob_optional_methods)

private:
    fep3::base::PropertyVariable<std::string> _prop_string_signal_name{"default_name"};
    fep3::base::PropertyVariable<int> _prop_int_factor{2};
    fep3::core::DataReader* _data_reader = nullptr;
};

class JobReceiverElement : public fep3::core::CustomJobElement {
public:
    // Begin(CustomJobElement_customized_CTOR)
    JobReceiverElement(const std::string& job_name,
                       int int_value,
                       std::string string_value,
                       std::unique_ptr<int> unique_ptr_value)
        : _job_name(job_name),
          _int_value(int_value),
          _string_value(string_value),
          _unique_ptr_value(std::move(unique_ptr_value)),
          CustomJobElement("job_element")
    {
    }
    // End(CustomJobElement_customized_CTOR)

    std::string getTypename() const override
    {
        return "job_element";
    }
    std::string getVersion() const override
    {
        return "1.0.0";
    }

    // Begin(CustomJobElement_createJob_DataTriggered)
    std::tuple<fep3::Result, JobPtr, JobConfigPtr> createJob(const fep3::arya::IComponents&) override
    {
        auto config = std::make_unique<fep3::DataTriggeredJobConfiguration>(
            std::vector<std::string>{"signal_msg"});

        auto job = std::make_shared<JobReceiver>(_job_name);

        return {fep3::Result{}, job, std::move(config)};
    }
    // End(CustomJobElement_createJob_DataTriggered)

    fep3::Result destroyJob() override
    {
        return {};
    }

    // Begin(CustomJobElement_optional_methods)
    virtual fep3::Result load(const fep3::arya::IComponents&)
    {
        // Do something in load phase.
        FEP3_LOG_INFO("element initialized");
        return {};
    }

    virtual fep3::Result initialize(const fep3::arya::IComponents&)
    {
        // Do something in initialize phase.
        FEP3_LOG_INFO("element initialized");
        return {};
    }

    virtual fep3::Result run()
    {
        // Do something in start phase.
        FEP3_LOG_INFO("element started");
        return {};
    }

    virtual void stop()
    {
        // Do something in stop phase.
        FEP3_LOG_INFO("element stopped");
    }

    virtual void deinitialize(const fep3::arya::IComponents&)
    {
        // Do something in deinitialize phase.
        FEP3_LOG_INFO("element deinitialized");
    }

    virtual void unload(const fep3::arya::IComponents&)
    {
        // Do something in unload phase.
        FEP3_LOG_INFO("element unloaded");
    }
    // End(CustomJobElement_optional_methods)

private:
    std::string _job_name;
    int _int_value;
    std::string _string_value;
    std::unique_ptr<int> _unique_ptr_value;
};

int main(int argc, const char* argv[])
{
    try {
        // Begin(DefaultJob_CustomElementFactory)
        auto receiver_elem_factory =
            std::make_shared<fep3::core::CustomElementFactory<JobReceiverElement>>(
                "job_receiver", 1, "string literals", std::make_unique<int>(1));

        auto part = fep3::base::createParticipant(
            "demo_job_receiver", "Demo Version 1.0", "demo_system", receiver_elem_factory);
        return part.exec();
        // End(DefaultJob_CustomElementFactory)
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what();
        return 3;
    }
}


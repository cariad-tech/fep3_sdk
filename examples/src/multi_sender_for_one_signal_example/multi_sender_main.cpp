/**
 * Copyright 2023 CARIAD SE.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <fep3/core.h>
#include <fep3/core/element_configurable.h>
#include <iostream>

using namespace fep3;
using namespace std::chrono_literals;

class EasySenderJob : public IJob {
public:
    using ExecuteCall = std::function<fep3::Result(fep3::Timestamp)>;

    explicit EasySenderJob(const ExecuteCall& ex_in,
                           const ExecuteCall& ex,
                           const ExecuteCall& ex_out)
        : _execute_func_data_in(ex_in), _execute_func(ex), _execute_func_data_out(ex_out)
    {
    }

    fep3::Result executeDataIn(Timestamp time_of_execution)
    {
        return _execute_func_data_in(time_of_execution);
    }
    fep3::Result execute(Timestamp time_of_execution)
    {
        return _execute_func(time_of_execution);
    }
    fep3::Result executeDataOut(Timestamp time_of_execution)
    {
        return _execute_func_data_out(time_of_execution);
    }
    std::function<fep3::Result(fep3::Timestamp)> _execute_func_data_in;
    std::function<fep3::Result(fep3::Timestamp)> _execute_func;
    std::function<fep3::Result(fep3::Timestamp)> _execute_func_data_out;
};

class MultiCoreSenderElement : public fep3::core::ElementConfigurable {
public:
    MultiCoreSenderElement()
        : fep3::core::ElementConfigurable("Demo Element Base Sender Type",
                                          FEP3_PARTICIPANT_LIBRARY_VERSION_STR)
    {
        _data_writer_plain_c_type = std::make_shared<core::DataWriter>(
            "plain_c_type_int32_t", base::StreamTypePlain<int32_t>());

        _my_job = std::make_shared<EasySenderJob>(
            [this](fep3::Timestamp sim_time) -> fep3::Result { return {}; },
            [this](fep3::Timestamp sim_time) -> fep3::Result { return process(sim_time); },
            [this](fep3::Timestamp sim_time) -> fep3::Result { return processDataOut(sim_time); });
    }

    fep3::Result load() override
    {
        IConfigurationService* config_service =
            fep3::getComponent<fep3::IConfigurationService>(*getComponents());
        std::string value_to_send = "";
        value_to_send =
            fep3::base::getPropertyValue<std::string>(*config_service, "value_to_send/values")
                .value_or(value_to_send);
        if (value_to_send.empty()) {
            RETURN_ERROR_DESCRIPTION(fep3::ERR_FAILED,
                                     "Option \"value_to_send\" can not be empty.");
        }

        // Save values to vector
        std::stringstream ss(value_to_send);
        std::string item;
        while (std::getline(ss, item, ',')) {
            _value_to_send.push_back(std::stoi(item));
        }

        return core::addToComponents("sender_job", _my_job, {1s}, *getComponents());
    }

    fep3::Result initialize() override
    {
        auto data_adding_res = core::addToComponents(*_data_writer_plain_c_type, *getComponents());
        if (!data_adding_res)
            return data_adding_res;
        return {};
    }

    void deinitialize() override
    {
        core::removeFromComponents(*_data_writer_plain_c_type, *getComponents());
    }

    void unload() override
    {
        core::removeFromComponents("sender_job", *getComponents());
    }

    fep3::Result process(fep3::Timestamp sim_time_of_execution)
    {
        // Write the plain values
        for (auto value: _value_to_send) {
            base::DataSampleType<int32_t> sample{value};
            sample.setTime(sim_time_of_execution);
            _data_writer_plain_c_type->write(sample);
            _global_send_count++;
            FEP3_LOG_INFO("Data sent plain value:" + std::to_string(value));
        }

        FEP3_LOG_INFO("Global send count = " + std::to_string(_global_send_count));

        return {};
    }

    fep3::Result processDataOut(fep3::Timestamp sim_time_of_execution)
    {
        _data_writer_plain_c_type->flushNow(sim_time_of_execution);
        return {};
    }

private:
    std::shared_ptr<EasySenderJob> _my_job;
    std::shared_ptr<core::DataWriter> _data_writer_plain_c_type;
    uint64_t _global_send_count = 0;
    std::vector<int32_t> _value_to_send{};
};

int main(int argc, const char* argv[])
{
    std::string value_to_send = "";

    auto cli = (
        clipp::option
        ("-p", "--value-to-send")
        & clipp::value("string", value_to_send)
        .doc("Values to send"));

    auto clp = fep3::core::CommandLineParserFactory::create(
        cli,
        fep3::core::ParserDefaultValues{
            "demo_multi_sender", "multi_sender_for_one_signal_system", ""});

    try {
        auto part =
            fep3::core::createParticipant<fep3::core::ElementFactory<MultiCoreSenderElement>>(
                argc, argv, "My Demo multi sender version 1.0", std::move(clp));

        auto config_service = part.getComponent<fep3::IConfigurationService>();
        // Set main property
        auto property_node = std::make_shared<fep3::base::NativePropertyNode>("value_to_send");
        // Set subproperties
        auto property_value_to_send =
            fep3::base::makeNativePropertyNode<std::string>("values", value_to_send);

        property_node->setChild(property_value_to_send);

        // Register property node to participant
        config_service->registerNode(property_node);

        return part.exec();
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what();
        return EXIT_FAILURE;
    }
}

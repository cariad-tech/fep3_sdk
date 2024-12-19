/**
 * Copyright 2023 CARIAD SE.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */



#include <fep3/base/properties/properties.h>
#include <fep3/core/participant.h>
#include <fep3/core/element_factory.h>
#include <fep3/core/element_base.h>
#include <fep_system/fep_system.h>

#include <fep3/components/logging/logging_service_intf.h>

#include <memory>

namespace {
class EmptyElement : public fep3::core::ElementBase
{
public:
    EmptyElement() : fep3::core::ElementBase("EmptyElement", "1.0.0")
    {
    }
};

//Begin(custom logging sink)
class CustomLoggingSink : public fep3::base::Properties<fep3::ILoggingService::ILoggingSink>
{
public:
    CustomLoggingSink() = default;
    fep3::Result log(fep3::LogMessage log) const override
    {
        // Implement your logging function here and return an error if it can fail
        return{};
    }
    // Optional: To add properties to the sink override this function or use _properties in the log function
    // For an example look at the file logging sink in the header logging_sink_file.hpp
    bool setProperty(const std::string& name, const std::string& value, const std::string& type) override
    {
        if (name == "my_property")
        {
            // Initialize your property here (if necessary)
        }
        return fep3::base::Properties<fep3::ILoggingService::ILoggingSink>::setProperty(name, value, type);
    }
};
//End(custom logging sink)

std::string buildCostlyString()
{
    return "Assume that returning this takes a lot of time";
}

} // namespace

int main(int, const char**)
{
    // Create system to call RPC functions
    fep3::System system("demo_system");

    // creating a participant with an empty element
    // in this case we FORCE the Url of the participant server to use!!!
    auto participant = fep3::core::createParticipant<fep3::core::ElementFactory<EmptyElement>>(
        "demo",
        "1.0.0",
        "demo_system",
        "http://localhost:10025");

    // Get participant proxy
    system.add("demo");
    auto part_proxy = system.getParticipant("demo");

    // Retrieve the logging service component
    auto logging_service = participant.getComponent<fep3::ILoggingService>();
    if (logging_service)
    {
        //Begin(create logger)
        std::shared_ptr<fep3::ILogger> logger = logging_service->createLogger("MyLogger.SubDomain.Domain");
        //End(create logger)

        //Begin(log)
        logger->logError("An Error has occured!");
        //End(log)

        //Begin(check severity)
        if (logger->isErrorEnabled())
        {
            logger->logError(buildCostlyString());
        }
        //End(check severity)

        //Begin(register sink)
        logging_service->registerSink("my_sink", std::make_shared<CustomLoggingSink>());
        //End(register sink)
    }

    // Retrieve the logging service RPC component proxy
    auto logging_service_proxy = part_proxy.getRPCComponentProxyByIID<fep3::rpc::IRPCLoggingService>();
    if (logging_service_proxy)
    {
        //Begin(set sink property)
        std::string file_sink = "file";
        auto properties = logging_service_proxy->getProperties(file_sink);
        properties->setProperty("file_path", "my_log_file.txt", "string");
        //End(set sink property)

        //Begin(set logger filter)
        logging_service_proxy->setLoggerFilter("SubDomain.Domain", { fep3::LoggerSeverity::warning, {"console", "file"} });
        //End(set logger filter)

        //Begin(set custom sink logger filter)
        logging_service_proxy->setLoggerFilter("MyLogger.SubDomain.Domain", { fep3::LoggerSeverity::error, {"my_sink"} });
        //End(set custom sink logger filter)

        //Begin(set json sink property)
        auto propertiesfileJsonSink = logging_service_proxy->getProperties("file_json");
        propertiesfileJsonSink->setProperty("file_path", "my_log_file.txt", "string");
        //End(set json sink property)

        //Begin(set logger json filter)
        logging_service_proxy->setLoggerFilter("SubDomain.Domain", { fep3::LoggerSeverity::warning, {"console", "file_json"} });
        //End(set logger json filter)
    }
}

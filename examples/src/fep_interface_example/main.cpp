/**
 * @copyright
 * @verbatim
 * Copyright 2023 CARIAD SE.
 *
This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#include <chrono>
#include <iostream>

#include <fep3/core.h>
#include <fep3/core/element_configurable.h>
#include <fep3/components/participant_info/participant_info_intf.h>

#include "fep_interface_example.h"


void configureLoggerFilter(const fep3::base::Participant &participant,
                           const std::string &enable_sinks,
                           const std::string &logger_name,
                           fep3::LoggerSeverity severity)
{
    // concept of configuring logging is currently not supported by fep_sdk before state unloaded
    // So use other logging mechanisms for now
}

int main(int argc, const char* argv[])
{
    std::string model_path;
    std::string sigs_in, sigs_out;
    std::string ddl_path;

    // Usually this should be done in the sim-tool, or some file based mechanism like a yaml file, where also the mapping of the
    // ddl struct members to the internal model variables can be specified. In this example a primitive approach will be taken:
    auto cli = (clipp::option("-m", "--model_path") & clipp::value("model path", model_path).doc("(Optional Argument) Set before calling load."),
                clipp::option("-i", "--sigs_in") & clipp::value("signals in", sigs_in).doc("(Optional Argument) Input signals in format: name:ddl_struct_name,... "),
                clipp::option("-o", "--sigs_out") & clipp::value("signals out", sigs_out).doc("(Optional Argument) Output signals in format: name:ddl_struct_name,... "),
                clipp::option("-d", "--ddl_path") & clipp::value("ddl path", ddl_path).doc("(Optional Argument) Path to a DDL description file."));


    fep3::core::ParserDefaultValues cli_default_values = {"FepInterfaceExampleElement", "ExampleSystem", ""};

    auto cli_parser = fep3::core::CommandLineParserFactory::create(cli, cli_default_values);
    cli_parser->parseArgs(argc, argv);

    try
    {
        // This is the simplest way to configure signals - via properties. If the 3rd party simulation tool allows it,
        // configure it from there, or use a nice human readable file as configuration, or use the MM3 ElementType Descr (json, yaml, etc.)
        SimToolInterfaceProperties sim_tool_if_props(model_path, sigs_in, sigs_out, ddl_path);

        auto participant = fep3::base::createParticipant(
            cli_parser->getParticipantName(),
            "1.0.0",
            cli_parser->getSystemName(),
            std::make_shared<ExampleInterfaceElementFactory>(sim_tool_if_props),
            cli_parser->getServerAddressUrl());

        // startup callback for participant
        auto entry_unloaded = [&sim_tool_if_props, &participant]()
        {
            auto logging_service = participant.getComponent<fep3::ILoggingService>();
            if (!logging_service)
            {
                throw std::runtime_error(fep3::getComponentIID<fep3::ILoggingService>() +
                                      " is not part of the given component registry");
            }
            auto logger = logging_service->createLogger("main");
            configureLoggerFilter(participant, "console", "main", fep3::LoggerSeverity::info);

            // prepare property tree entries for unloaded state
            auto configuration_service = participant.getComponent<fep3::IConfigurationService>();
            if (!configuration_service)
            {
                auto message = fep3::getComponentIID<fep3::IConfigurationService>() +
                               " is not part of the given component registry";
                if (logger->isErrorEnabled())
                {
                    logger->logError(message);
                }
                throw std::runtime_error(message);
            }

            auto result = sim_tool_if_props.initConfiguration(*configuration_service);
            if (!result && logger->isErrorEnabled())
            {
                logger->logError(result.getDescription());
            }
            if (logger->isInfoEnabled())
            {
                auto message = std::string("Participant '") + participant.getName() +
                               "' in '" + participant.getSystemName() + "' is unloaded";
                logger->logInfo(message);
            }
        };
        return participant.exec(entry_unloaded);
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what();
        return 1;
    }
}
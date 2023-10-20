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



#include <chrono>
#include <memory>

#include <fep3/cpp.h>
#include <fep3/core.h>

using namespace fep3;
using namespace std::chrono_literals;

namespace {
class MyElement : public fep3::core::ElementBase
{
public:
    MyElement() : fep3::core::ElementBase("MyElementType", "1.0.0")
    {
    }
};

class MyDataJob : public fep3::cpp::DataJob
{
public:
    MyDataJob() : fep3::cpp::DataJob("my_data_job", 1s)
    {
    }
};
} // namespace

int main(int argc, char* argv[])
{
    {
    //Begin(CppBasic)
    // Recommended variation with command line argument parsing
    fep3::base::Participant part = fep3::cpp::createParticipant<fep3::cpp::DataJobElement<MyDataJob>>(argc, argv, "MyParticipant", "MySystem");

    // Most simple variations with hard-coded participant and system name
    fep3::cpp::createParticipant<fep3::cpp::DataJobElement<MyDataJob>>("MyParticipant");
    fep3::cpp::createParticipant<fep3::cpp::DataJobElement<MyDataJob>>("MyParticipant", "MySystem");
    //End(CppBasic)
    }

    {
    //Begin(CppUserArg)
    // Variable to which the command line argument gets copied to. Set the default value here.
    std::string project = "fep";

    // Add the user defined argument to a clipp::parameter or clipp::group object. Make sure to not overwrite any options already defined by the SDK.
    auto cli = (
        clipp::option
        ("-p", "--project")
        & clipp::value("string", project)
        .doc("Values to send"));

    // Create a CommandLineParser object with the factory function. This will add the natively supported command line options to the parser.
    std::unique_ptr<fep3::core::CommandLineParser> clp = fep3::core::CommandLineParserFactory::create(cli, fep3::core::ParserDefaultValues{"MyParticipant", "MySystem", ""});

    // Pass the CommandLineParser object to the createParticipant function
    fep3::base::Participant part = fep3::cpp::createParticipant<fep3::cpp::DataJobElement<MyDataJob>>(argc, argv, std::move(clp));

    // Alternatively call the factory function inline. The last value of the ParserDefaultValues (server address url) can be omitted
    // but not all compilers support partially defined initializer lists.
    fep3::cpp::createParticipant<fep3::cpp::DataJobElement<MyDataJob>>(argc, argv, fep3::core::CommandLineParserFactory::create(cli, {"MyParticipant", "MySystem", ""}));
    //End(CppUserArg)
    }

    {
    //Begin(CoreBasic)
    // Recommended variation with command line argument parsing
    fep3::base::Participant part = fep3::core::createParticipant<fep3::core::ElementFactory<MyElement>>(argc, argv, "1.2.3", {"MyParticipant", "MySystem", ""});

    // If the participant and system name shall always be passed as command line arguments, the ParserDefaultValues can also be omitted.
    fep3::core::createParticipant<fep3::core::ElementFactory<MyElement>>(argc, argv, "1.2.3");
    //End(CoreBasic)

    //Begin(CoreNoParsing)
    fep3::core::createParticipant<fep3::core::ElementFactory<MyElement>>("MyParticipant", "1.2.3", "MySystem");
    //End(CoreNoParsing)
    }

    {
    clipp::parameter parser;
    //Begin(CoreUserArg)
    fep3::core::createParticipant<fep3::core::ElementFactory<MyElement>>(argc, argv, "1.2.3", fep3::core::CommandLineParserFactory::create(parser, {"MyParticipant", "MySystem", ""}));
    //End(CoreUserArg)
    }
}
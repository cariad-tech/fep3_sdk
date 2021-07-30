/**
 * @file
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

    This Source Code Form is subject to the terms of the Mozilla
    Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

If it is not possible or desirable to put the notice in a particular file, then
You may include the notice in a location (such as a LICENSE file in a
relevant directory) where a recipient would be likely to look for such a notice.

You may add additional accurate notices of copyright ownership.

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
    Participant part = fep3::cpp::createParticipant<fep3::cpp::DataJobElement<MyDataJob>>(argc, argv, "MyParticipant", "MySystem");

    // Most simple variations with hard-coded participant and system name
    fep3::cpp::createParticipant<fep3::cpp::DataJobElement<MyDataJob>>("MyParticipant");
    fep3::cpp::createParticipant<fep3::cpp::DataJobElement<MyDataJob>>("MyParticipant", "MySystem");
    //End(CppBasic)
    }

    {
    //Begin(CppUserArg)
    // Variable to which the command line argument gets copied to. Set the default value here.
    std::string project = "fep";

    // Add the user defined argument to a clara::Parser object. Make sure to not overwrite any options already defined by the SDK.
    clara::Parser parser;
    parser |= clara::Opt(project, "string")
        ["-p"]["--project"]
        ("Set the project the participant is part of");

    // Create a CommandLineParser object with the factory function. This will add the natively supported command line options to the parser.
    std::unique_ptr<CommandLineParser> clp = fep3::CommandLineParserFactory::create(parser, ParserDefaultValues{"MyParticipant", "MySystem", ""});

    // Pass the CommandLineParser object to the createParticipant function
    Participant part = fep3::cpp::createParticipant<fep3::cpp::DataJobElement<MyDataJob>>(argc, argv, std::move(clp));

    // Alternatively call the factory function inline. The last value of the ParserDefaultValues (server address url) can be omitted
    // but not all compilers support partially defined initializer lists.
    fep3::cpp::createParticipant<fep3::cpp::DataJobElement<MyDataJob>>(argc, argv, CommandLineParserFactory::create(parser, {"MyParticipant", "MySystem", ""}));
    //End(CppUserArg)
    }

    {
    //Begin(CoreBasic)
    // Recommended variation with command line argument parsing
    Participant part = fep3::core::createParticipant<fep3::core::ElementFactory<MyElement>>(argc, argv, "1.2.3", {"MyParticipant", "MySystem", ""});

    // If the participant and system name shall always be passed as command line arguments, the ParserDefaultValues can also be omitted.
    fep3::core::createParticipant<fep3::core::ElementFactory<MyElement>>(argc, argv, "1.2.3");
    //End(CoreBasic)

    //Begin(CoreNoParsing)
    fep3::core::createParticipant<fep3::core::ElementFactory<MyElement>>("MyParticipant", "1.2.3", "MySystem");
    //End(CoreNoParsing)
    }

    {
    clara::Parser parser;
    //Begin(CoreUserArg)
    fep3::core::createParticipant<fep3::core::ElementFactory<MyElement>>(argc, argv, "1.2.3", CommandLineParserFactory::create(parser, {"MyParticipant", "MySystem", ""}));
    //End(CoreUserArg)
    }
}
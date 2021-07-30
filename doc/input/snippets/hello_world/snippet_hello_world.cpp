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



//Begin(HELLO_WORLD)
#include <fep3/cpp.h>
#include <chrono>

using namespace fep3;
using namespace std::chrono_literals;

class MinimalDataJob : public cpp::DataJob {
public:
    MinimalDataJob() : cpp::DataJob("hello_world", 1s ) {
    }

    Result process(Timestamp sim_time_of_execution) {
        FEP3_LOG_INFO("Hello World!");
        return {};
    }
};

int main(int argc, const char* argv[]) {
    auto part = cpp::createParticipant<cpp::DataJobElement<MinimalDataJob>>(
        argc, argv, "my_participant", "my_system");
    return part.exec();
}
//End(HELLO_WORLD)

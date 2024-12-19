/**
 * Copyright 2023 CARIAD SE.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
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

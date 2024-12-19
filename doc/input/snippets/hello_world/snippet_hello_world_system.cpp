/**
 * Copyright 2023 CARIAD SE.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */



//Begin(HELLO_WORLD_SYSTEM)
#include <thread>
#include <chrono>
#include <fep_system/fep_system.h>

using namespace std::chrono_literals;

int main(int argc, const char* argv[])
{
    auto my_system = fep3::discoverSystem("my_system");

    // start
    my_system.load();
    my_system.initialize();
    my_system.start();

    // run for 5 secs
    std::this_thread::sleep_for(5s);

    // stop
    my_system.stop();
    my_system.deinitialize();
    my_system.unload();
    my_system.shutdown();
    return 0;
}
//End(HELLO_WORLD_SYSTEM)

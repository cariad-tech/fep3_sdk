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

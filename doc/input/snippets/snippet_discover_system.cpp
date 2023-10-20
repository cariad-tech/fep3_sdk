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
#include <fep_system/fep_system.h>
#include <iostream>
using namespace fep3;

int main(int argn, const char* argv[])
{
    // Begin(discover with default timeout)
    try {
        fep3::System discovered_system = discoverSystem("test_system");
    }
    catch (std::exception& e) {
        std::cout << "Discovery of system failed, reason: " << e.what() << "\n";
    }
    // End(discover with default timeout)

    using namespace std::literals::chrono_literals;
    // Begin(discover with custom url and timeout)
    try {
        fep3::System discovered_system =
            discoverSystemByURL("http://230.230.230.2:9990", "test_system", 2000ms);
    }
    catch (std::exception& e) {
        std::cout << "Discovery of system failed, reason: " << e.what() << "\n";
    }
    // End(discover with custom url and timeout)

    // Begin(discover all systems with custom timeout and participant count)
    try {
        std::vector<fep3::System> discovered_systems = discoverAllSystems(5, 2000ms);
    }
    catch (std::exception& e) {
        std::cout << "Discovery of system failed, reason: " << e.what() << "\n";
    }
    // End(discover all systems with custom timeout and participant count)

    // Begin(discover a system with custom timeout and participant names)
    try {
        fep3::System discovered_system =
            discoverSystem("test_system", {"participant_1", "participant_2"}, 2000ms);
    }
    catch (std::exception& e) {
        std::cout << "Discovery of systems failed, reason: " << e.what() << "\n";
    }
    // End(discover a system with custom timeout and participant names)
    return 0;
}

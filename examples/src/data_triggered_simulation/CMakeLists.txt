#
# Copyright @ 2021 VW Group. All rights reserved.
# 
#     This Source Code Form is subject to the terms of the Mozilla
#     Public License, v. 2.0. If a copy of the MPL was not distributed
#     with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
# 
# If it is not possible or desirable to put the notice in a particular file, then
# You may include the notice in a location (such as a LICENSE file in a
# relevant directory) where a recipient would be likely to look for such a notice.
# 
# You may add additional accurate notices of copyright ownership.
# 
#

# find the package
find_package(fep3_participant_cpp REQUIRED)

##################################################################
# DEMO Data Triggered Participant Alice
##################################################################

add_executable(demo_data_triggered_alice
               alice.cpp
               notification_data_receiver.hpp)

set_target_properties(demo_data_triggered_alice PROPERTIES FOLDER "examples/data_triggered_simulation")
# link against the fep3_participant
target_link_libraries(demo_data_triggered_alice PRIVATE
                      fep3_participant_cpp)
#fep3_participant_install(demo_data_triggered_alice examples/bin)
#fep3_participant_deploy(demo_data_triggered_alice)

##################################################################
# DEMO Data Triggered Participant Bob
##################################################################

add_executable(demo_data_triggered_bob
               bob.cpp
               notification_data_receiver.hpp)

set_target_properties(demo_data_triggered_bob PROPERTIES FOLDER "examples/data_triggered_simulation")
# link against the fep3_participant
target_link_libraries(demo_data_triggered_bob PRIVATE
                      fep3_participant_cpp)
#fep3_participant_install(demo_data_triggered_bob examples/bin)
#fep3_participant_deploy(demo_data_triggered_bob)
# Copyright 2023 CARIAD SE.
# 
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

# find the package
find_package(fep3_participant_cpp REQUIRED)

##################################################################
# DEMO Data Triggered Participant Sender
##################################################################

add_executable(demo_data_triggered_sender
               sender.cpp)

set_target_properties(demo_data_triggered_sender PROPERTIES FOLDER "examples/data_triggered_simulation")
target_link_libraries(demo_data_triggered_sender PRIVATE
                      fep3_participant_cpp)
internal_fep3_participant_install(demo_data_triggered_sender examples/bin)
internal_fep3_participant_deploy(demo_data_triggered_sender)

##################################################################
# DEMO Data Triggered Participant Receiver
##################################################################

add_executable(demo_data_triggered_receiver
               receiver.cpp)

set_target_properties(demo_data_triggered_receiver PROPERTIES FOLDER "examples/data_triggered_simulation")
target_link_libraries(demo_data_triggered_receiver PRIVATE
                      fep3_participant_cpp)
internal_fep3_participant_install(demo_data_triggered_receiver examples/bin)
internal_fep3_participant_deploy(demo_data_triggered_receiver)

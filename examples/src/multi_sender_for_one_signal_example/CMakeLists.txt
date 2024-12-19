# Copyright 2023 CARIAD SE.
# 
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

# find the package
find_package(fep3_participant_cpp REQUIRED PATHS ${fep3_participant_DIR})

internal_fep3_participant_create_copy_target(multi_sender_for_one_signal_example)
set_target_properties(fep_participant_file_copy_multi_sender_for_one_signal_example PROPERTIES FOLDER "examples/multi_sender_for_one_signal_example")

##################################################################
# Multi Sender with a CPP DataJob
##################################################################
add_executable(demo_multi_sender multi_sender_main.cpp)

set_target_properties(demo_multi_sender PROPERTIES FOLDER "examples/multi_sender_for_one_signal_example")
# link against the fep3_participant
target_link_libraries(demo_multi_sender PRIVATE fep3_participant_cpp)
add_dependencies(demo_multi_sender fep_participant_file_copy_multi_sender_for_one_signal_example)
internal_fep3_participant_install(demo_multi_sender examples/bin)

##################################################################
# Multi Receiver with a CPP DataJob
##################################################################
add_executable(demo_one_receiver one_receiver_main.cpp)

set_target_properties(demo_one_receiver PROPERTIES FOLDER "examples/multi_sender_for_one_signal_example")
# link against the fep3_participant_cpp API
target_link_libraries(demo_one_receiver PRIVATE fep3_participant_cpp)
add_dependencies(demo_one_receiver fep_participant_file_copy_multi_sender_for_one_signal_example)
internal_fep3_participant_install(demo_one_receiver examples/bin)

#
# Copyright @ 2022 VW Group. All rights reserved.
# 
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

# find the package
find_package(fep3_participant_cpp REQUIRED PATHS ${fep3_participant_DIR})

internal_fep3_participant_create_copy_target(async_waitset_example)
set_target_properties(fep_participant_file_copy_async_waitset_example PROPERTIES FOLDER "examples/async_waitset_example")

##################################################################
# DEMO EASY Sender with a CPP DataJob
##################################################################
add_executable(demo_async_waitset_sender
               async_waitset_sender.cpp)

set_target_properties(demo_async_waitset_sender PROPERTIES FOLDER "examples/async_waitset_example")
# link against the fep3_participant
target_link_libraries(demo_async_waitset_sender  PRIVATE
                      fep3_participant_core fep3_participant_cpp)

fep3_participant_link_pthread(demo_async_waitset_sender)
add_dependencies(demo_async_waitset_sender fep_participant_file_copy_async_waitset_example)
internal_fep3_participant_install(demo_async_waitset_sender examples/bin)

##################################################################
# DEMO EASY Receiver with a CPP DataJob
##################################################################
add_executable(demo_async_waitset_receiver
               async_waitset_receiver.cpp)

set_target_properties(demo_async_waitset_receiver PROPERTIES FOLDER "examples/async_waitset_example")
# link against the fep3_participant_cpp API
target_link_libraries(demo_async_waitset_receiver PRIVATE
                      fep3_participant_cpp)
add_dependencies(demo_async_waitset_receiver fep_participant_file_copy_async_waitset_example)
internal_fep3_participant_install(demo_async_waitset_receiver examples/bin)

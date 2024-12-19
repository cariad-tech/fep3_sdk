# Copyright 2023 CARIAD SE.
# 
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.


# find the package
find_package(fep3_participant_core REQUIRED)
find_package(dev_essential REQUIRED COMPONENTS ddl)

internal_fep3_participant_create_copy_target(easy_core_example)
set_target_properties(fep_participant_file_copy_easy_core_example PROPERTIES FOLDER "examples/easy_core_example")

##################################################################
# DEMO EASY Sender with a CORE ElementBase
##################################################################
add_executable(demo_easy_core_sender
               core_sender_main.cpp
               example_ddl_types.h)

set_target_properties(demo_easy_core_sender PROPERTIES FOLDER "examples/easy_core_example")
# link against the fep3_participant
target_link_libraries(demo_easy_core_sender PRIVATE
                      fep3_participant_core
                      dev_essential::ddl)
add_dependencies(demo_easy_cpp_sender fep_participant_file_copy_easy_core_example)
internal_fep3_participant_install(demo_easy_core_sender examples/bin)

##################################################################
# DEMO EASY Receiver with a CORE ElementBase
##################################################################
add_executable(demo_easy_core_receiver
               core_receiver_main.cpp
               example_ddl_types.h)

set_target_properties(demo_easy_core_receiver PROPERTIES FOLDER "examples/easy_core_example")
# link against the fep3_participant_core API
target_link_libraries(demo_easy_core_receiver PRIVATE
                      fep3_participant_core
                      dev_essential::ddl)
add_dependencies(demo_easy_core_receiver fep_participant_file_copy_easy_core_example)
internal_fep3_participant_install(demo_easy_core_receiver examples/bin)


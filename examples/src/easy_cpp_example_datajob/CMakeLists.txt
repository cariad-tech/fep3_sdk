# Copyright 2023 CARIAD SE.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.


# find the package
find_package(fep3_participant_cpp REQUIRED PATHS ${fep3_participant_DIR})
find_package(dev_essential REQUIRED COMPONENTS ddl)

internal_fep3_participant_create_copy_target(easy_cpp_example)
set_target_properties(fep_participant_file_copy_easy_cpp_example PROPERTIES FOLDER "examples/easy_cpp_example")

##################################################################
# DEMO EASY Sender with a CPP DataJob
##################################################################
add_executable(demo_easy_cpp_sender
               cpp_sender_main.cpp
               example_ddl_types.h)

set_target_properties(demo_easy_cpp_sender PROPERTIES FOLDER "examples/easy_cpp_example")
# link against the fep3_participant
target_link_libraries(demo_easy_cpp_sender PRIVATE
                      fep3_participant_cpp
                      dev_essential::ddl)
add_dependencies(demo_easy_cpp_sender fep_participant_file_copy_easy_cpp_example)
internal_fep3_participant_install(demo_easy_cpp_sender examples/bin)

##################################################################
# DEMO EASY Receiver with a CPP DataJob
##################################################################
add_executable(demo_easy_cpp_receiver
               cpp_receiver_main.cpp
               example_ddl_types.h)

set_target_properties(demo_easy_cpp_receiver PROPERTIES FOLDER "examples/easy_cpp_example")
# link against the fep3_participant_cpp API
target_link_libraries(demo_easy_cpp_receiver PRIVATE
                      fep3_participant_cpp
                      dev_essential::ddl)
add_dependencies(demo_easy_cpp_receiver fep_participant_file_copy_easy_cpp_example)
internal_fep3_participant_install(demo_easy_cpp_receiver examples/bin)

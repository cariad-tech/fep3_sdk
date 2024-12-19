# Copyright 2023 CARIAD SE.
# 
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.


# find the package
find_package(fep3_participant_core REQUIRED)

internal_fep3_participant_create_copy_target(default_job_example)
set_target_properties(fep_participant_file_copy_default_job_example PROPERTIES FOLDER "examples/default_job_example")

##################################################################
# DEMO EASY Sender with a CORE ElementBase
##################################################################
add_executable(demo_default_job_sender
               default_job_sender.cpp)

set_target_properties(demo_default_job_sender PROPERTIES FOLDER "examples/default_job_example")
# link against the fep3_participant
target_link_libraries(demo_default_job_sender PRIVATE
                      fep3_participant_core)
add_dependencies(demo_default_job_sender fep_participant_file_copy_default_job_example)
internal_fep3_participant_install(demo_default_job_sender examples/bin)

##################################################################
# DEMO Default Job Receiver with a core DefaultJobElement
##################################################################
add_executable(demo_default_job_receiver
               default_job_receiver.cpp)

set_target_properties(demo_default_job_receiver PROPERTIES FOLDER "examples/default_job_example")
# link against the fep3_participant_core API
target_link_libraries(demo_default_job_receiver PRIVATE
                      fep3_participant_core)
add_dependencies(demo_default_job_receiver fep_participant_file_copy_default_job_example)
internal_fep3_participant_install(demo_default_job_receiver examples/bin)


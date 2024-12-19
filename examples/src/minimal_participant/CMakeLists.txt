# Copyright 2023 CARIAD SE.
# 
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.


# find the package
find_package(fep3_participant_cpp REQUIRED)

##################################################################
# DEMO EASY Sender with a CPP DataJob
##################################################################
add_executable(demo_minimal_participant
               minimal_participant.cpp
)

set_target_properties(demo_minimal_participant PROPERTIES FOLDER "examples/minimal_participant")

# link against the fep3_participant
target_link_libraries(demo_minimal_participant PRIVATE
                      fep3_participant_cpp)
internal_fep3_participant_install(demo_minimal_participant examples/bin)
internal_fep3_participant_deploy(demo_minimal_participant)

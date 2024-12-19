# Copyright 2023 CARIAD SE.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#

# find the package
find_package(fep3_participant_core REQUIRED)
find_package(fep3_participant_cpp REQUIRED)
find_package(dev_essential REQUIRED COMPONENTS system ddl)

internal_fep3_participant_create_copy_target(fep_interface_example)
set_target_properties(fep_participant_file_copy_fep_interface_example PROPERTIES FOLDER "examples/fep_interface_example")

# #################################################################
# DEMO FEP Interface Example
# #################################################################
add_executable(fep_interface_example
    fep_interface_example.cpp
    fep_interface_example.h
    main.cpp
    custom_sim_tool.hpp
    string_map_property_type.hpp
    delay_property_type.hpp
)

set_target_properties(fep_interface_example PROPERTIES FOLDER "examples/fep_interface_example")

target_link_libraries(fep_interface_example
    dev_essential::ddl
    dev_essential::system
    fep3_participant_core
    fep3_participant_cpp
)

add_dependencies(fep_interface_example fep_participant_file_copy_fep_interface_example)

# copy DDL
install(FILES fep_interface_example.description DESTINATION examples/bin)

internal_fep3_participant_install(fep_interface_example examples/bin)
internal_fep3_participant_deploy(fep_interface_example)
#
# Copyright @ 2021 VW Group. All rights reserved.
# 
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

# Find the package
find_package(fep3_participant_cpp REQUIRED)
find_package(dev_essential REQUIRED COMPONENTS pkg_rpc)

add_library(demo_components_plugin SHARED
            components_plugin.cpp
            component_a_intf.h
            component_b_intf.h)
set_target_properties(demo_components_plugin PROPERTIES FOLDER "examples/plugins")
target_link_libraries(demo_components_plugin PRIVATE fep3_participant_cpp_plugin)
internal_fep3_participant_install(demo_components_plugin examples/bin)

# Demo for demonstrating demo_plugin_element,
add_executable(demo_plugin_element plugin_element.cpp)

set_target_properties(demo_plugin_element PROPERTIES FOLDER "examples/plugins")
# Link against the fep3_participant
target_link_libraries(demo_plugin_element PRIVATE
                      fep3_participant_core)

internal_fep3_participant_install(demo_plugin_element examples/bin)

# Update fep3_participant.fep_components 
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/demo_components_plugin.fep_components DESTINATION examples/bin)

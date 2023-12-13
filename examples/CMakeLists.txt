#
# Copyright @ 2021 VW Group. All rights reserved.
# 
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

if (NOT examples_integrated)
    project(fep3-sdk-examples)

    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_STANDARD 17)

    # Enable project folder structure for Visual Studio IDE
    set_property(GLOBAL PROPERTY USE_FOLDERS true)

endif()

add_subdirectory(src/rpc_service_bus)
add_subdirectory(src/easy_cpp_example_datajob)
add_subdirectory(src/easy_core_example_element)
add_subdirectory(src/logging_example)
add_subdirectory(src/data_triggered_simulation)
add_subdirectory(src/dynamic_array)
add_subdirectory(src/minimal_participant)
add_subdirectory(src/component_plugin)
add_subdirectory(src/async_waitset_example)
add_subdirectory(src/multi_sender_for_one_signal_example)
add_subdirectory(src/signal_mapping_example)
add_subdirectory(src/fep_interface_example)
add_subdirectory(src/default_job_example)

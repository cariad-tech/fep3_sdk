# Copyright 2023 CARIAD SE.
# 
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.


# find the package
find_package(fep3_participant_cpp REQUIRED)
find_package(fep3_system REQUIRED)
find_package(dev_essential REQUIRED COMPONENTS system)

##################################################################
# CONFIGURATION SERVICE SNIPPETS
##################################################################

add_executable(snippet_config_service
               snippet_config_service.cpp)

target_include_directories(snippet_config_service PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(snippet_config_service PROPERTIES FOLDER "examples/snippets")
target_link_libraries(snippet_config_service PRIVATE
                      fep3_participant_cpp)

##################################################################
# LOGGING SERVICE SNIPPETS
##################################################################

add_executable(snippet_logging_service
               snippet_logging_service.cpp)

target_include_directories(snippet_logging_service PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(snippet_logging_service PROPERTIES FOLDER "examples/snippets")
target_link_libraries(snippet_logging_service PRIVATE
                      fep3_participant_cpp
                      fep3_system)
##################################################################
# DATA TRANSFER SNIPPETS
##################################################################

add_executable(snippet_data_transfer
               snippet_data_transfer.cpp)

target_include_directories(snippet_data_transfer PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(snippet_data_transfer PROPERTIES FOLDER "examples/snippets")
target_link_libraries(snippet_data_transfer PRIVATE
                      fep3_participant_cpp
                      fep3_system)

##################################################################
# DATA REGISTRY SNIPPETS
##################################################################

add_executable(snippet_data_registry
               snippet_data_registry.cpp)

target_include_directories(snippet_data_registry PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(snippet_data_registry PROPERTIES FOLDER "examples/snippets")
target_link_libraries(snippet_data_registry PRIVATE
                      fep3_participant_cpp
                      fep3_system)

##################################################################
# PARTICIPANT SNIPPETS
##################################################################

add_executable(snippet_participant
               snippet_participant.cpp)

target_include_directories(snippet_participant PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(snippet_participant PROPERTIES FOLDER "examples/snippets")
target_link_libraries(snippet_participant PRIVATE
                      fep3_participant_cpp)
##################################################################
# MIGRATION GUIDE SNIPPETS
##################################################################

add_executable(snippet_migration_guide
               snippet_migration_guide_fep_3.cpp)

target_include_directories(snippet_migration_guide PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(snippet_migration_guide PROPERTIES FOLDER "examples/snippets")
target_link_libraries(snippet_migration_guide PRIVATE
                      fep3_participant_cpp)

##################################################################
# MINIMAL PARTICIPANT SNIPPETS
##################################################################

add_executable(snippet_minimal_participant
               snippet_minimal_participant.cpp)

target_include_directories(snippet_minimal_participant PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(snippet_minimal_participant PROPERTIES FOLDER "examples/snippets")
target_link_libraries(snippet_minimal_participant PRIVATE
                      fep3_participant_cpp)

##################################################################
# CORE RECEIVER SNIPPETS
##################################################################

add_executable(snippet_core_receiver_main
               snippet_core_receiver_main.cpp)

target_include_directories(snippet_core_receiver_main PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(snippet_core_receiver_main PROPERTIES FOLDER "examples/snippets")
target_link_libraries(snippet_core_receiver_main PRIVATE
                      fep3_participant_cpp)

##################################################################
# ELEMENT FACTORY SNIPPETS
##################################################################

add_executable(snippet_element_factory
               snippet_element_factory.cpp)

target_include_directories(snippet_element_factory PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(snippet_element_factory PROPERTIES FOLDER "examples/snippets")
target_link_libraries(snippet_element_factory PRIVATE
                      fep3_participant_cpp)

##################################################################
# CORE SENDER SNIPPETS
##################################################################

add_executable(snippet_core_sender
               snippet_core_sender.cpp)

target_include_directories(snippet_core_sender PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(snippet_core_sender PROPERTIES FOLDER "examples/snippets")
target_link_libraries(snippet_core_sender PRIVATE
                      fep3_participant_cpp)

##################################################################
# WAITING FOR RTI DDS WRITERS SNIPPETS
##################################################################

add_executable(snippet_waiting_for_writers
               snippet_waiting_for_writers.cpp)

target_include_directories(snippet_waiting_for_writers PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(snippet_waiting_for_writers PROPERTIES FOLDER "examples/snippets")
target_link_libraries(snippet_waiting_for_writers PRIVATE
                      fep3_participant_cpp)

##################################################################
# COMPONENT PLUGINS SNIPPETS
##################################################################

add_executable(snippet_component_plugin
               snippet_component_plugin.cpp)

target_include_directories(snippet_component_plugin PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(snippet_component_plugin PROPERTIES FOLDER "examples/snippets")
target_link_libraries(snippet_component_plugin PRIVATE
                      fep3_participant_cpp)

##################################################################
# DISCOVER SYSTEM SNIPPETS
##################################################################

add_executable(snippet_discover_system
               snippet_discover_system.cpp)

target_include_directories(snippet_discover_system PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(snippet_discover_system PROPERTIES FOLDER "examples/snippets")
target_link_libraries(snippet_discover_system PRIVATE
                      fep3_system
                      dev_essential::result)
if(MSVC)
# system uses std::unique_ptr for pimpl which causes 4251 error
    target_compile_options(snippet_discover_system PRIVATE /wd4251)
else()
    target_link_libraries(snippet_discover_system PRIVATE
                          ${CMAKE_DL_LIBS})
endif(MSVC)

##################################################################
# ASYNC WAITSET SNIPPETS
##################################################################

add_executable(snippet_async_waitset
               snippet_async_waitset.cpp)

target_include_directories(snippet_async_waitset PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(snippet_async_waitset PROPERTIES FOLDER "examples/snippets")
target_link_libraries(snippet_async_waitset PRIVATE
                      fep3_participant_cpp)

##################################################################
# DATA-TRIGGERED JOB SNIPPETS
##################################################################

add_executable(snippet_data_triggered_job
               snippet_data_triggered_job.cpp)

target_include_directories(snippet_data_triggered_job PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(snippet_data_triggered_job PROPERTIES FOLDER "examples/snippets")
target_link_libraries(snippet_data_triggered_job  PRIVATE
                      fep3_participant_cpp)

##################################################################
# DEFAULT JOB SNIPPETS
##################################################################

add_executable(snippet_default_job
               snippet_default_job.cpp)

target_include_directories(snippet_default_job PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(snippet_default_job PROPERTIES FOLDER "examples/snippets")
target_link_libraries(snippet_default_job PRIVATE
                      fep3_participant_cpp)

##################################################################
# HEllO WORLD SNIPPETS
##################################################################
add_subdirectory(hello_world)


##################################################################
# COMPONENT PLUGIN ANALYZER SNIPPET
##################################################################
add_executable(snippet_component_plugin_analyzer
               snippet_component_plugin_analyzer.cpp)

#start_cmake_snippet_component_plugin_analyzer
find_package(fep3_sdk_component_plugin_analyzer REQUIRED)
target_link_libraries(snippet_component_plugin_analyzer 
PRIVATE
    fep3_sdk_component_plugin_analyzer
)
#end_cmake_snippet_component_plugin_analyzer
set_target_properties(snippet_component_plugin_analyzer PROPERTIES FOLDER "examples/snippets")         
target_compile_definitions(snippet_component_plugin_analyzer 
PUBLIC
    NATIVE_COMPONENTS_PATH="$<TARGET_FILE:fep_components_plugin>"
    DDS_SIM_BUS_PATH="$<TARGET_FILE:fep3_connext_dds_plugin>"
    DDS_SERVICE_BUS_PATH="$<TARGET_FILE:fep3_dds_service_bus_plugin>"
)

##################################################################
# MONITOR PARTICIPANT SNIPPETS
##################################################################

add_executable(snippet_monitor_participant
               snippet_monitor_participant.cpp)

target_include_directories(snippet_monitor_participant PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(snippet_monitor_participant PROPERTIES FOLDER "examples/snippets")
target_link_libraries(snippet_monitor_participant PRIVATE
                      fep3_system
                      dev_essential::result)
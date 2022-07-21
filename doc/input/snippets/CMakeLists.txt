#
# Copyright @ 2021 VW Group. All rights reserved.
# 
#     This Source Code Form is subject to the terms of the Mozilla
#     Public License, v. 2.0. If a copy of the MPL was not distributed
#     with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
# 
# If it is not possible or desirable to put the notice in a particular file, then
# You may include the notice in a location (such as a LICENSE file in a
# relevant directory) where a recipient would be likely to look for such a notice.
# 
# You may add additional accurate notices of copyright ownership.
# 
#


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

add_subdirectory(hello_world)

# Copyright 2023 CARIAD SE.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.


# find the package
find_package(fep3_participant_cpp REQUIRED PATHS ${fep3_participant_DIR})
find_package(dev_essential 1.3.0 REQUIRED COMPONENTS system)

internal_fep3_participant_create_copy_target(signal_mapping_example)
set_target_properties(fep_participant_file_copy_signal_mapping_example PROPERTIES FOLDER "examples/signal_mapping_example")

##################################################################
# DEMO Signal Mapping Sender
##################################################################
add_executable(demo_signal_mapping_sender
               signal_mapping_sender_main.cpp
               example_ddl_types.h)

set_target_properties(demo_signal_mapping_sender PROPERTIES FOLDER "examples/signal_mapping_example")
# link against the fep3_participant
target_link_libraries(demo_signal_mapping_sender PRIVATE
                      fep3_participant_core
                      dev_essential::system)

add_dependencies(demo_signal_mapping_sender fep_participant_file_copy_signal_mapping_example)
internal_fep3_participant_install(demo_signal_mapping_sender examples/bin)

add_custom_command(TARGET demo_signal_mapping_sender POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/demo_data/a.description $<TARGET_FILE_DIR:demo_signal_mapping_sender>/demo_data/a.description
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/demo_data/b.description $<TARGET_FILE_DIR:demo_signal_mapping_sender>/demo_data/b.description
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/demo_data/c.description $<TARGET_FILE_DIR:demo_signal_mapping_sender>/demo_data/c.description)

##################################################################
# DEMO Signal Mapping Receiver
##################################################################
add_executable(demo_signal_mapping_receiver
               signal_mapping_receiver_main.cpp
               example_ddl_types.h)

set_target_properties(demo_signal_mapping_receiver PROPERTIES FOLDER "examples/signal_mapping_example")
# link against the fep3_participant_cpp API
target_link_libraries(demo_signal_mapping_receiver PRIVATE
                      fep3_participant_core
                      dev_essential::system)

add_dependencies(demo_signal_mapping_receiver fep_participant_file_copy_signal_mapping_example)
internal_fep3_participant_install(demo_signal_mapping_receiver examples/bin)

add_custom_command(TARGET demo_signal_mapping_receiver POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/demo_data/a.description $<TARGET_FILE_DIR:demo_signal_mapping_receiver>/demo_data/a.description
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/demo_data/b.description $<TARGET_FILE_DIR:demo_signal_mapping_receiver>/demo_data/b.description
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/demo_data/c.description $<TARGET_FILE_DIR:demo_signal_mapping_receiver>/demo_data/c.description)

install(FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/demo_data/a.description"
    "${CMAKE_CURRENT_SOURCE_DIR}/demo_data/b.description"
    "${CMAKE_CURRENT_SOURCE_DIR}/demo_data/c.description"
    DESTINATION examples/bin/demo_data)

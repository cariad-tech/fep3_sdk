#
# Copyright @ 2021 VW Group. All rights reserved.
# 
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.


# find the package
find_package(fep3_participant_cpp REQUIRED PATHS ${fep3_participant_DIR})
find_package(dev_essential)

internal_fep3_participant_create_copy_target(dynamic_arrays_example)
set_target_properties(fep_participant_file_copy_dynamic_arrays_example PROPERTIES FOLDER "examples/dynamic_array_example")

add_custom_target(demo_dynamic_array_sender_copy_description)

add_custom_command(TARGET demo_dynamic_array_sender_copy_description POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:demo_dynamic_array_sender>/demo_data/
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/demo_data/demo_dynamic_array.description
     $<TARGET_FILE_DIR:demo_dynamic_array_sender>/demo_data/
)
set_target_properties(demo_dynamic_array_sender_copy_description PROPERTIES FOLDER "examples/dynamic_array_example")
##################################################################
# DEMO array sender with a CORE ElementBase
##################################################################
add_executable(demo_dynamic_array_sender
               demo_dynamic_array_sender_main.cpp
               dynamic_array_data_sample.h
               stream_type_file_ref_big.h
               demo_dynamic_array.h)

set_target_properties(demo_dynamic_array_sender PROPERTIES FOLDER "examples/dynamic_array_example")
# link against the fep3_participant
target_link_libraries(demo_dynamic_array_sender PRIVATE
                      fep3_participant_cpp
                      dev_essential::dev_essential)
add_dependencies(demo_easy_cpp_sender fep_participant_file_copy_dynamic_arrays_example demo_dynamic_array_sender_copy_description)
internal_fep3_participant_install(demo_dynamic_array_sender examples/bin)


##################################################################
# DEMO array receiver with a CORE ElementBase
##################################################################
add_executable(demo_dynamic_array_receiver
               demo_dynamic_array_receiver_main.cpp
               dynamic_array_data_sample.h
               stream_type_file_ref_big.h
               demo_dynamic_array.h)

set_target_properties(demo_dynamic_array_receiver PROPERTIES FOLDER "examples/dynamic_array_example")
# link against the fep3_participant_core API
target_link_libraries(demo_dynamic_array_receiver PRIVATE
                      fep3_participant_cpp
                      dev_essential::dev_essential)
add_dependencies(demo_dynamic_array_receiver fep_participant_file_copy_dynamic_arrays_example demo_dynamic_array_sender_copy_description)
internal_fep3_participant_install(demo_dynamic_array_receiver examples/bin)

if(MSVC)
    #disable warning for using array [] without size
    set_source_files_properties(demo_dynamic_array.h COMPILE_FLAGS /wd4200)
endif()

install(FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/demo_data/demo_dynamic_array.description"
    DESTINATION examples/bin/demo_data)

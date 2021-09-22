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


##################################################################
# Standalone minimum setup
##################################################################
set(is_standalone OFF)
if(is_standalone)
#Begin(CMAKE_SETUP)
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 14)
project(hello_world)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)
#End(CMAKE_SETUP)
else()
# To generate the version number dynamically
configure_file(conanfile.txt.in ${DOC_BUILD_DIR}/snippets/hello_world/conanfile.txt)
endif()

##################################################################
# HELLO WORLD
##################################################################

#Begin(CMAKE_HELLO_WORLD)
find_package(fep3_participant_cpp REQUIRED)
find_package(fep3_system REQUIRED)
find_package(dev_essential REQUIRED COMPONENTS system)

add_executable(snippet_hello_world
               snippet_hello_world.cpp)

add_executable(snippet_hello_world_system
               snippet_hello_world_system.cpp)

target_link_libraries(snippet_hello_world PRIVATE
                      fep3_participant_cpp)
target_link_libraries(snippet_hello_world_system PRIVATE
                      fep3_system
                      dev_essential::system
                      ${CMAKE_DL_LIBS})
#End(CMAKE_HELLO_WORLD)

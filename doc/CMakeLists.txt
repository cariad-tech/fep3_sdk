#
# Copyright @ 2022 VW Group. All rights reserved.
# 
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

find_package(Doxygen REQUIRED)

# directory for doxygen and sphinx config files which were preprocessed by cmake
set(DOXYGEN_INPUTS "${fep3_participant_DIR}/include ${fep3_system_DIR}/include")
set(DOXYGEN_DISABLE_WARNINGS_AS_ERRORS TRUE)

set(DOXYGEN_WARN_IF_UNDOCUMENTED "YES")

if(DOXYGEN_DISABLE_WARNINGS_AS_ERRORS)
    set(DOXYGEN_WARNINGS_AS_ERRORS "NO")
    message(WARNING "DOXYGEN_WARNINGS_AS_ERRORS is deactivated")
else(DOXYGEN_DISABLE_WARNINGS_AS_ERRORS)
    set(DOXYGEN_WARNINGS_AS_ERRORS "YES")
endif(DOXYGEN_DISABLE_WARNINGS_AS_ERRORS)

set(DOXYGEN_EXCLUDES "\" \"" )
set(DOXYGEN_EXCLUDE_PATTERNS "*/rpc_services/part*/*_client.h */rpc_services/part*/*_service.h */rpc_services/data*/*_client.h */rpc_services/data*/*_service.h" )

set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/output)
set(DOXYGEN_XML_OUTPUT_DIR ${DOXYGEN_OUTPUT_DIR}/xml)

set(DOXYGEN_PREDEFINED "FEP3_SYSTEM_EXPORT= FEP3_PARTICIPANT_EXPORT= FEP3_CONTROLLER_EXPORT=")

if (WIN32)
    set(SPHINX_BUILD_EXE "sphinx-build.exe")
else(WIN32)
    set(SPHINX_BUILD_EXE "sphinx-build")
endif (WIN32)

# compile check code snippets
add_subdirectory(input/snippets)

# create configurations
configure_file(input/conf.py.in ${CMAKE_CURRENT_BINARY_DIR}/sphinx_config/conf.py)
configure_file(input/doxygen.cfg.in ${CMAKE_CURRENT_BINARY_DIR}/doxygen.cfg)


# This is to avoid double documentation build on install
set(DOCUMENTATION_GENERATED_FLAG_FILE ${CMAKE_CURRENT_BINARY_DIR}/doc_generated)

add_custom_target(fep_sdk_user_guide ALL DEPENDS ${DOCUMENTATION_GENERATED_FLAG_FILE})
set_target_properties(fep_sdk_user_guide PROPERTIES FOLDER process/doc)

add_custom_command(OUTPUT ${DOCUMENTATION_GENERATED_FLAG_FILE}
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doxygen.cfg
    # -W -n append to sphinx build in order to activate strict building (no warnings at all are tolerated)
    COMMAND ${SPHINX_BUILD_EXE} -b html -c ${CMAKE_CURRENT_BINARY_DIR}/sphinx_config ${CMAKE_CURRENT_SOURCE_DIR}/input ${CMAKE_CURRENT_BINARY_DIR}/output/html
    # And mark that documentation as created
    COMMAND ${CMAKE_COMMAND} -E touch ${DOCUMENTATION_GENERATED_FLAG_FILE}
    WORKING_DIRECTORY .
)

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/output/html/
        DESTINATION doc/html
)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/output/doxygen_html/
        DESTINATION doc/doxygen_html
)
install(FILES fep3-sdk.html
        DESTINATION doc
)

<!--
  Copyright @ 2021 VW Group. All rights reserved.
  
      This Source Code Form is subject to the terms of the Mozilla
      Public License, v. 2.0. If a copy of the MPL was not distributed
      with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
  
  If it is not possible or desirable to put the notice in a particular file, then
  You may include the notice in a location (such as a LICENSE file in a
  relevant directory) where a recipient would be likely to look for such a notice.
  
  You may add additional accurate notices of copyright ownership.
  
  -->

# FEP 3 SDK

# Description

This package contains some examples and the documentation of the FEP 3 SDK.

### FEP 3 SDK Documentation

* see fep3_sdk/doc/fep3-sdk.html

## How to use

The FEP 3 SDK provides a CMake >= 3.17 configuration. See the fep3_system and fep3_participant documentation on how to use the different libraries in your own CMake projects.

## Build order and dependencies

To build the packages bottom-up, the following order is needed. Instructions on how to build each library, can be found in the corresponding library repository.

* dev_essential
* fep3_participant (dev_essential)
* fep3_system  (dev_essential, fep3_participant)
* fep3_sdk (dev_essential, fep3_participant, fep3_system)

## How to build using only cmake ###
### Prerequisites
- Download [CMake](https://cmake.org/) at least in version 3.17.0
- Using [git](https://git-scm.com/), clone the repository and checkout the desired branch (e.g. `master`)
-<a id="howtodevessential"></a> Build the dev_essential library as described in the [Readme](https://www.github.com/dev-essential) file.
- <a id="howtoparticipant"></a> Build the fep3_participant library as described in the [Readme](https://www.github.com/fep3_participant) file.
- <a id="howtosystem"></a> Build the fep3_system library as described in the [Readme](https://www.github.com/fep3_system) file.

### Optional
- <a id="howtodoxygen"></a> [Doxygen](https://www.doxygen.nl/index.html) version 1.8.14. Doxygen executable should be located under <doxygen_dir>/bin
- <a id="howtosphinx"></a> [Sphinx](https://pypi.org/project/Sphinx/) version 3.3.0.
### Build with cmake
- Run the following command, (adaptations may be needed in case a different Visual Studio version is used or different configuration should be built).
    ```shell
    > cmake.exe -H<root_dir> -B<build_dir> -G "Visual Studio 16 2019" -A x64 -T v142 -DCMAKE_INSTALL_PREFIX=<install_dir> -Dfep3_system_DIR=<fep_system_dir> -Dfep3_participant_DIR=<fep3_participant_dir> -Ddev_essential_DIR=<dev_essential_dir>/lib/cmake/dev_essential -Dfep3_sdk_cmake_enable_documentation=False
    > cmake --build . --target install --config Release
    ```
    - <root_dir> The path where the  *fep sdk* library is checked out and the main CMakeLists.txt is located.
    - <build_dir> The build directory
    - <install_dir> Path where the built artifacts will be installed.
    - <dev_essential_dir> The path were the [*dev_essential*](#howtodevessential) library was installed. File *dev_essential-config.cmake* is located under  <dev_essential_dir>\lib\cmake\dev_essential.
    - <fep3_participant_dir> The path were the [*fep3_participant*](#howtoparticipant) library was installed. File *fep3_participant-config.cmake* is located in this folder.
    - <fep_system_dir> The path were the [*fep3_system*](#howtosystem) library was installed. File *fep3_system-config.cmake* is located in this folder.
    >  **Note**: The above cmake calls are exemplary for using Windows and Visual Studio as generator. For gcc the addition of -DCMAKE_POSITION_INDEPENDENT_CODE=True is needed. Also depending on the generator used, the *--config* in the build step could be ignored and the adaptation of CMAKE_CONFIGURATION_TYPES or CMAKE_BUILD_TYPE could be necessary for building in other configurations.

### Additional Cmake options
- Enable the documentation
    - The **fep3_sdk_cmake_enable_documentation** variable activates the build of the documentation. Default value is True. For this flag [doxygen](#howtodoxygen) and [sphinx](#howtosphinx) are required. The doxygen executable should be located in *${DOXYGEN_ROOT}/bin/doxygen.exe* and the cmake variable *DOXYGEN_ROOT* should be set accordingly.
    - A call to cmake with documentation activated could look like:
        ```shell
        > cmake.exe -H<root_dir> -B<build_dir> -G "Visual Studio 16 2019" -A x64 -T v141 -DCMAKE_INSTALL_PREFIX=<install_dir> -Dfep3_system_DIR=<fep_system_dir> -Dfep3_participant_DIR=<fep3_participant_dir> -Ddev_essential_DIR=<dev_essential_dir>/lib/cmake/dev_essential -Dfep3_sdk_cmake_enable_documentation=True -DDOXYGEN_ROOT=<doxyxen_dir>
        > cmake --build . --target install --config Release
        ```
### Tested compilers
- Windows 10 x64 with Visual Studio C++ 2019 and v142 Toolset.
- Linux Ubuntu 18.04 LTS x64 with GCC 7.5 and libstdc++14 (C++14 ABI)

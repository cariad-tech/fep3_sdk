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


.. _Installation:

============
Installation
============

Basic Requirements
==================

* Supported OS: Windows 7, Windows 10 or Linux
* Required software: python, pip, conan, cmake

Install packages
================

.. code-block:: shell

   conan install fep_sdk/{VERSION}@fep/stable
   conan install fep_sdk_participant/{VERSION}@fep/stable
   conan install fep_sdk_system/{VERSION}@fep/stable

Read documentation on local server (default: http://0.0.0.0:8000/).

.. code-block:: shell

   cd ~/your/conan/fep_sdk/package/doc/html
   python -m http.server

Hello World
===========

Create a simple hello world participant :file:`hello_world.cpp`.

.. literalinclude:: snippets/hello_world/snippet_hello_world.cpp
   :start-after: //Begin(HELLO_WORLD)
   :end-before: //End(HELLO_WORLD)

Create a simple hello world system :file:`hello_world_system.cpp`.

.. literalinclude:: snippets/hello_world/snippet_hello_world_system.cpp
   :start-after: //Begin(HELLO_WORLD_SYSTEM)
   :end-before: //End(HELLO_WORLD_SYSTEM)

Create a :file:`CMakeLists.txt`.

.. literalinclude:: snippets/hello_world/CMakeLists.txt
   :start-after: #Begin(CMAKE_SETUP)
   :end-before: #End(CMAKE_SETUP)

.. literalinclude:: snippets/hello_world/CMakeLists.txt
   :start-after: #Begin(CMAKE_HELLO_WORLD)
   :end-before: #End(CMAKE_HELLO_WORLD)

Create :file:`conanfile.txt`.

.. literalinclude:: snippets/hello_world/conanfile.txt
   :start-after: #Begin(conanfile)
   :end-before: #End(conanfile)

Build it in :file:`build` folder.

.. code-block:: shell

   mkdir build && cd build
   conan install ..  
   cmake ..
   cmake --build .

Run two programs in separate terminals

.. code-block:: shell

   ./build/bin/hello_world
   ./build/bin/hello_world_system

Result

.. code-block::

   [04.05.2021 - 14:22:07]: my_participant@participant  ST: 0[ns]  Info Successfully loaded element
   [04.05.2021 - 14:22:07]: my_participant@participant  ST: 0[ns]  Info Successfully initialized element
   [04.05.2021 - 14:22:07]: my_participant@participant  ST: 0[ns]  Info Successfully initialized components
   [04.05.2021 - 14:22:07]: my_participant@participant  ST: 0[ns]  Info Successfully tensed components
   [04.05.2021 - 14:22:07]: my_participant@participant  ST: 0[ns]  Info Successfully ran element
   [04.05.2021 - 14:22:07]: my_participant@participant  ST: 51301[ns]  Info Successfully started components
   Hello World!
   Hello World!
   Hello World!
   Hello World!
   Hello World!
   [04.05.2021 - 14:22:12]: my_participant@participant  ST: 0[ns]  Info Successfully stopped components
   [04.05.2021 - 14:22:12]: my_participant@participant  ST: 0[ns]  Info Successfully stopped element
   [04.05.2021 - 14:22:12]: my_participant@participant  ST: 0[ns]  Info Successfully relaxed components
   [04.05.2021 - 14:22:12]: my_participant@participant  ST: 0[ns]  Info Successfully deinitialized components
   [04.05.2021 - 14:22:12]: my_participant@participant  ST: 0[ns]  Info Successfully deinitialized element

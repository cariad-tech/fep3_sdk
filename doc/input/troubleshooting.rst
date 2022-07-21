.. Copyright @ 2021 VW Group. All rights reserved.
.. 
..     This Source Code Form is subject to the terms of the Mozilla
..     Public License, v. 2.0. If a copy of the MPL was not distributed
..     with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
.. 
.. If it is not possible or desirable to put the notice in a particular file, then
.. You may include the notice in a location (such as a LICENSE file in a
.. relevant directory) where a recipient would be likely to look for such a notice.
.. 
.. You may add additional accurate notices of copyright ownership.


===============
Troubleshooting
===============

What to do in case of packet loss with the :ref:`RTI Connext DDS Simulation Bus`?
=================================================================================
The user of the FEP SDK may experience that the first few sent samples are lost in the beginning of the simulation.
To prevent such behavior the following should be taken into consideration:

* the switch to Running (after Initialized, see :ref:`label_participant_state_machine`) may have to be delayed by 1s or more, because the availability of the Data Writers takes some arbitrary time and are published asynchronously, but at the FEP SDK side "Init finished" can be returned earlier. This issue is addressed by FEPDEV-5242, but until it's resolved delay should be applied after init as workaround.

* In case a timing master is used, it is advised to be started at last. Otherwise the warning *\"Client clock is not started yet (participant didn't get start transition command), received master time event is blocked.\"* could be emitted. Also participants starting after the timing master may have their slave clocks synchronized later resulting in a clock time jump. (for more details on clock synchronization see :ref:`label_guide_timing`)

Executables loading plugins that use pthread.
=============================================
In Linux, when loading a plugin using pthread (for example when *std::thread* is used), *libpthread* is not automatically loaded.
This could be an issue leading to a *sigsev* in a participant executable when the plugin is loaded during startup.
An in depth discussion can be found in `this stack overflow thread <https://stackoverflow.com/questions/51209268/using-stdthread-in-a-library-loaded-with-dlopen-leads-to-a-sigsev>`_ and the `Bug report <https://gcc.gnu.org/bugzilla/show_bug.cgi?id=67791>`_ . If the used executable has already a pthread dependency due to another library this is not an issue. If this is not the case, the cmake macro *fep3_participant_link_pthread* is available, which can be used to link a target to pthread as follows:

.. code-block:: bash

   fep3_participant_link_pthread(YOUR_TARGET_NAME)

In order to find out if an executable is linked against to *libpthread* or a plugin has a *libpthread* dependency, the following bash command can be used

.. code-block:: bash

   ldd YOUR_EXECUTABLE_NAME


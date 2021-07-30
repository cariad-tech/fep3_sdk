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


===============
Troubleshooting
===============

What to do in case of packet loss with the :ref:`RTI Connext DDS Simulation Bus`?
=============================================================
The user of the FEP SDK may experience that the first few sent samples are lost in the beginning of the simulation.
To prevent such behavior the following should be taken into consideration:

* the switch to Running (after Initialized, see :ref:`label_participant_state_machine`) may have to be delayed by 1s or more, because the availability of the Data Writers takes some arbitrary time and are published asynchronously, but at the FEP SDK side "Init finished" can be returned earlier. This issue is addressed by FEPDEV-5242, but until it's resolved delay should be applied after init as workaround.

* because of the higher workload in the beginning of a simulation and the yet uninvestigated anomaly in timestamp handling (see FEPSDK-3089) the size of the data input queue of data jobs should be increased a little.

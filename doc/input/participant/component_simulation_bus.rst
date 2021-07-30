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


.. _label_simulation_bus:

==============
Simulation Bus
==============

.. sidebar:: Simulation Bus

        .. contents::


Summary
==========

+------------------------------------------------------+-----------------------------------------------------------------+
| Name                                                 |  Simulation Bus                                                 |
+------------------------------------------------------+-----------------------------------------------------------------+
| Component Interface                                  |  :cpp:class:`fep3::arya::ISimulationBus`                        |
+------------------------------------------------------+-----------------------------------------------------------------+
| native delivery                                      |  built-in for testing purposes,                                 |
|                                                      |  cpp-plugin RTI Connext DDS Simulation Bus                      |
+------------------------------------------------------+-----------------------------------------------------------------+
| cpp-plugin possible                                  |  yes                                                            |
+------------------------------------------------------+-----------------------------------------------------------------+
| c-plugin possible                                    |  yes                                                            |
+------------------------------------------------------+-----------------------------------------------------------------+

Overview
========

The delivered service bus as native :term:`FEP Component` will come with following implementations:

* :ref:`RTI Connext DDS Simulation Bus`


.. _RTI Connext DDS Simulation Bus:

RTI Connext DDS Simulation Bus
==============================

.. warning::
    Constraints:

    * The length of the system_name is limited to 255 characters.

QOS Profile
===========
The SimulationBus provides a file called USER_QOS_PROFILE.xml (see lib/rti/USER_QOS_PROFILES.xml). This file provides the QoS settings for each signal. For each natively provided StreamMetaType a matching QoS profile exists that the SimulationBus will use.
For example, the FEP3 StreamMetaType "plain_ctype_array" has a matching QoS profile called "fep3::plain_ctype_array" stored in the file.
That means you can define your own QOS profiles for a self-defined StreamType. For "my_own_stream_meta_type" create a "fep3::my_own_stream_meta_type" profile in your USER_QOS_PROFILES.xml.
If the SimulationBus does not find a matching profile for a StreamType, it will use the default profile "fep3::default_profile" instead.

Since RTI DDS relies on UDP, there is of course a limitation of the message size to ~63k bytes, messages bigger than ~63k bytes need to be fragmented.
In order to be able to fragment the messages, the QOS Profile must be set to asynchronous (see ASYNCHRONOUS_PUBLISHER in the RTI DDS documentation).
In order to do this we have defined an extra QOS Profile that ends with _big e.g. plain-array-ctype_big. In this profile we can set the qos of the asynchronous transmission.
So in the USER_QOS_PROFILE.xml is a normal QOS Profile for the StreamType plain-array-ctype and one for StreamTypes where the property max_size exceeds the limitation of ~63k bytes.

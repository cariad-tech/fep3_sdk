.. Copyright 2023 CARIAD SE.
.. 
.. This Source Code Form is subject to the terms of the Mozilla 
.. Public License, v. 2.0. If a copy of the MPL was not distributed 
.. with this file, You can obtain one at https://mozilla.org/MPL/2.0/.


.. _label_simulation_bus:

==============
Simulation Bus
==============

.. sidebar:: Simulation Bus

        .. contents::


Summary
=======

+------------------------------------------------------+-----------------------------------------------------------------+
| Name                                                 |  Simulation Bus                                                 |
+------------------------------------------------------+-----------------------------------------------------------------+
| Component Interface                                  |  :cpp:class:`fep3::arya::ISimulationBus`                        |
+------------------------------------------------------+-----------------------------------------------------------------+
| native delivery                                      |  CPP-plugin RTI Connext DDS Simulation Bus                      |
+------------------------------------------------------+-----------------------------------------------------------------+
| CPP-plugin possible                                  |  yes                                                            |
+------------------------------------------------------+-----------------------------------------------------------------+
| C-plugin possible                                    |  yes                                                            |
+------------------------------------------------------+-----------------------------------------------------------------+

Overview
========

The Component Simulation Bus is responsible for data communication between FEP Participants.

The delivered Simulation Bus as native :term:`FEP Component` will come with following implementations:

* :ref:`RTI Connext DDS Simulation Bus`


.. _RTI Connext DDS Simulation Bus:

RTI Connext DDS Simulation Bus
==============================

The RTI Connext DDS Simulation Bus comes with diverse features for exchanging data, and can be configured in several ways.
A thorough explanation can be found in the dedicated chapter :ref:`Configuring Data Exchange`.

.. warning::
    Constraints:

    * The length of the *system_name* is limited to 255 characters.
    * Some calls to :ref:`RTI Connext DDS Simulation Bus` library are not thread safe (for example `the DomainParticipant creation <https://community.rti.com/static/documentation/connext-dds/6.0.1/doc/api/connext_dds/api_cpp2/classdds_1_1domain_1_1DomainParticipant.html#adb0fdf101ecfe041b6472e67b443df6b>`_ ). This should be taken into account in case there are multiple participants running within the same process.

.. _QoS Profile:

QoS Profile
===========

The Simulation Bus provides a file called USER_QOS_PROFILE.xml (see lib/rti/USER_QOS_PROFILES.xml). This file provides the QoS settings for each signal. For each natively :ref:`supported StreamMetaType<Supported Stream Meta Type>` a matching QoS profile exists that the Simulation Bus will use. 

For example, the FEP3 *StreamMetaType* "plain_ctype_array" has a matching QoS profile called "fep3::plain_ctype_array" stored in the file.
This means custom defined QOS profiles may be used for self-defined *StreamTypes*. For "my_own_stream_meta_type" create a "fep3::my_own_stream_meta_type" profile in the USER_QOS_PROFILES.xml.
If the Simulation Bus does not find a matching profile for a *StreamType*, it will use the default profile "fep3::default_profile" instead.

Since RTI DDS relies on UDP, there is of course a limitation of the message size to ~64k bytes, messages bigger than ~64k bytes need to be fragmented.
In order to be able to fragment the messages, the QOS Profile must be set to asynchronous (see ASYNCHRONOUS_PUBLISHER in the RTI DDS documentation).
In order to do this, we have defined an extra QOS Profile that ends with _big e.g. 'plain-array-ctype_big'. In this profile we can set the QOS of the asynchronous transmission.
So, in the USER_QOS_PROFILE.xml is a normal QOS Profile for the *StreamType* plain-array-ctype and one for Stream Types where the property max_size exceeds the size limitation.
To take into account the message size overhead RTI DDS brings along, FEP uses 63k bytes as threshold for message sizes and uses the big QOS profiles if the threshold is reached or exceeded.
The same mechanism applies to stream types. If the size of a stream type is smaller than 63k bytes, the stream type is transmitted using QOS profile 'stream_type'.
The size of a stream type is defined by its properties. Stream types of size >= 63k bytes are transmitted using QOS profile 'stream_type_big'.
For further information regarding QOS configuration, please have a look at :ref:`label_configuring_qos`.

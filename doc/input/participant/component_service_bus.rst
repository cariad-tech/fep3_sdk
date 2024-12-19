.. Copyright 2023 CARIAD SE.
.. 
.. This Source Code Form is subject to the terms of the Mozilla 
.. Public License, v. 2.0. If a copy of the MPL was not distributed 
.. with this file, You can obtain one at https://mozilla.org/MPL/2.0/.


.. _label_service_bus:

===========
Service Bus
===========

.. sidebar:: Service Bus

        .. contents::


Summary
=======

+------------------------------------------------------+-----------------------------------------------------------------+
| Name                                                 |  Service Bus                                                    |
+------------------------------------------------------+-----------------------------------------------------------------+
| Component Interface                                  |  :cpp:class:`fep3::arya::IServiceBus`                  |br|     |
|                                                      |  :cpp:class:`fep3::catelyn::IServiceBus`               |br|     |
+------------------------------------------------------+-----------------------------------------------------------------+
| RPC Service Interface                                |  :cpp:class:`fep3::rpc::arya::IRPCParticipantInfoDef`   |br|    |
|                                                      |  :cpp:class:`fep3::rpc::arya::IRPCParticipantInfo`      |br|    |
|                                                      |  :cpp:class:`fep3::rpc::catelyn::IRPCHttpServerDef`     |br|    |
|                                                      |  :cpp:class:`fep3::rpc::catelyn::IRPCHttpServer`        |br|    |
+------------------------------------------------------+-----------------------------------------------------------------+
| RPC Service Description                              |  participant_info.json                                          |
+------------------------------------------------------+-----------------------------------------------------------------+
| native delivery                                      |  CPP-plugin for HTTP,                |br|                       |
|                                                      |  CPP-plugin for RTI                  |br|                       |
+------------------------------------------------------+-----------------------------------------------------------------+
| CPP-plugin possible                                  |  yes                                                            |
+------------------------------------------------------+-----------------------------------------------------------------+
| C-plugin possible                                    |  no                                                             |
+------------------------------------------------------+-----------------------------------------------------------------+

Overview
========

A :term:`FEP Participant` is a network node within a :term:`FEP System`.
The participant is the "gate" to connect to the defined network.
So, one main part of a participant is called :term:`FEP Service Bus`. The defined network is called "service network".

From a participant's point of view the :term:`FEP Service Bus` has the following important features:

- provide one service access point for any control remote procedure call to the participant from the network world
- provide the possibility to response to a request from system perspective
- provide a service interface which may be used from the service network world

The FEP System is formed by a set of FEP Participants.
The smallest FEP System contains *one* FEP Participant.

From a system's point of view the :term:`FEP Service Bus` has the following important features:

- network discovery to discover current participants which are connected to the same service network
- provide access to the participants services to control it by remote procedure calls
- provide the possibility to request any public functionality within the participant that are provided by a service
- connect to a defined set of participants which may be part of the FEP System

.. image:: images/section_fep_service_bus_1.png

FEP Participant components and the FEP Service Bus
--------------------------------------------------

The participant's functionality is separated into clearly defined services. These services are called `FEP Components`.
Each component is able to use the service bus (which is also provided as a component) to
provide some service interfaces for its specific task.

Examples:
`````````

.. image:: images/section_fep_participant_components_design.png

:Logging Service Component: The task for the Logging Service Component is to provide a possibility to log information,
                            error descriptions or warnings to a single service.
                            The service interface of this component is to send logging information to other service bus attendees.
                            To do so, the Logging Service Component will i.e. provide a possibility for a
                            remote procedure call method like ``getLogMessages()``.
:Configuration Component: The task for the Configuration Component is to provide a possibility to set and retrieve the values of properties.
                          These properties will be registered with a name and might be a parameter for any other functionality.
                          The service interface of this service will provide some remote procedure calls like ``getProperty(name)``
                          and ``setProperty(name, value)``.

.. tip::
   Each component has a participant internal interface to program the participants functionality and a participant service interface
   to provide a remote procedure access via service bus.


System Access, Participant Server, Participant Requester
--------------------------------------------------------

The system access is a single communication access point to the service bus at a dedicated system and network.
Each system access may register a participant server at the systems network URL.
Usually, this system URL is used to discover other participants using the same protocol.

Other participants can be only discovered and appear in the system, if a Participant Server was created.
This server will provide the registered service on the dedicated server URL.

To use the service of a participant from systems point of view,
the address of this far Participant Server has to be known.
Otherwise it will not be possible to communicate with it.
It depends on the protocol used by the requester implementation
if this address might be a dedicated URL address as it is within a ``http`` implementation
or a single communication channel on a `dds` service bus.

Example
```````
.. image:: images/service_bus_system_access_discover.png

:Discover Addresses via System Access: Discoverable participants on the whole system network address http://239.255.255.250:190 are:
                                       | "Participant1" with the URL http://host1:9090 for System1
                                       | "Participant2" with http://host1:9091 for System1
                                       | "Participant3" with the URL http://host2:9090 for System2
                                       | "Participant4" with http://host2:9091 for System2
                                       | SystemView1 and SystemView2 did not register a server to the network, so they are not seen and are not discoverable.
:System1 with Participant Servers: Discoverable participants on "System1" are:
                                   | "Participant1" with the URL http://host1:9090
                                   | "Participant2" with http://host1:9091
                                   | SystemView1 did not register a server to the network, so they it is not seen and is not discoverable.
:System2 with Participant Servers: Discoverable participants on "System2" are:
                                   | "Participant3" with the URL http://host2:9090
                                   | "Participant4" with http://host2:9091
                                   | SystemView2 did not register a server to the network, so they it is not seen and is not discoverable.

.. _label_service_bus

Service Bus
===========

The delivered service bus as native :term:`FEP Component` will come with following implementations:

* :ref:`HTTP Service Bus`
* :ref:`RTI Service Bus`


.. _HTTP Service Bus:

HTTP Service Bus
----------------

.. _HTTP Server:

HTTP Server and Service Registry
````````````````````````````````

The HTTP Server is an implementation of a real HTTP Server which will react on *HTTP REQUEST* messages.
This implementation uses the Library of the :term:`Package RPC` .

The server itself must be initialized for a valid location address. Examples for valid addresses are:

* *http://localhost:9090* - this will open a socket on the network device of the "localhost" and port 9090.
* *http://0.0.0.0:9090* - this will open a socket on every network device and port 9090. So, it will be available on "localhost" and each of the network addresses.
* *http://localhost:0* - this will open a socket on the network device of the "localhost" and will find a free port starting at 9090.
* *http://0.0.0.0:0* - this will open a socket on every network device and will find a free port starting at 9090.

The dynamic mechanism of finding a free port will make only sense if the dynamic discovery of the :ref:`HTTP System Access` is used.
Otherwise it is not possible to discover and obtain the port the other participant opened while initializing.

The Service Registry implementation will forward HTTP REQUESTS to the objects registered by name:

* If a :term:`RPC Service` is registered with the name *clock_master* the HTTP server will forward every HTTP REQUEST to it with the address i.e. *http://localhost:9090/clock_master*.

.. _label_service_bus_http_system_access:

.. _HTTP System Access:

HTTP System Access and SSDP Discovery
`````````````````````````````````````

The HTTP System Access will provide the possibility to discover all other servers within the same network and with the same system name by implementing a :term:`Participant Discovery` protocol.
The system access must be created via a valid **multicast address** and a port. The default address is:

* *http://230.230.230.1:9990*

Each server, somewhere in the network, using the same address will be discovered if the firewall ruleset does not prevent that.
Each :ref:`HTTP Server` will send :term:`Discovery Messages` containing its name and a system name. Both are provided with :cpp:func:`fep3::catelyn::IServiceBus::ISystemAccess::createServer`.
For usage within the :term:`FEP Participant` this will be the name of the participant and the system name which are both provided to :cpp:func:`~fep3::core::createParticipant`.

This mechanism is using the :term:`SSDP` from the UPnP standard v1.1.
This implementation will follow chapter 1 of the specification
http://www.upnp.org/specs/arch/UPnP-arch-DeviceArchitecture-v1.1.pdf, but is no fully UPnP 1.1 implementation.


In case of disabled discovery, each :term:`RPC Requester` has to be created using a full address.
A call of :cpp:func:`~fep3::IServiceBus::getRequester` must use i.e. *http://other_interface:9097* and
cannot be used by its alias name it was created within this system (see :cpp:func:`fep3::catelyn::IServiceBus::ISystemAccess::createServer`).

Native Service Bus Delivery
```````````````````````````

The HTTP Service Bus is delivered as :term:`CPP Plugin` component so it will automatically be created. Also it is delivered as a separate :term:`CPP Plugin` ( *lib/http/fep3_http_service_bus.dll* / *lib/http/fep3_http_service_bus.so*).

.. _RTI Service Bus:

RTI Service Bus
---------------

An alternative implementation to :ref:`Native Service Bus <HTTP System Access>` is implemented in the *fep3_dds_service_bus_plugin* using RTI DDS. The difference between the  :ref:`RTI Service Bus` and the :ref:`Native Service Bus <HTTP System Access>`
plugins is the mechanism used for :term:`Participant Discovery`. :ref:`RTI Service Bus` uses a keyed RTI DDS topic for exchanging the discovery information, making discovery possible in networks with multicast disabled.
In case FEP is deployed in networks with multicast enabled, no additional settings are required. In case multicast is deactivated, there are two options so that participants in different machines can be discovered:

.. _label_adapt_peers_list_for_unicast:

1. Use a `initial peers list file <https://community.rti.com/static/documentation/connext-dds/6.1.0/doc/manuals/connext_dds_professional/users_manual/index.htm#users_manual/ConfigPeersListUsed_inDiscov.htm?Highlight=known%20peers>`_
that should be located in the same path as the participant executable (and **not** the path where the plugin is located). The file must contain in a comma seperated format the IPs or the host names of the machines running the participants.
The content of the peers list file could look like this:

::

    172.18.0.3, 172.18.0.2


2. Use the `RTI Cloud discovery service <https://community.rti.com/static/documentation/connext-dds/6.1.0/doc/manuals/connext_dds_professional/users_manual/index.htm#users_manual/P2P_Behind_Cone.htm>`_
this solution requires one cloud discovery service instance running on one machine, and a NDDS_DISCOVERY_PEERS file (should be located in the same directory as the participant executable and where the actual plugin is located)
pointing to the IP and port of the running discovery service. The content of the peers list file could look like this:

::

    rtps@172.18.0.5:7400

.. _Load Service Discovery Plugin:

Loading the RTI Service Bus plugin
``````````````````````````````````

As standard, the :ref:`RTI Service Bus` plugin will be loaded, but the :ref:`Native Service Bus <HTTP System Access>` plugin can be loaded by changing the *.components* files of **ALL** Participants **AND** the service library.

Load the plugin in a FEP Participant
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In the *fep3_participant.fep_components* and *fep3_system.fep_components* files replace

.. code-block:: xml

    <component>
        <source type="cpp-plugin">
            ./fep_components_plugin
        </source>
        <iid>service_bus.arya.fep3.iid</iid>
    </component>

with

.. code-block:: xml

    <component>
        <source type="cpp-plugin">
            rti/fep3_dds_service_bus_plugin
        </source>
        <iid>service_bus.arya.fep3.iid</iid>
    </component>

.. note::
    Make sure that all the participants in a FEP System, as well the FEP System library use the same type of Service Bus plugin. Mixing :ref:`HTTP System Access` and :ref:`RTI Service Bus` plugins is not supported
    and will result to errors and/or not proper functionality of the FEP System.

.. _label_notes_using_rti_dds_service_bus:

RTI DDS Service BUS Discovery
`````````````````````````````
Similar to the :ref:`Discovery Protocol <HTTP System Access>` used by the :ref:`HTTP Service Bus` for :term:`Participant Discovery`, the :ref:`RTI Service Bus` implements the :term:`Participant Discovery` using RTI DDS signals.
Each :ref:`HTTP Server <HTTP Server>` will send :term:`Discovery Messages` in form of RTI DDS signals containing its name, system name, host name and the listening port of the :ref:`HTTP Server <HTTP Server>`.
Name and system name are provided with :cpp:func:`fep3::catelyn::IServiceBus::ISystemAccess::createServer`.
For usage within the :term:`FEP Participant` this will be the name of the participant and the system name which are both provided to :cpp:func:`~fep3::core::createParticipant`.


Notes when using the RTI DDS Service BUS
````````````````````````````````````````

* Do not put a QOS file in the same directory as the participant executable. Manipulation of the QOS for the Topics used in :term:`Participant Discovery` can result in malfunction. The QOS file of the :ref:`RTI Connext DDS Simulation Bus` is located
  under lib/rti/USER_QOS_PROFILES.xml.
* The DDS Service Bus uses a separate DDS Domain participant from :ref:`RTI Connext DDS Simulation Bus`. The default domain ID used for the :term:`Participant Discovery` is **0**. The Domain ID of the :term:`Participant Discovery` can be changed by setting
  the environment variable *FEP3_SERVICE_BUS_DOMAIN_ID*.

.. note::
    Make sure that all participants in a FEP System use the same DDS Domain ID for the :term:`Participant Discovery`.

* The DDS topic name used for discovery is named *service_discovery* and should not be used for any other signal name in case the default domain ID **0** is used apart from the RTI DDS Service Bus.

* In case the multicast in your system is deactivated, make sure the :ref:`peers list is adapted <label_adapt_peers_list_for_unicast>`. Also when multicast is deactivated, depending on the number of participants (and system library instances) used, you may have to adapt the DDS Participant ID limited as described in this `rti dds article <https://community.rti.com/kb/why-cant-more-5-domainparticipants-communicate-within-same-machine>`_ . In case you face problems discovering participants with multicast deactivated, this is the most probable culprit.

.. _label_debugging_rti_dds_service_bus:

Debugging the Service Bus
-------------------------
The :ref:`RTI Service Bus`, logs additional debug messages that can be activated by  :ref:`setting the severity  <label_changing_the_severity_level_filter>` of the service bus logger to debug (5). When you have the logs activated, you should see the processed update events incoming from all the participants. In case no update events are received from one or more participants, it means that there are no DDS Signals received from these participants.

.. _access_service_bus_discovery_messages:

Accessing the participant :term:`discovery messages`
----------------------------------------------------
As discussed previously in the chapter, both :ref:`HTTP Service Bus` and the :ref:`RTI Service Bus` use a discovery mechanism to find the participants within a FEP system.
The received :term:`Discovery Messages` can be forwarded to a Sink using :cpp:func:`fep3::catelyn::IServiceBus::ISystemAccess::registerUpdateEventSink` 
and :cpp:func:`fep3::catelyn::IServiceBus::ISystemAccess::deregisterUpdateEventSink`. The callbacks are executed sequentially in an independent thread.


Interface Changes
=================

+------------------------------------------------------+----------------------------------------------------------------------------------------+
| Namespace                                            |  Changes                                                                               |
+------------------------------------------------------+----------------------------------------------------------------------------------------+
| catelyn                                              |  :cpp:class:`fep3::catelyn::IServiceBus`                                          |br| |
|                                                      |  :cpp:class:`fep3::catelyn::IServiceBus::getSystemAccessCatelyn`                  |br| |
|                                                      |  :cpp:enum:`fep3::catelyn::IServiceBus::ServiceUpdateEventType`                   |br| |
|                                                      |  :cpp:struct:`fep3::catelyn::IServiceBus::ServiceUpdateEvent`                     |br| |
|                                                      |  :cpp:class:`fep3::catelyn::IServiceBus::IServiceUpdateEventSink`                 |br| |
|                                                      |  :cpp:func:`fep3::catelyn::IServiceBus::IServiceUpdateEventSink::updateEvent`     |br| |
|                                                      |  :cpp:class:`fep3::catelyn::IServiceBus::ISystemAccess`                           |br| |
|                                                      |  :cpp:func:`fep3::catelyn::IServiceBus::ISystemAccess::registerUpdateEventSink`   |br| |
|                                                      |  :cpp:func:`fep3::catelyn::IServiceBus::ISystemAccess::deregisterUpdateEventSink` |br| |
+------------------------------------------------------+----------------------------------------------------------------------------------------+

.. |br| raw:: html

     <br>

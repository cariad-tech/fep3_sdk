.. Copyright 2023 CARIAD SE.
.. 
.. This Source Code Form is subject to the terms of the Mozilla 
.. Public License, v. 2.0. If a copy of the MPL was not distributed 
.. with this file, You can obtain one at https://mozilla.org/MPL/2.0/.


.. _label_clock_service:

=============
Clock Service
=============

.. sidebar::  Clock Service

        .. contents::

Summary
=======

+------------------------------------------------------+-----------------------------------------------------------------------------+
| Name                                                 |  Clock Service                                                              |
+------------------------------------------------------+-----------------------------------------------------------------------------+
| Component Interface                                  |  :cpp:class:`fep3::arya::IClockService`                                     |
+------------------------------------------------------+-----------------------------------------------------------------------------+
| RPC Service Interface                                |  :cpp:class:`fep3::rpc::arya::IRPCClockServiceDef`                          |
|                                                      |  :cpp:class:`fep3::rpc::arya::IRPCClockSyncMasterDef`                       |
+------------------------------------------------------+-----------------------------------------------------------------------------+
| RPC Service Description                              | :ref:`label_rpc_service_clock`                                              |
|                                                      | :ref:`label_rpc_service_clock_sync_master`                                  |
+------------------------------------------------------+-----------------------------------------------------------------------------+
| native delivery                                      |  CPP-plugin                                                                 |
+------------------------------------------------------+-----------------------------------------------------------------------------+
| CPP-plugin possible                                  |  yes                                                                        |
+------------------------------------------------------+-----------------------------------------------------------------------------+
| C-plugin possible                                    |  yes                                                                        |
+------------------------------------------------------+-----------------------------------------------------------------------------+

Overview
========

The :ref:`label_clock_service` is responsible for managing one or more different clocks which may represent different times with a different time base.
Every registered clock is identified by a unique name.
There is one so called :ref:`label_clock_service_main_clock`, which provides the time of the :ref:`label_clock_service`.
As described in the section :ref:`label_clock_type` , clocks are distinguished by a type.

The time of the :ref:`label_clock_service` represents the simulation time of the participant.


Component Interface
===================

The :cpp:class:`fep3::arya::IClockService` offers the following functionality:

* Register and unregister clocks and retrieve information about all registered clocks including the :ref:`label_clock_service_main_clock`.
* Register and unregister event sinks (:cpp:class:`fep3::arya::IClock::IEventSink`).
* Get the current time or the type of the :ref:`label_clock_service` or of any other registered clock.


.. _label_clock_service_event_sink:

Event Sink
----------

An :ref:`label_clock_service_event_sink` allows synchronous reactions on time reset and time update events. An :ref:`label_clock_service_event_sink` is created by implementing the abstract methods of (:cpp:class:`fep3::arya::IClock::IEventSink`).
It can be registered using :cpp:func:`fep3::arya::IClockService::registerEventSink`. After registration the :ref:`label_clock_service_event_sink` will be notified whenever the :ref:`label_clock_service_main_clock`
emits an event.


RPC Service Interfaces
======================

RPC Clock Service
-----------------

The :cpp:class:`fep3::rpc::arya::IRPCClockServiceDef` offers the following functionality:

* Retrieve information about all registered clocks.
* Get the current time of the :ref:`label_clock_service` or of any other registered clock.



.. _label_clock_sync_master:

RPC Clock Sync Master
---------------------

The :ref:`label_clock_service` provides a :ref:`label_clock_sync_master` which is the counterpart of the :ref:`label_clock_sync_service` and enables the synchronization of a timing master with timing slaves.
The :ref:`label_clock_sync_master` offers the possibility to register as a synchronization slave as well as to retrieve the current time of the master and its type (:cpp:class:`fep3::rpc::arya::IRPCClockSyncMasterDef` for detail).
Also, it can actively synchronize timing slaves. The communication is done via RPC.
To establish a clock synchronization the :ref:`label_clock_sync_service` has to be used on the slave side.
If discrete synchronization is used (e.g. :ref:`label_clock_sync_slave_master_on_demand_discrete`), all timing slaves are synchronized concurrently. The master will wait until all slaves are successfully synchronized or a timeout occurred.
The timeout is configured via :c:macro:`FEP3_CLOCK_SERVICE_TIME_UPDATE_TIMEOUT`. If the timeout is exceeded the timing master will log an error, stop to wait for the blocking timing slave and continue with its distribution of time update events.
The relevant timing slave will still receive time update events in this case.
If on the other hand the timing master receives an invalid rpc response from a timing slave, the timing master will log an error and remove the corresponding timing slave from synchronization but simulation will be continued.

Properties
^^^^^^^^^^

The :ref:`label_clock_sync_master` can be configured using the following properties.

.. list-table::
   :header-rows: 1
   :widths: 20 60 20 60

   * - Name
     - Code Macro
     - Default Value
     - Restrictions
   * - TimeUpdateTimeout
     - :c:macro:`FEP3_CLOCK_SERVICE_TIME_UPDATE_TIMEOUT`
     - :c:macro:`FEP3_TIME_UPDATE_TIMEOUT_DEFAULT_VALUE`
     - Minimum :c:macro:`FEP3_TIME_UPDATE_TIMEOUT_MIN_VALUE`



.. _label_clock_service_main_clock:

Main Clock
==========

The :ref:`label_clock_service_main_clock` is the clock providing the current time for a :term:`FEP Participant`. Calling :cpp:func:`fep3::arya::IClockService::getTime` will return the time of that clock.

If the current :ref:`label_clock_service_main_clock` is being unregistered, the Clock Service emits a warning level log and sets :ref:`label_clock_implementation_local_system_realtime` as :ref:`label_clock_service_main_clock`.


.. _label_clock_type:

Clock Type
==========

Clocks can either be of type :ref:`label_clock_type_discrete` or :ref:`label_clock_type_continuous`. In the API the Enum :cpp:enum:`fep3::arya::ClockType` is used to distinguish the types of clocks.


.. _label_clock_type_discrete:

Discrete
--------

A discrete clock actively sets a timestamp and synchronously propagates an internal updating event to all registered :ref:`label_clock_service_event_sink` s.
If other slave participants use the :ref:`label_clock_sync_service` the time update event will be propagated via RPC.
See :ref:`label_clock_implementation_local_system_simtime` for an implementation.


.. _label_clock_type_continuous:

Continuous
----------

A continuous clock will provide real time (or simulated real time) on each :cpp:func:`fep3::arya::IClockService::getTime` call.
See :ref:`label_clock_implementation_local_system_realtime` for an implementation.


Configuration
=============

The :ref:`label_clock_service` can be configured using the following property.
Depending on the specific clock configured as :ref:`label_clock_service_main_clock`, there might be additional configuration options.
For details about the native clocks, see the documentation of the :ref:`label_clock_service_native`.

If the participant you are configuring is a timing slave you have to set the :ref:`label_clock_service_main_clock` to one of the slave clocks, described in the section of the :ref:`label_clock_sync_service`.


Properties
----------


.. list-table::
   :widths: 20 60 20
   :header-rows: 1

   * - Name
     - Code Macro
     - Default Value
   * - MainClock
     - :c:macro:`FEP3_CLOCK_SERVICE_MAIN_CLOCK`
     - :c:macro:`FEP3_CLOCK_LOCAL_SYSTEM_REAL_TIME`


.. _label_clock_service_native:

Native Implementations
======================

The following native implementations are available

.. list-table::
   :header-rows: 1

   * - Name
     - Code Macro
     - Type
   * - :ref:`label_clock_implementation_local_system_simtime`
     - :c:macro:`FEP3_CLOCK_LOCAL_SYSTEM_SIM_TIME`
     - :ref:`label_clock_type_discrete`
   * - :ref:`label_clock_implementation_local_system_realtime`
     - :c:macro:`FEP3_CLOCK_LOCAL_SYSTEM_REAL_TIME`
     - :ref:`label_clock_type_continuous`


.. _label_clock_implementation_local_system_simtime:

local_system_simtime
--------------------

This clock will drive the time in logical time steps in resolution of the property :c:macro:`FEP3_CLOCK_SERVICE_CLOCK_SIM_TIME_STEP_SIZE`.
If for example the property is set to the value *5000000 ns* the clock will set the time steps *0 ns, 5000000 ns, 10000000 ns* and so on.
The clock will try to set the time steps according to the clock of the operating system. So the difference between the step at time *0* and *5000000* should actually be 5000000 ns.

As the clock :ref:`label_clock_implementation_local_system_simtime` is a discrete clock, for every new time step all :ref:`label_clock_service_event_sink` s will be called synchronously.
If the processing of all :ref:`label_clock_service_event_sink` s takes longer than the :c:macro:`FEP3_CLOCK_SERVICE_CLOCK_SIM_TIME_STEP_SIZE` the clock will run slower than the wall clock.
That would for example be the case if a Job with a *cycle_time* of 5000000 ns would take 8000000 ns to compute.

Also the speed of the time simulation can be adjusted using the factor :c:macro:`FEP3_CLOCK_SERVICE_CLOCK_SIM_TIME_TIME_FACTOR`.
Setting it to :c:macro:`FEP3_CLOCK_SIM_TIME_TIME_FACTOR_AFAP_VALUE` will lead to a simulation running as fast as possible.

The clock's real wall clock step size, which it tries to follow, is determined by dividing :c:macro:`FEP3_CLOCK_SERVICE_CLOCK_SIM_TIME_STEP_SIZE` by :c:macro:`FEP3_CLOCK_SERVICE_CLOCK_SIM_TIME_TIME_FACTOR`
and may not fall below :c:macro:`FEP3_CLOCK_WALL_CLOCK_MIN_STEP_SIZE`.


Properties
^^^^^^^^^^

The clock :ref:`label_clock_implementation_local_system_simtime` can be configured using the following properties.

.. list-table::
   :widths: 20 60 20 60
   :header-rows: 1

   * - Name
     - Code Macro
     - Default Value
     - Restrictions
   * - CycleTime
     - :c:macro:`FEP3_CLOCK_SERVICE_CLOCK_SIM_TIME_STEP_SIZE`
     - :c:macro:`FEP3_CLOCK_SIM_TIME_STEP_SIZE_DEFAULT_VALUE`
     - Minimum: :c:macro:`FEP3_CLOCK_SIM_TIME_STEP_SIZE_MIN_VALUE`, Maximum: :c:macro:`FEP3_CLOCK_SIM_TIME_STEP_SIZE_MAX_VALUE`
   * - TimeFactor_float
     - :c:macro:`FEP3_CLOCK_SERVICE_CLOCK_SIM_TIME_TIME_FACTOR`
     - :c:macro:`FEP3_CLOCK_SIM_TIME_TIME_FACTOR_DEFAULT_VALUE`
     - Minimum: :c:macro:`FEP3_CLOCK_SIM_TIME_TIME_FACTOR_AFAP_VALUE`


.. _label_clock_implementation_local_system_realtime:

local_system_realtime
---------------------

The :ref:`label_clock_implementation_local_system_realtime` clock uses the clock of the operating system.
It will return the tick count of the current time starting at 0 (where usually 0 is the time stamp of starting the computer).

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

.. _label_guide_timing_use_cases:

================
Timing Use Cases
================

.. sidebar::  Timing Use Cases

        .. contents::


The following section lists various timing and scheduling use cases and the corresponding configurations.

.. _label_guide_timing_use_cases_no_synchronization:

No time synchronization
=======================

Use Cases:
----------

* Development of new participants.
* Non time dependent functional testing of participants.

Configuration:
--------------

.. list-table::
   :header-rows: 1

   * -
     - Participant 1
     - Participant 2
     - Participant 3
   * - :ref:`label_clock_service_main_clock`
     - :ref:`label_clock_implementation_local_system_realtime`
     - :ref:`label_clock_implementation_local_system_realtime`
     - :ref:`label_clock_implementation_local_system_realtime`
   * - :ref:`label_scheduler_service_active_scheduler`
     - :ref:`label_clock_based_scheduler`
     - :ref:`label_clock_based_scheduler`
     - :ref:`label_clock_based_scheduler`


This configuration means:
^^^^^^^^^^^^^^^^^^^^^^^^^

* No time synchronization is used.
* The time of every participant will continue by its system clock independent from any other participant.
* The :ref:`label_scheduler_service_active_scheduler` of a participant will continuously look at the time and will execute the registered Jobs at the corresponding point in time.
* The :ref:`label_scheduler_service_active_scheduler` will watch runtime execution times and will react to violation according to the configured strategy (see :ref:`label_clock_based_scheduler`).


.. _label_guide_timing_use_cases_continuous_synchronization:

Master/Slave continuous synchronization
=======================================

Native continuous real-time
---------------------------

Use Cases:
^^^^^^^^^^

* Realtime mode within a realtime environment like vehicle prototypes or other distributed systems.
* No fixed distributed scheduling.
* Data driven systems.

Configuration:
^^^^^^^^^^^^^^

.. list-table::
   :header-rows: 1

   * -
     - Master Participant
     - Participant 2
     - Participant 3
   * - :ref:`label_clock_service_main_clock`
     - :ref:`label_clock_implementation_local_system_realtime`
     - :ref:`label_clock_sync_slave_master_on_demand`
     - :ref:`label_clock_sync_slave_master_on_demand`
   * - :ref:`label_scheduler_service_active_scheduler`
     - :ref:`label_clock_based_scheduler`
     - :ref:`label_clock_based_scheduler`
     - :ref:`label_clock_based_scheduler`
   * - :c:macro:`FEP3_CLOCKSYNC_SERVICE_CONFIG_SLAVE_SYNC_CYCLE_TIME`
     -
     - 100 ms
     - 100 ms
   * - :c:macro:`FEP3_CLOCKSYNC_SERVICE_CONFIG_TIMING_MASTER`
     -
     - "Master Participant"
     - "Master Participant"

This configuration means:
^^^^^^^^^^^^^^^^^^^^^^^^^^

* *Participant 2* and *Participant 3* are timing slaves, the :ref:`label_clock_sync_slave_master_on_demand` clocks will synchronize every 100 ms with the *Master Participant*.
* The slave times are straightened by the Cristian's Algorithm using the round trip time of getting the time from the master.
* The :ref:`label_clock_sync_slave_master_on_demand` clock will interpolate the time between the 100 ms synchronization steps.
* The *Master Participant* will provide the time by a continuous local system real time.
* The :ref:`label_scheduler_service_active_scheduler` will continuously look at the time and will execute the configured Jobs at the corresponding point in time.
* The :ref:`label_scheduler_service_active_scheduler` will watch runtime execution times and will react to violation according to the configured strategy (see :ref:`label_clock_based_scheduler`).

External real-time clock
---------------------------------

Use Cases:
^^^^^^^^^^^^^^^^

* Realtime mode within a realtime environment like vehicle prototypes or other distributed systems.
* No fixed distributed scheduling.
* The time source clock *some_fancy_realtime_clock* is provided by an external implementation.

Configuration:
^^^^^^^^^^^^^^^^

.. list-table::
   :header-rows: 1

   * -
     - Master Participant
     - Participant 2
     - Participant 3
   * - :ref:`label_clock_service_main_clock`
     - some_fancy_realtime_clock
     - :ref:`label_clock_sync_slave_master_on_demand`
     - :ref:`label_clock_sync_slave_master_on_demand`
   * - :ref:`label_scheduler_service_active_scheduler`
     - :ref:`label_clock_based_scheduler`
     - :ref:`label_clock_based_scheduler`
     - :ref:`label_clock_based_scheduler`
   * - :c:macro:`FEP3_CLOCKSYNC_SERVICE_CONFIG_SLAVE_SYNC_CYCLE_TIME`
     -
     - 100 ms
     - 100 ms
   * - :c:macro:`FEP3_CLOCKSYNC_SERVICE_CONFIG_TIMING_MASTER`
     -
     - "Master Participant"
     - "Master Participant"



This configuration means:
^^^^^^^^^^^^^^^^^^^^^^^^^^

* *Participant 2* and *Participant 3* are timing slaves, the :ref:`label_clock_sync_slave_master_on_demand` clock will synchronize every 100 ms with the *Master Participant*.
* The slave times are straightened by the Cristian's Algorithm using the round trip time of getting the time from the master.
* The :ref:`label_clock_sync_slave_master_on_demand` clock will interpolate the time between the 100 ms synchronization steps.
* The *Master Participant* will provide the time by a continuous external time called *some_fancy_realtime_clock*.
* The :ref:`label_scheduler_service_active_scheduler` will continuously look at the time and will execute the configured Jobs at the corresponding point in time.
* The :ref:`label_scheduler_service_active_scheduler` will watch runtime execution times and will react to violation according to the configured strategy (see :ref:`label_clock_based_scheduler`).


.. _label_guide_timing_use_cases_discrete_synchronization:

Master/Slave discrete synchronization
=====================================

Native discrete synchronization (fixed time steps)
--------------------------------------------------

Use Cases:
^^^^^^^^^^

* Simulated deterministic test environment
* Fixed distributed scheduling with discrete logical time (usually no realtime!)

Configuration:
^^^^^^^^^^^^^^

.. list-table::
   :header-rows: 1

   * -
     - Master Participant
     - Participant 2
     - Participant 3
   * - :ref:`label_clock_service_main_clock`
     - :ref:`label_clock_implementation_local_system_simtime`
     - :ref:`label_clock_sync_slave_master_on_demand_discrete`
     - :ref:`label_clock_sync_slave_master_on_demand_discrete`
   * - :ref:`label_scheduler_service_active_scheduler`
     - :ref:`label_clock_based_scheduler`
     - :ref:`label_clock_based_scheduler`
     - :ref:`label_clock_based_scheduler`
   * - :c:macro:`FEP3_CLOCK_SERVICE_CLOCK_SIM_TIME_STEP_SIZE`
     -
     - 100 ms
     - 100 ms
   * - :c:macro:`FEP3_CLOCKSYNC_SERVICE_CONFIG_TIMING_MASTER`
     -
     - "Master Participant"
     - "Master Participant"



This configuration means:
^^^^^^^^^^^^^^^^^^^^^^^^^

* Time synchronization is initiated by registering to the :ref:`label_clock_service` of the timing master.
* Each :ref:`label_clock_sync_slave_master_on_demand_discrete` clock will receive time update events from the configured master.
* The :ref:`label_clock_service` of the master will wait at each time step for the slaves confirmation event that the time has been reached.
* The :ref:`label_scheduler_service_active_scheduler` will be informed about each logical time step and will execute the configured Jobs.
* The :ref:`label_scheduler_service_active_scheduler` will watch runtime execution times and will react to violation according to the configured strategy (see :ref:`label_clock_based_scheduler`).


External clock (i.e. player)
----------------------------

Use Cases:
^^^^^^^^^^

* Simulated deterministic test environment
* Fixed distributed scheduling by a discrete logical time from the external clock (i.e. an external player!)

Configuration:
^^^^^^^^^^^^^^

.. list-table::
   :header-rows: 1

   * -
     - Master Participant
     - Participant 2
     - Participant 3
   * - :ref:`label_clock_service_main_clock`
     - some_external_player_clock
     - :ref:`label_clock_sync_slave_master_on_demand_discrete`
     - :ref:`label_clock_sync_slave_master_on_demand_discrete`
   * - :ref:`label_scheduler_service_active_scheduler`
     - :ref:`label_clock_based_scheduler`
     - :ref:`label_clock_based_scheduler`
     - :ref:`label_clock_based_scheduler`
   * - :c:macro:`FEP3_CLOCK_SERVICE_CLOCK_SIM_TIME_STEP_SIZE`
     -
     - 100 ms
     - 100 ms
   * - :c:macro:`FEP3_CLOCKSYNC_SERVICE_CONFIG_TIMING_MASTER`
     -
     - "Master Participant"
     - "Master Participant"

This configuration means:
^^^^^^^^^^^^^^^^^^^^^^^^^


* Time synchronization is initiated by registering to the :ref:`label_clock_service` of the timing master.
* Each :ref:`label_clock_sync_slave_master_on_demand_discrete` clock will receive time update events from the configured master.
* The :ref:`label_clock_service` of the master will wait at each time step for the slaves confirmation event that the time has been reached.
* The :ref:`label_scheduler_service_active_scheduler` will be informed about each logical time step and will execute the configured Jobs.
* The :ref:`label_scheduler_service_active_scheduler` will watch runtime execution times and will react to violation according to the configured strategy (see :ref:`label_clock_based_scheduler`).



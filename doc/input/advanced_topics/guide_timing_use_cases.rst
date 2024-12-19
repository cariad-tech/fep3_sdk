.. Copyright 2023 CARIAD SE.
.. 
.. This Source Code Form is subject to the terms of the Mozilla 
.. Public License, v. 2.0. If a copy of the MPL was not distributed 
.. with this file, You can obtain one at https://mozilla.org/MPL/2.0/.


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

Use Cases
---------

* Development of new participants.
* Non time dependent functional testing of participants.

Configuration
-------------

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


This configuration means
^^^^^^^^^^^^^^^^^^^^^^^^

* No time synchronization is used.
* The time of every participant will continue by its own system clock independent from any other participant.
* The :ref:`label_scheduler_service_active_scheduler` will continuously look at the time and will execute the configured jobs at the corresponding point in time.
  The :ref:`label_clock_based_scheduler` has to repeatedly check the local time for pending job triggers and is subjected to the operating system process scheduler both of which might lead to time uncertainties.
* The :ref:`label_scheduler_service_active_scheduler` will watch runtime execution times and will react to violation according to the configured strategy (see :ref:`label_clock_based_scheduler`).


.. _label_guide_timing_use_cases_continuous_synchronization:

Master/Slave continuous synchronization
=======================================

Native real-time clock
----------------------

Use Cases
^^^^^^^^^

* Realtime mode within a real-time environment like vehicle prototypes or other distributed systems.
* Non blocking scheduling across multiple FEP participants in a FEP system.
* Data driven systems which need to avoid blocking of participants due to time distribution activity and time synchronization overhead.

Configuration
^^^^^^^^^^^^^

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
     - 1 s
     - 1 s
   * - :c:macro:`FEP3_CLOCKSYNC_SERVICE_CONFIG_TIMING_MASTER`
     -
     - "Master Participant"
     - "Master Participant"

This configuration means
^^^^^^^^^^^^^^^^^^^^^^^^

* *Participant 2* and *Participant 3* are timing slaves, the :ref:`label_clock_sync_slave_master_on_demand` clocks will synchronize its own local system clock every 1 s with the *Master Participant*.
* The slave times are straightened by the Cristian's Algorithm using the round trip time of getting the time from the master.
* The *Master Participant* will provide its time using a continuous local system real time.
* The :ref:`label_scheduler_service_active_scheduler` will continuously look at the time and will execute the configured jobs at the corresponding point in time.
  The :ref:`label_clock_based_scheduler` has to repeatedly check the local time for pending job triggers and is subjected to the operating system process scheduler both of which might lead to time uncertainties.
* The :ref:`label_scheduler_service_active_scheduler` will watch runtime execution times and will react to violation according to the configured strategy (see :ref:`label_clock_based_scheduler`).

External real-time clock
------------------------

Use Cases
^^^^^^^^^

* Realtime mode within a real-time environment like vehicle prototypes.
* Non blocking scheduling across multiple FEP participants in a FEP system.
* The time source clock *some_fancy_realtime_clock* is provided by an external implementation.

Configuration
^^^^^^^^^^^^^

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



This configuration means
^^^^^^^^^^^^^^^^^^^^^^^^

* *Participant 2* and *Participant 3* are timing slaves, the :ref:`label_clock_sync_slave_master_on_demand` clock will synchronize every 100 ms with the *Master Participant* system time.
* The slave times are straightened by the Cristian's Algorithm using the round-trip time of getting the time from the master.
* The :ref:`label_clock_sync_slave_master_on_demand` clock will interpolate the time needed to exchange synchronization information between the timing slaves and the timing master during synchronization.
* The *Master Participant* will provide the time by a continuous external time called *some_fancy_realtime_clock*.
  The :ref:`label_clock_based_scheduler` has to repeatedly check the local time for pending job triggers and is subjected to the operating system process scheduler both of which might lead to time uncertainties. 
* The :ref:`label_scheduler_service_active_scheduler` will watch runtime execution times and will react to violation according to the configured strategy (see :ref:`label_clock_based_scheduler`).


.. _label_guide_timing_use_cases_discrete_synchronization:

Master/Slave discrete synchronization
=====================================

Native simulation time (fixed time steps)
-----------------------------------------

Use Cases
^^^^^^^^^

* Controlled simulation environment including determinism regarding the time related behaviour of participants.
* Fixed distributed scheduling of jobs across multiple FEP participants within a FEP system with discrete logical time.

Configuration
^^^^^^^^^^^^^

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
     - 100 ms
     -
     -
   * - :c:macro:`FEP3_CLOCK_SERVICE_CLOCK_SIM_TIME_TIME_FACTOR`
     - 1.0
     -
     -
   * - :c:macro:`FEP3_CLOCKSYNC_SERVICE_CONFIG_TIMING_MASTER`
     -
     - "Master Participant"
     - "Master Participant"



This configuration means
^^^^^^^^^^^^^^^^^^^^^^^^

* Time synchronization is initiated by registering all timing slaves to the :ref:`label_clock_service` of the timing master.
* Each :ref:`label_clock_sync_slave_master_on_demand_discrete` clock will receive time update events from the configured master.
* Due to the value of :c:macro:`FEP3_CLOCK_SERVICE_CLOCK_SIM_TIME_STEP_SIZE` the timing master updates all timing slaves using a simulation time step size of 100 ms and
  due to :c:macro:`FEP3_CLOCK_SERVICE_CLOCK_SIM_TIME_TIME_FACTOR` it tries to progress at the same speed as its underlying system clock.
* The :ref:`label_clock_service` of the master will wait at each time step for all slaves acknowledgments that the time has been reached and jobs have been executed.
* The :ref:`label_scheduler_service_active_scheduler` will be informed about each time update received by the clock and will execute jobs if necessary.
* The :ref:`label_scheduler_service_active_scheduler` will watch runtime execution times and will react to violation according to the configured strategy (see :ref:`label_clock_based_scheduler`).


External clock (i.e. player)
----------------------------

Use Cases
^^^^^^^^^

* Controlled simulation environment including determinism regarding the time related behaviour of participants
* Fixed distributed scheduling of jobs across multiple FEP participants within a FEP system by a discrete logical time from the external clock (i.e. an external player!)

Configuration
^^^^^^^^^^^^^

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
   * - :c:macro:`FEP3_CLOCKSYNC_SERVICE_CONFIG_TIMING_MASTER`
     -
     - "Master Participant"
     - "Master Participant"

This configuration means
^^^^^^^^^^^^^^^^^^^^^^^^

* The discrete clock has to be registered at the clock service and configured as main clock via its name.
* Time synchronization is initiated by registering all timing slaves to the :ref:`label_clock_service` of the timing master.
* Each :ref:`label_clock_sync_slave_master_on_demand_discrete` clock will receive time update events from the configured master.
* The :ref:`label_scheduler_service_active_scheduler` will be informed about each time update received by the clock and will execute jobs if necessary.
* The :ref:`label_scheduler_service_active_scheduler` will watch runtime execution times and will react to violation according to the configured strategy (see :ref:`label_clock_based_scheduler`).



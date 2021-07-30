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

.. _label_guide_timing:

.. sidebar:: Time Synchronization and Scheduling explained

        .. contents::

=============================================
Time Synchronization and Scheduling explained
=============================================

This page introduces a more detailed look into the time synchronization and scheduling mechanisms used.

The section :ref:`label_continuous_synchronization` basically refers to the setups in :ref:`label_guide_timing_use_cases_continuous_synchronization`.
The section :ref:`label_discrete_synchronization` basically refers to the setups in :ref:`label_guide_timing_use_cases_discrete_synchronization`.


.. _label_continuous_synchronization:

Continuous Synchronization
==========================

In case of a :ref:`label_continuous_synchronization` the master and the slave will each run with their own clock.
In an interval of :c:macro:`FEP3_CLOCKSYNC_SERVICE_CONFIG_SLAVE_SYNC_CYCLE_TIME` the slave will ask the master for its current time and synchronize its clock to that time.
The :ref:`label_scheduler_service` of the slaves will schedule jobs based on that time.

For this Use Case the following components are used:

    * :ref:`label_clock_service` (:ref:`label_clock_implementation_local_system_realtime`)
    * :ref:`label_clock_sync_service` (:ref:`label_clock_sync_slave_master_on_demand`)
    * :ref:`label_scheduler_service`

An Example
----------

The following demonstrates such a time synchronization.

We have the following setup

    * A *Timing Master* (:ref:`label_clock_implementation_local_system_realtime`).
    * A *Timing Slave 1* which has one job with a cycle time of 5ms (:ref:`label_clock_sync_slave_master_on_demand` with a sync cycle time of 100 ms).
    * A *Timing Slave 2* which has one job with a cycle time of 10ms (:ref:`label_clock_sync_slave_master_on_demand` with a sync cycle time of 50 ms).

Due to the :c:macro:`FEP3_CLOCKSYNC_SERVICE_CONFIG_SLAVE_SYNC_CYCLE_TIME` the *Timing Slave 1* will ask for the master's time every 100 ms and synchronize its clock to that time.
The *Timing Slave 2* will do the same but every 50 ms.
*Timing Slave 1* will then, based on his synchronized clock, schedule a job every 5ms. *Timing Slave 2* will schedule a job every 10ms.

If using this kind of time synchronization the clocks could drift apart in between the synchronization intervals.


.. _label_discrete_synchronization:

Discrete Time Synchronization
=============================

The master emits *timeUpdating* events whenever its time is changing. The event is sent to all timing slaves. If a timing slave receives such an event it might schedule one or more jobs.
When a timing master is emitting *timeUpdating* events, depends on the implementation of its configured main clock.

The :ref:`label_clock_implementation_local_system_simtime` clock for example sends *timeUpdating* events in the resolution of the property :c:macro:`FEP3_CLOCK_SERVICE_CLOCK_SIM_TIME_STEP_SIZE` based on the clock of the operating system.
An external_player might send *timeUpdating* events whenever it processes data with a new timestamp (of course that is dependent on the concrete implementation).
The :ref:`label_clock_sync_slave_master_on_demand_discrete` sync clock on the slave side will receive the *timeUpdating* event and update the local clock of the slave. The :ref:`label_scheduler_service` might then schedule jobs based on the new time.

Timing slaves have to register to the timing master to receive *timeUpdating* events. To do so the slaves have to specify the property :c:macro:`FEP3_CLOCKSYNC_SERVICE_CONFIG_TIMING_MASTER`.

For this Use Case the following components are used:

    * :ref:`label_clock_service` (:ref:`label_clock_implementation_local_system_simtime`)
    * :ref:`label_clock_sync_service` (:ref:`label_clock_sync_slave_master_on_demand_discrete`)
    * :ref:`label_scheduler_service`

An Example
----------

The following demonstrates such a time synchronization.

We have the following setup

    * A *Timing Master* emitting *timeUpdating* events for every 1 ms (:ref:`label_clock_implementation_local_system_simtime` (1ms)).
    * A *Timing Slave 1* which has one job with a cycle time of 5 ms (:ref:`label_clock_sync_slave_master_on_demand_discrete`).
    * A *Timing Slave 2* which has one job with a cycle time of 10 ms (:ref:`label_clock_sync_slave_master_on_demand_discrete`).

Due to the :c:macro:`FEP3_CLOCK_SERVICE_CLOCK_SIM_TIME_STEP_SIZE` of 1 ms the master sends a *timeUpdating* event for every 1 ms it simulates. The event is sent to all timing slaves in non determined order.
For every call it sends, it waits for the timing slave to process the event. Processing the *timeUpdating* event means that the slave checks, based on the received time, whether it needs to trigger a job.
If jobs need to be triggered the timing slave will wait with his response until all jobs are executed. This way the timing master will also wait until all job executions are completed.

In detail this will lead to the following behavior

.. image:: images/discrete_time_synchronisation_detail.png

The following table outlines the behavior for a simulation duration of 10ms.

+-----------------------+--------------------------------------------------------------------------------------------------------------------+
| Time                  |  What happens                                                                                                      |
+=======================+====================================================================================================================+
|                       | When going to RUNNING *Slave 1* and *Slave 2* will register themselves as timing slaves.                           |
+-----------------------+--------------------------------------------------------------------------------------------------------------------+
|  1 ms                 | The master will emit the timeUpdating event with *time == 1*. The slaves will forward the update to the schedulers.|
|                       | As there is no matching Job registered, the scheduler won't execute anything and just return.                      |
+-----------------------+--------------------------------------------------------------------------------------------------------------------+
| 2 ms - 4 ms           | *timeUpdating* events will be emitted but no Job will be executed (The same as for t == 1 ms).                     |
+-----------------------+--------------------------------------------------------------------------------------------------------------------+
|  5 ms                 | At simulation time 5 ms the *Slave 1* will trigger its 5 ms Job. *Slave 2* won't execute its 10 ms Job.            |
|                       | The Timing master waits until the scheduler of *Slave 1* has executed the job                                      |
|                       |                                                                                                                    |
+-----------------------+--------------------------------------------------------------------------------------------------------------------+
| 6 ms - 9 ms           | *timeUpdating* events will be emitted but no Job will be executed.                                                 |
+-----------------------+--------------------------------------------------------------------------------------------------------------------+
|  10 ms                | At simulation time 10 ms the *Slave 1* will trigger its 5 ms Job again.                                            |
|                       | The *Slave 2* will trigger its 10 ms job for the first time.                                                       |
|                       | The *Timing Master* waits until the schedulers of *Slave 1* and *Slave 2* have both executed the Jobs.             |
+-----------------------+--------------------------------------------------------------------------------------------------------------------+



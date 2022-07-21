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


.. _label_scheduler_service:

==================
Scheduler Service
==================

.. sidebar::   Scheduler Service

        .. contents::

Summary
=======

+------------------------------------------------------+-----------------------------------------------------------------+
| Name                                                 |  Scheduler Service                                              |
+------------------------------------------------------+-----------------------------------------------------------------+
| Component Interface                                  |  :cpp:class:`fep3::arya::ISchedulerService`                     |
+------------------------------------------------------+-----------------------------------------------------------------+
| RPC Service Interface                                |  :cpp:class:`fep3::rpc::arya::IRPCSchedulerServiceDef`          |
+------------------------------------------------------+-----------------------------------------------------------------+
| RPC Service Description                              |  :ref:`label_rpc_service_scheduler_service`                     |
+------------------------------------------------------+-----------------------------------------------------------------+
| native delivery                                      |  built-in                                                       |
+------------------------------------------------------+-----------------------------------------------------------------+
| CPP-plugin possible                                  |  yes                                                            |
+------------------------------------------------------+-----------------------------------------------------------------+
| C-plugin possible                                    |  yes                                                            |
+------------------------------------------------------+-----------------------------------------------------------------+


Overview
========

The :ref:`label_scheduler_service` manages schedulers and is responsible for the scheduling of the Jobs of a :term:`FEP Participant`.
Every participant has one :ref:`label_scheduler_service`. Various participants in a :term:`FEP System` may use different schedulers depending on the corresponding use case.

Component Interface
===================

The :cpp:class:`fep3::arya::ISchedulerService` interface offers the following functionality:

* Register and unregister schedulers.


.. _label_scheduler_service_scheduler:

Scheduler
---------

.. _label_scheduler_service_active_scheduler:

Active scheduler
================

The :ref:`label_scheduler_service` has one :ref:`label_scheduler_service_active_scheduler` which is responsible for the scheduling.


Native Implementations
======================

Following built-in native implementation is available and will be used automatically.

.. list-table::
   :header-rows: 1

   * - Name
     - Code Macro
   * - :ref:`label_clock_based_scheduler`
     - :c:macro:`FEP3_SCHEDULER_CLOCK_BASED`


.. _label_clock_based_scheduler:

clock_based_scheduler
---------------------------

The :ref:`label_clock_based_scheduler` triggers Jobs based on the local time of the participant provided by the :ref:`label_clock_service_main_clock` and the :cpp:member:`fep3::arya::JobConfiguration::_cycle_sim_time` of the Jobs.
The Jobs will cyclically be triggered every time the corresponding Job's cycle time passes.

The :ref:`label_clock_based_scheduler` will retrieve the Jobs from the :ref:`label_job_registry` when the :ref:`label_scheduler_service` is initializing (:cpp:func:`fep3::arya::IComponent::initialize`).
Therefore jobs have to be added when the :ref:`label_element_interface` is

* loading (:cpp:func:`fep3::arya::IElement::loadElement`),
* or unloading (:cpp:func:`fep3::arya::IElement::unloadElement`).

Also, for every Job execution a run time check is performed. The check is configured with the Jobs :cpp:class:`fep3::arya::JobConfiguration` (see class documentation for details).
The run time check will check that the execution time of a job does not exceed the configured :cpp:member:`fep3::arya::JobConfiguration::_max_runtime_real_time` of the Job.
The time check is done based on wall clock time and not simulation time. Using :cpp:member:`fep3::arya::JobConfiguration::_runtime_violation_strategy`
a strategy can be configured that will be applied in case the max runtime is exceeded.

.. note:: Jobs may be configured using a :ref:`label_job_registry_job_configuration`.

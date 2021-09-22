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


.. _label_job_registry:

=============
 Job Registry
=============

.. sidebar:: Job Registry

        .. contents::

Summary
=======

+------------------------------------------------------+-----------------------------------------------------------------+
| Name                                                 |  Job Registry                                                   |
+------------------------------------------------------+-----------------------------------------------------------------+
| Component Interface                                  |  :cpp:class:`fep3::arya::IJobRegistry`                          |
+------------------------------------------------------+-----------------------------------------------------------------+
| RPC Service Interface                                | :cpp:class:`fep3::rpc::arya::IRPCJobRegistryDef`                |
+------------------------------------------------------+-----------------------------------------------------------------+
| RPC Service Description                              |  :ref:`label_rpc_job_registry`                                  |
+------------------------------------------------------+-----------------------------------------------------------------+
| native delivery                                      |  built-in                                                       |
+------------------------------------------------------+-----------------------------------------------------------------+
| cpp-plugin possible                                  |  yes                                                            |
+------------------------------------------------------+-----------------------------------------------------------------+
| c-plugin possible                                    |  yes                                                            |
+------------------------------------------------------+-----------------------------------------------------------------+


Overview
========

The :ref:`label_job_registry` is responsible for managing all jobs (:cpp:class:`fep3::arya::IJobRegistry::IJob`) of a :term:`FEP Participant`
and for making them available to other components like the :ref:`label_scheduler_service`. Using the :ref:`label_job_registry_job_configuration` registered jobs can also be reconfigured without recompilation.

Component Interface
===================

The :cpp:class:`fep3::arya::IJobRegistry` interface offers the following functionality:

* Adding and removing Jobs to/from the :ref:`label_job_registry`.
* Retrieve information about all currently registered Jobs.
* Retrieve the Jobs itself. The retrieved Jobs can be used for scheduling.

.. note::
  A job has to be added/removed to/from the :ref:`label_job_registry` when the :ref:`label_element_interface` is:
    * loading (:cpp:func:`fep3::arya::IElement::loadElement`),
    * unloading (:cpp:func:`fep3::arya::IElement::unloadElement`),

.. _label_job:

Job
---

A :cpp:class:`fep3::arya::IJobRegistry::IJob` is a cyclic processing unit which can be added to the :ref:`label_job_registry`.
Every Job has a unique name within the :ref:`label_job_registry` it is added to.

A Job typically will read signal data from :cpp:class:`fep3::arya::IDataRegistry::IDataReader`, process received data samples and write signal samples to :cpp:class:`fep3::arya::IDataRegistry::IDataWriter`.
The configured :ref:`label_scheduler_service_active_scheduler` will trigger all jobs which are registered at the :ref:`label_job_registry` according to its scheduling mechanism.

The execution of a Job is separated by the following steps:

* :cpp:func:`fep3::arya::IJobRegistry::IJob::executeDataIn` to read sample data from readers.
* :cpp:func:`fep3::arya::IJobRegistry::IJob::execute` to do the actual processing.
* :cpp:func:`fep3::arya::IJobRegistry::IJob::executeDataOut` to write sample data to writers.

A Job is configured by a :cpp:class:`fep3::arya::JobConfiguration` when adding it to the :cpp:class:`fep3::arya::IJobRegistry`.
The main configuration of a Job is the :cpp:member:`fep3::arya::JobConfiguration::_cycle_sim_time` which determines in which intervals a Job is executed by the :ref:`label_scheduler_service_active_scheduler`.
For the other configuration options see :cpp:class:`fep3::arya::JobConfiguration`.

.. note::
  The configuration parameter :cpp:member:`fep3::arya::JobConfiguration::_delay_sim_time` is currently not evaluated.

Sample validity for a simulation step
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If samples are read from a :cpp:class:`fep3::arya::IDataRegistry::IDataReader`, only samples with a timestamp smaller than the current simulation time, will be provided.
Only those samples are considered to be valid for the current simulation step.
Samples with a simulation time equal to the current time are accessible from the next simulation step on.
With this simulation model there is no data dependency between jobs within one simulation step and
all jobs of one simulation step can be calculated in parallel.

Configuration
=============

The :ref:`label_job_registry` can be configured using the following properties.

Properties
----------


.. list-table::
   :widths: 20 60 20
   :header-rows: 1

   * - Name
     - Code Macro
     - Default Value
   * - Jobs
     - .. doxygendefine:: FEP3_JOB_REGISTRY_JOBS
        :no-link:
     - No value. Retrieves a job entry subproperty containing the job configuration values as subproperties on job registration.

.. note::
  Job entry subproperties of :c:macro:`FEP3_JOB_REGISTRY_JOBS` have to be set after the :ref:`label_element_interface` has been loaded (:cpp:func:`fep3::arya::IElement::loading`) when jobs are registered
  and before the :cpp:class:`fep3::arya::ISchedulerService` is tensing (:cpp:func:`fep3::arya::IComponent::tense`) when job nodes and their configurations are parsed and applied.

.. _label_job_registry_job_configuration:

Job Configuration
====================

During registration a job is configured using a :cpp:class:`fep3::arya::JobConfiguration`. To adapt the behaviour of participants without having to recompile,
registered jobs may be reconfigured using the :c:macro:`FEP3_JOB_REGISTRY_JOBS` property node which contains information regarding all registered jobs and their configuration.
A job entry equals a property node containing the job name as value and following subproperties containing information regarding the job configuration:

+------------------------------------------------------------------+----------------------------------------------------------------------------------------------------+--------------------------------------------------------------------------------------+
| **Attribute**                                                    | **Valid values**                                                                                   | **Description**                                                                      |
+------------------------------------------------------------------+----------------------------------------------------------------------------------------------------+--------------------------------------------------------------------------------------+
| .. doxygendefine:: FEP3_JOB_CYCLE_SIM_TIME_PROPERTY              | An integer > 0                                                                                     | The time after which a job is triggered cyclically by the scheduler in microseconds. |
+------------------------------------------------------------------+----------------------------------------------------------------------------------------------------+--------------------------------------------------------------------------------------+
| .. doxygendefine:: FEP3_JOB_DELAY_SIM_TIME_PROPERTY              | An integer >= 0                                                                                    | Not yet applied.                                                                     |
+------------------------------------------------------------------+----------------------------------------------------------------------------------------------------+--------------------------------------------------------------------------------------+
| .. doxygendefine:: FEP3_JOB_MAX_RUNTIME_REAL_TIME_PROPERTY       | An integer >= 0                                                                                    | The maximum real time value a job may take to run in microseconds.                   |
|                                                                  |                                                                                                    | If a job exceeds the configured value, a configured strategy is executed.            |
|                                                                  |                                                                                                    | In case of value '0', runtime violation strategies are not applied.                  |
+------------------------------------------------------------------+----------------------------------------------------------------------------------------------------+--------------------------------------------------------------------------------------+
| .. doxygendefine:: FEP3_JOB_RUNTIME_VIOLATION_STRATEGY_PROPERTY  | :cpp:enum:`fep3::arya::JobConfiguration::TimeViolationStrategy::ignore_runtime_violation`,         | The strategy to react to job runtime violations.                                     |
|                                                                  | :cpp:enum:`fep3::arya::JobConfiguration::TimeViolationStrategy::warn_about_runtime_violation`,     |                                                                                      |
|                                                                  | :cpp:enum:`fep3::arya::JobConfiguration::TimeViolationStrategy::skip_output_publish`,              |                                                                                      |
+------------------------------------------------------------------+----------------------------------------------------------------------------------------------------+--------------------------------------------------------------------------------------+

Native Implementations
======================

The :term:`FEP Participant Library` provides a native implementation of a :ref:`label_job_registry`, which will be used automatically.

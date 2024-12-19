.. Copyright 2023 CARIAD SE.
.. 
.. This Source Code Form is subject to the terms of the Mozilla 
.. Public License, v. 2.0. If a copy of the MPL was not distributed 
.. with this file, You can obtain one at https://mozilla.org/MPL/2.0/.


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
| Component Interface                                  |  :cpp:class:`fep3::catelyn::IJobRegistry`                       |
+------------------------------------------------------+-----------------------------------------------------------------+
| RPC Service Interface                                |  :cpp:class:`fep3::rpc::arya::IRPCJobRegistryDef`               |
+------------------------------------------------------+-----------------------------------------------------------------+
| RPC Service Description                              |  :ref:`label_rpc_job_registry`                                  |
+------------------------------------------------------+-----------------------------------------------------------------+
| native delivery                                      |  CPP-plugin                                                     |
+------------------------------------------------------+-----------------------------------------------------------------+
| CPP-plugin possible                                  |  yes                                                            |
+------------------------------------------------------+-----------------------------------------------------------------+
| C-plugin possible                                    |  yes                                                            |
+------------------------------------------------------+-----------------------------------------------------------------+


Overview
========

The :ref:`label_job_registry` is responsible for managing all jobs (:cpp:class:`fep3::catelyn::IJobRegistry::IJob`) of a :term:`FEP Participant`
and for making them available to other components like the :ref:`label_scheduler_service`. Using the :ref:`label_job_registry_job_configuration` registered jobs can also be reconfigured without recompilation.

Component Interface
===================

The :cpp:class:`fep3::catelyn::IJobRegistry` interface offers the following functionality:

* Adding and removing Jobs to/from the :ref:`label_job_registry`.
* Retrieve information about all currently registered Jobs.
* Retrieve the Jobs itself. The retrieved Jobs can be used for scheduling.

.. note::
  A job has to be added/removed to/from the :ref:`label_job_registry` when the :ref:`Element interface <label_element_interface>` is:
    * loading (:cpp:func:`fep3::base::IElement::loadElement`),
    * unloading (:cpp:func:`fep3::base::IElement::unloadElement`),

.. _label_job:

Job
---

A :cpp:class:`fep3::catelyn::IJobRegistry::IJob` is a processing unit which can be added to the :ref:`label_job_registry`.
Every Job has a unique name within the :ref:`label_job_registry` it is added to.

A Job typically will read signal data from :cpp:class:`fep3::arya::IDataRegistry::IDataReader`, process received data samples and write signal samples to :cpp:class:`fep3::arya::IDataRegistry::IDataWriter`.
The configured :ref:`label_scheduler_service_active_scheduler` will trigger all jobs which are registered at the :ref:`label_job_registry` according to its scheduling mechanism.

The execution of a Job is separated by the following steps:

* :cpp:func:`fep3::arya::IJob::executeDataIn()` to read sample data from readers.
* :cpp:func:`fep3::arya::IJob::execute()` to do the actual processing.
* :cpp:func:`fep3::arya::IJob::executeDataOut()` to write sample data to writers.

A Job is configured by a :cpp:class:`fep3::catelyn::ClockTriggeredJobConfiguration` or a :cpp:class:`fep3::catelyn::DataTriggeredJobConfiguration`,
when adding it to the :cpp:class:`fep3::catelyn::IJobRegistry`.
See :ref:`label_clock_triggered_job_configuration` and :ref:`label_data_triggered_job_configuration`.

.. note::
  The configuration parameter :cpp:member:`fep3::catelyn::JobConfiguration::_delay_sim_time` is currently not evaluated.

.. _label_sample_validity:

Sample validity for a simulation step
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If using :ref:`label_cpp_datajob`, created data readers are configured to handle sample reception according to the provided job configuration.

In case of a clock triggered job a :cpp:class:`fep3::arya::IDataRegistry::IDataReader` will provide samples with a timestamp smaller than the current simulation time only.
Only those samples are considered to be valid for the current simulation step.
Samples with a simulation time equal to the current time are accessible from the next simulation step on.
With this simulation model there is no data dependency between jobs within one simulation step and
all jobs of one simulation step can be calculated in parallel.

In case of a data triggered job a :cpp:class:`fep3::arya::IDataRegistry::IDataReader` will provide samples with a timestamp smaller or equal to the current simulation time only.
Only those samples are considered to be valid for the current simulation step.

.. note::
  If not using :ref:`label_cpp_datajob`, created :cpp:class:`fep3::core::DataReader` s have to be configured manually.

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
     - :c:macro:`FEP3_JOB_REGISTRY_JOBS`
     - No value. Retrieves a job entry subproperty containing the job configuration values as subproperties on job registration.

.. note::
  Job entry sub properties of :c:macro:`FEP3_JOB_REGISTRY_JOBS` have to be set after the :ref:`Element interface <label_element_interface>` has been loaded (:cpp:func:`fep3::base::IElement::loading`) when jobs are registered
  and before the :cpp:class:`fep3::arya::ISchedulerService` is tensing (:cpp:func:`fep3::arya::IComponent::tense`) when job nodes and their configurations are parsed and applied.

.. _label_job_registry_job_configuration:

Job Configuration
=================

During registration a job can be configured using a :cpp:class:`fep3::catelyn::ClockTriggeredJobConfiguration` or :cpp:class:`fep3::catelyn::DataTriggeredJobConfiguration`. 
To adapt the behavior of participants without having to recompile, registered jobs may be reconfigured using the :c:macro:`FEP3_JOB_REGISTRY_JOBS` property node which contains information regarding all registered jobs and their configuration.

A job entry equals a property node containing the job name as value and following sub properties containing information regarding the job configuration. 
Each type of job has its own configurations.

.. _label_clock_triggered_job_configuration: 

ClockTriggeredJobConfiguration
------------------------------

+------------------------------------------------------------+---------------------------------------+---------------------------------------------------------------------+---------------------------+
| **Attribute**                                              | **Valid values**                      | **Description**                                                     | **Default Value**         |
+------------------------------------------------------------+---------------------------------------+---------------------------------------------------------------------+---------------------------+
| :c:macro:`FEP3_JOB_TRIGGER_TYPE_PROPERTY`                  | "cyclic_trigger" or "data_trigger"    | The trigger type of a job                                           | "cyclic_trigger"          |
+------------------------------------------------------------+---------------------------------------+---------------------------------------------------------------------+---------------------------+
| :c:macro:`FEP3_JOB_CYCLE_SIM_TIME_PROPERTY`                | An integer > 0                        | The time which a job                                                | No default value.         |
|                                                            |                                       | is triggered cyclically by the scheduler in nanoseconds.            |                           |
+------------------------------------------------------------+---------------------------------------+---------------------------------------------------------------------+---------------------------+
| :c:macro:`FEP3_JOB_DELAY_SIM_TIME_PROPERTY`                | An integer >= 0                       | Not yet applied.                                                    | 0ns. Job is not delayed.  |
+------------------------------------------------------------+---------------------------------------+---------------------------------------------------------------------+---------------------------+
| :c:macro:`FEP3_JOB_MAX_RUNTIME_REAL_TIME_PROPERTY`         | An integer >= 0                       | The maximum real time value a job may take to run in nanoseconds.   | 0ns.                      |
|                                                            |                                       | If a job exceeds the configured value, a configured strategy        | Job runtime is not checked| 
|                                                            |                                       | is executed.                                                        |                           |
|                                                            |                                       | In case of value '0', runtime violation strategies are not applied  |                           |
+------------------------------------------------------------+---------------------------------------+---------------------------------------------------------------------+---------------------------+
| :c:macro:`FEP3_JOB_RUNTIME_VIOLATION_STRATEGY_PROPERTY`    | :ref:`label_time_violation_strategy`  | The strategy to react to job runtime violations.                    | Runtime violations        |
|                                                            |                                       |                                                                     | are ignored               |
+------------------------------------------------------------+---------------------------------------+---------------------------------------------------------------------+---------------------------+

.. _label_data_triggered_job_configuration:

DataTriggeredJobConfiguration
------------------------------

+------------------------------------------------------------+---------------------------------------+---------------------------------------------------------------------+---------------------------+
| **Attribute**                                              | **Valid values**                      | **Description**                                                     | **Default Value**         |
+------------------------------------------------------------+---------------------------------------+---------------------------------------------------------------------+---------------------------+
| :c:macro:`FEP3_JOB_TRIGGER_TYPE_PROPERTY`                  | "cyclic_trigger" or "data_trigger"    | The trigger type of a job                                           | "data_trigger"            |
+------------------------------------------------------------+---------------------------------------+---------------------------------------------------------------------+---------------------------+
| :c:macro:`FEP3_JOB_TRIGGER_SIGNAL_PROPERTY`                | vector of string for signal names     | The signal names for triggering the job                             | No default value          |
+------------------------------------------------------------+---------------------------------------+---------------------------------------------------------------------+---------------------------+
| :c:macro:`FEP3_JOB_MAX_RUNTIME_REAL_TIME_PROPERTY`         | An integer >= 0                       | The maximum real time value a job may take to run in nanoseconds.   | 0ns.                      |
|                                                            |                                       | If a job exceeds the configured value, a configured strategy        | Job runtime is not checked| 
|                                                            |                                       | is executed.                                                        |                           |
|                                                            |                                       | In case of value '0', runtime violation strategies are not applied  |                           |
+------------------------------------------------------------+---------------------------------------+---------------------------------------------------------------------+---------------------------+
| :c:macro:`FEP3_JOB_RUNTIME_VIOLATION_STRATEGY_PROPERTY`    | :ref:`label_time_violation_strategy`  | The strategy to react to job runtime violations.                    | Runtime violations        |
|                                                            |                                       |                                                                     | are ignored               |
+------------------------------------------------------------+---------------------------------------+---------------------------------------------------------------------+---------------------------+

.. note::
  When reconfiguring a job's trigger type, the corresponding data readers are not reconfigured regarding their :ref:`data reception behaviour <label_element_interface>`.
  A job which has been created using either a data triggered or clock triggered job configuration will stick with the corresponding data reader configuration and therefore data reader sample reception behaviour.

.. _label_time_violation_strategy:

Time Violation Strategy
-----------------------

There are mainly three strategies to choose when initialize a job configuration

+--------------------------------------------------------------------------------------------------------+
| **Strategy**                                                                                           |
+--------------------------------------------------------------------------------------------------------+
| :cpp:enumerator:`fep3::catelyn::IJobConfiguration::TimeViolationStrategy::unknown`                     |
+--------------------------------------------------------------------------------------------------------+
| :cpp:enumerator:`fep3::catelyn::IJobConfiguration::TimeViolationStrategy::ignore_runtime_violation`    |
|                                                                                                        |
+--------------------------------------------------------------------------------------------------------+
| :cpp:enumerator:`fep3::catelyn::JobConfiguration::TimeViolationStrategy::warn_about_runtime_violation` |
+--------------------------------------------------------------------------------------------------------+
| :cpp:enumerator:`fep3::catelyn::IJobConfiguration::TimeViolationStrategy::skip_output_publish`         |
+--------------------------------------------------------------------------------------------------------+

Native Implementations
======================

The :term:`FEP Participant Library` provides a native implementation of a :ref:`label_job_registry`, which will be used automatically.

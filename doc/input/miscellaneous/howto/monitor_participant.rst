.. Copyright 2023 CARIAD SE.
.. 
.. This Source Code Form is subject to the terms of the Mozilla 
.. Public License, v. 2.0. If a copy of the MPL was not distributed 
.. with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

.. |br| raw:: html

  <br/>

.. _label_guide_monitor_participant:

=====================================
How to remotely monitor a participant
=====================================

Check if the participant is online
==================================
In order to check if the participant is online the easiest way is to use the call :cpp:func:`fep3::System::getParticipantState` from the 
:term:`FEP System Library`. In case the participant is not
available (it cannot be reached with an :term:`RPC <RPC Service>` call) the function will return :cpp:enumerator:`fep3::rpc::arya::ParticipantState::unreachable`.
A possible example could look as follows:

.. literalinclude:: ../../snippets/snippet_monitor_participant.cpp
    :start-after: //start_monitor_participant_getParticipantState
    :end-before: //end_monitor_participant_getParticipantState


Using the Health Service
========================
By accessing the :ref:`label_health_service` from the :term:`FEP System Library`,  you can check the errors returned by running jobs
or whether the participant is alive.

.. literalinclude:: ../../snippets/snippet_monitor_participant.cpp
    :start-after: //start_monitor_participant_checkParticipantHealth
    :end-before: //end_monitor_participant_checkParticipantHealth

In addition you can (indirectly) observe from the :cpp:struct:`fep3::catelyn::IHealthService::JobExecuteResult` whether the job execution
is hanging by checking if :cpp:member:`fep3::IHealthService::catelyn::JobExecuteResult::simulation_time` is getting updated.

.. literalinclude:: ../../snippets/snippet_monitor_participant.cpp
    :start-after: //start_monitor_participant_check_hanging_job
    :end-before: //end_monitor_participant_check_hanging_job

A detailed description of accessing the health service from the :term:`FEP System Library` can be found in :ref:`label_accessing_participants_health`.

Logging
=======
The :ref:`label_scheduler_service` also uses the :ref:`label_logging_service`  to report long running jobs. Two log types in particular would be of interest here.
The first debug log will be issued in case a job is scheduled while the previous job call did not finish.
The second warning will be logged according to the setting of :cpp:member:`fep3::arya::JobConfiguration::_runtime_violation_strategy` and
:cpp:member:`fep3::arya::JobConfiguration::_max_runtime_real_time`.
Assuming there is a clock triggered job that is taking more than its period to execute, if we configure system as follows:

.. code-block:: console

    set_participant_property demo_core_sender /job_registry/jobs/sender_job/runtime_violation_strategy warn_about_runtime_violation
    set_participant_property demo_core_sender /job_registry/jobs/sender_job/max_runtime_real_time 10000000
    set_participant_property demo_core_sender /logging/default_severity 5

When the system is running the following logs will be given:

.. code-block:: console

    Debug Task 'sender_job' not scheduled in time 29001421100 ns, previous task call is not yet finished
    Warning Job sender_job: Computation time (706900504 us) exceeded configured maximum runtime.

More information about the scheduler warnings can be found in the :ref:`scheduler documentation <label_clock_based_scheduler>`.
In order to access the participant logs from the :term:`FEP System Library` you should register a :cpp:class:`fep3::IEventMonitor`
and then access the participant logs.

.. warning::
    Log message content may change so it is not recommended to implement any logic that is based on volatile message strings.

Service Bus
===========
This method is referenced for completeness and should not be used systematically as a method to infer the participant's 
status. Within the participant you can register for the :term:`heartbeat messages <Heartbeat>` that all the participants send. First you need to
implement the :cpp:class:`fep3::IServiceBus::IServiceUpdateEventSink` interface and registering using  :cpp:func:`fep3::catelyn::IServiceBus::ISystemAccess::registerUpdateEventSink`.
By examining the structure :cpp:struct:`fep3::catelyn::IServiceBus::ServiceUpdateEvent` and implementing a timer you can check if the participant is still alive.
More information can be found in :ref:`access_service_bus_discovery_messages`


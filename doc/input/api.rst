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


.. _API_References:

===========
FEP SDK API
===========

Doxygen documentation
=====================

The complete Doxygen documentation can be found here `fep3-sdk doxygen <../doxygen_html/index.html>`_ .


RPC Service
===========

.. _label_rpc_data_registry:

RPC Data Registry
-----------------

.. literalinclude:: {PARTICIPANT_DIR}/include/fep3/rpc_services/data_registry/data_registry.json


.. _label_rpc_service_clock:

RPC Clock Service
-----------------

.. literalinclude:: {PARTICIPANT_DIR}/include/fep3/rpc_services/clock/clock.json


.. _label_rpc_service_clock_sync_slave:

RPC Clock Sync Slave
--------------------
.. literalinclude:: {PARTICIPANT_DIR}/include/fep3/rpc_services/clock_sync/clock_sync_slave.json


.. _label_rpc_service_clock_sync_master:

RPC Clock Sync Master
---------------------

.. literalinclude:: {PARTICIPANT_DIR}/include/fep3/rpc_services/clock_sync/clock_sync_master.json


.. _label_rpc_service_configuration_service:

RPC Configuration Service
-------------------------

.. literalinclude:: {PARTICIPANT_DIR}/include/fep3/rpc_services/configuration/configuration.json

.. _label_rpc_job_registry:

RPC Job Registry
----------------

.. literalinclude:: {PARTICIPANT_DIR}/include/fep3/rpc_services/job_registry/job_registry.json

.. _label_rpc_logging_service:

RPC Logging Service
-------------------

.. literalinclude:: {PARTICIPANT_DIR}/include/fep3/rpc_services/logging/logging_service.json

.. _label_rpc_participant_info:

RPC Participant Info
--------------------

.. literalinclude:: {PARTICIPANT_DIR}/include/fep3/rpc_services/participant_info/participant_info.json

.. _label_rpc_service_scheduler_service:

RPC Scheduler Service
---------------------

.. literalinclude:: {PARTICIPANT_DIR}/include/fep3/rpc_services/scheduler_service/scheduler_service.json

Macros
======


Clock Service
-------------

.. doxygendefine:: FEP3_CLOCK_SERVICE_CONFIG
.. doxygendefine:: FEP3_MAIN_CLOCK_PROPERTY
.. doxygendefine:: FEP3_CLOCK_SIM_TIME_STEP_SIZE_PROPERTY
.. doxygendefine:: FEP3_CLOCK_SIM_TIME_TIME_FACTOR_PROPERTY
.. doxygendefine:: FEP3_TIME_UPDATE_TIMEOUT_PROPERTY

.. doxygendefine:: FEP3_CLOCK_SERVICE_MAIN_CLOCK

.. doxygendefine:: FEP3_CLOCK_SERVICE_CLOCK_SIM_TIME_STEP_SIZE
.. doxygendefine:: FEP3_CLOCK_SIM_TIME_STEP_SIZE_MIN_VALUE
.. doxygendefine:: FEP3_CLOCK_SIM_TIME_STEP_SIZE_MAX_VALUE
.. doxygendefine:: FEP3_CLOCK_SIM_TIME_STEP_SIZE_DEFAULT_VALUE

.. doxygendefine:: FEP3_CLOCK_SERVICE_CLOCK_SIM_TIME_TIME_FACTOR
.. doxygendefine:: FEP3_CLOCK_SIM_TIME_TIME_FACTOR_AFAP_VALUE
.. doxygendefine:: FEP3_CLOCK_SIM_TIME_TIME_FACTOR_DEFAULT_VALUE

.. doxygendefine:: FEP3_CLOCK_WALL_CLOCK_STEP_SIZE_MIN_VALUE
.. doxygendefine:: FEP3_CLOCK_WALL_CLOCK_STEP_SIZE_MAX_VALUE
    
.. doxygendefine:: FEP3_CLOCK_LOCAL_SYSTEM_REAL_TIME
.. doxygendefine:: FEP3_CLOCK_LOCAL_SYSTEM_SIM_TIME

.. doxygendefine:: FEP3_CLOCK_SERVICE_TIME_UPDATE_TIMEOUT
.. doxygendefine:: FEP3_TIME_UPDATE_TIMEOUT_DEFAULT_VALUE

Clock Sync Service
------------------
.. doxygendefine:: FEP3_CLOCKSYNC_SERVICE_CONFIG

.. doxygendefine:: FEP3_TIMING_MASTER_PROPERTY
.. doxygendefine:: FEP3_CLOCKSYNC_SERVICE_CONFIG_TIMING_MASTER
.. doxygendefine:: FEP3_CLOCK_SLAVE_MASTER_ONDEMAND
.. doxygendefine:: FEP3_CLOCK_SLAVE_MASTER_ONDEMAND_DISCRETE

.. doxygendefine:: FEP3_SLAVE_SYNC_CYCLE_TIME_PROPERTY
.. doxygendefine:: FEP3_CLOCKSYNC_SERVICE_CONFIG_SLAVE_SYNC_CYCLE_TIME
.. doxygendefine:: FEP3_SLAVE_SYNC_CYCLE_TIME_DEFAULT_VALUE

Scheduler Service
-----------------

.. doxygendefine:: FEP3_SCHEDULER_SERVICE_CONFIG

.. doxygendefine:: FEP3_SCHEDULER_PROPERTY
.. doxygendefine:: FEP3_SCHEDULER_SERVICE_SCHEDULER
.. doxygendefine:: FEP3_SCHEDULER_CLOCK_BASED

Job Registry
------------

.. doxygendefine:: FEP3_JOB_REGISTRY_CONFIG

.. doxygendefine:: FEP3_JOBS_PROPERTY
.. doxygendefine:: FEP3_JOB_REGISTRY_JOBS

.. doxygendefine:: FEP3_JOB_CYCLE_SIM_TIME_PROPERTY
.. doxygendefine:: FEP3_JOB_DELAY_SIM_TIME_PROPERTY
.. doxygendefine:: FEP3_JOB_MAX_RUNTIME_REAL_TIME_PROPERTY
.. doxygendefine:: FEP3_JOB_RUNTIME_VIOLATION_STRATEGY_PROPERTY

Data Registry
-------------

.. doxygendefine:: FEP3_DATA_REGISTRY_CONFIG

.. doxygendefine:: FEP3_MAPPING_CONFIGURATION_PROPERTY
.. doxygendefine:: FEP3_DATA_REGISTRY_MAPPING_CONFIGURATION
.. doxygendefine:: FEP3_MAPPING_CONFIGURATION_FILE_PATH_PROPERTY
.. doxygendefine:: FEP3_DATA_REGISTRY_MAPPING_CONFIGURATION_FILE_PATH
.. doxygendefine:: FEP3_SIGNAL_RENAMING_INPUT_CONFIGURATION_PROPERTY
.. doxygendefine:: FEP3_DATA_REGISTRY_SIGNAL_RENAMING_INPUT_CONFIGURATION
.. doxygendefine:: FEP3_SIGNAL_RENAMING_OUTPUT_CONFIGURATION_PROPERTY
.. doxygendefine:: FEP3_DATA_REGISTRY_SIGNAL_RENAMING_OUTPUT_CONFIGURATION

Namespace fep3
==============

.. doxygennamespace:: fep3
    :members:

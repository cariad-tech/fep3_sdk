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


.. _label_participant_library:

================================
(OLD) :term:`FEP Participant` Library
================================

Participant Library and API
___________________________


.. _label_participant_and_element:

Participant and Element
-----------------------

+-----------------------------------------------+------------------------------------------------+------------------------------------------------------------+
|   :cpp:class:`fep3::arya::Participant`        |   :cpp:class:`fep3::arya::IElement`            | :cpp:class:`fep3::rpc::arya::IRPCParticipantStateMachine`  |
|                                               |                                                |                                                            |
|      :ref:`label_participant`                 |       :ref:`label_element_interface`           |     :ref:`label_participant_state_machine`                 |
|                                               |                                                |                                                            |
|                                               |                                                |                                                            |
+-----------------------------------------------+------------------------------------------------+------------------------------------------------------------+

.. toctree::
    :maxdepth: 1

    participant_and_element


.. _label_components:

Components
----------

The functional parts of the :term:`FEP Participant` are called :term:`FEP Components<FEP Component>`.
Each :term:`FEP Component` covers some specific functionality, e. g. functionality to exchange simulation data with other participants.

:term:`FEP Components<FEP Component>` can be accessed via the :term:`FEP Participant` API's
template method getComponent() in :cpp:class:`fep3::core::arya::Participant`, passing the :term:`FEP Component Interface`
as template argument.

.. _label_default_components:

FEP Default Components
~~~~~~~~~~~~~~~~~~~~~~
By default the :term:`FEP Participant` loads the FEP Default Component set, which consists of the
following :term:`FEP Components<FEP Component>`:

+--------------------------------------+--------------------------------------------------+-------------------------------------------------------+
| :term:`FEP Component`                | :term:`FEP Component Interface`                  | RPC Service Interface                                 |
+======================================+==================================================+=======================================================+
| :ref:`label_clock_service`           | :cpp:class:`fep3::arya::IClockService`           | :cpp:class:`fep3::rpc::arya::IRPCClockService`        |
+--------------------------------------+--------------------------------------------------+-------------------------------------------------------+
| :ref:`label_clock_sync_service`      | :cpp:class:`fep3::arya::IClockSyncService`       | :cpp:class:`fep3::rpc::arya::IRPCClockSyncSlaveDef`   |
+--------------------------------------+--------------------------------------------------+-------------------------------------------------------+
| :ref:`label_configuration_service`   | :cpp:class:`fep3::arya::IConfigurationService`   | :cpp:class:`fep3::rpc::arya::IRPCConfiguration`       |
+--------------------------------------+--------------------------------------------------+-------------------------------------------------------+
| :ref:`label_data_registry`           | :cpp:class:`fep3::arya::IDataRegistry`           | :cpp:class:`fep3::rpc::arya::IRPCDataRegistry`        |
+--------------------------------------+--------------------------------------------------+-------------------------------------------------------+
| :ref:`label_job_registry`            | :cpp:class:`fep3::arya::IJobRegistry`            |                                                       |
+--------------------------------------+--------------------------------------------------+-------------------------------------------------------+
| :ref:`label_logging_service`         | :cpp:class:`fep3::arya::ILoggingService`         | :cpp:class:`fep3::rpc::arya::IRPCLoggingService`      |
+--------------------------------------+--------------------------------------------------+-------------------------------------------------------+
| :ref:`label_participant_info`        | :cpp:class:`fep3::arya::IParticipantInfo`        | :cpp:class:`fep3::rpc::arya::IRPCParticipantInfo`     |
+--------------------------------------+--------------------------------------------------+-------------------------------------------------------+
| :ref:`label_scheduler_service`       | :cpp:class:`fep3::arya::ISchedulerService`       |                                                       |
+--------------------------------------+--------------------------------------------------+-------------------------------------------------------+
| :ref:`label_service_bus`             | :cpp:class:`fep3::arya::IServiceBus`             | :cpp:class:`fep3::rpc::arya::IRPCParticipantInfo`     |
+--------------------------------------+--------------------------------------------------+-------------------------------------------------------+
| :ref:`label_simulation_bus`          | :cpp:class:`fep3::arya::ISimulationBus`          |                                                       |
+--------------------------------------+--------------------------------------------------+-------------------------------------------------------+

.. toctree::
    :hidden:

    native_components

FEP Component System
~~~~~~~~~~~~~~~~~~~~
The :ref:`label_component_system` provides facilities to make the :term:`FEP Participants<FEP Participant>` of a :term:`FEP System`
use other :term:`FEP Components<FEP Component>` than the :ref:`label_default_components`, e. g. in order to replace a native component
by a custom implementation.

.. _label_core_api:

Core API
________

.. _label_core_cmake:

Core CMake Config
-----------------

The Core API comes with its own target and can be used as follows:

::

  # find the package
  find_package(fep3_participant_core REQUIRED)

  # ... define your target and other stuff

  # link against the fep3_participant_core
  target_link_libraries(my_target PRIVATE
                        fep3_participant_core)


.. _label_core_element_base:

Element Base
------------

See :ref:`label_demo_core_easy` for an example how to use the
:cpp:class:`fep3::core::arya::ElementBase`.

The :cpp:class:`fep3::core::arya::ElementBase` is a convenience class that
implements the :ref:`label_element_interface`. It handles the typename and the version
information of a FEP Element (see :cpp:func:`fep3::core::arya::ElementBase::getTypename`,
:cpp:func:`fep3::core::arya::ElementBase::getVersion`) and provides access to
:term:`FEP Components<FEP Component>` via :cpp:func:`fep3::core::arya::ElementBase::getComponents`.
Calls to :cpp:func:`fep3::arya::IElement::loadElement` in the context of the
:term:`FEP Participant`'s :ref:`label_participant_state_machine` are tranferred to calls to
:cpp:func:`fep3::core::arya::ElementBase::load` (:cpp:func:`fep3::arya::IElement::unloadElement`
to :cpp:func:`fep3::arya::ElementBase::unload` respectively).

.. _label_core_element_configurable:

Element Configurable
--------------------

The :cpp:class:`fep3::core::arya::ElementConfigurable` base class provides a convenience class
to use the :cpp:class:`fep3::arya::IConfigurationService`.

See :ref:`label_demo_core_easy` for an example how to use the
:cpp:class:`fep3::arya::Configuration`.

.. _label_core_easy_logging:

Easy Logging
------------

The :cpp:class:`fep3::logging::arya::EasyLogging` base class provides a convenience class and macros
to use the :cpp:class:`fep3::arya::ILoggingService`. If more customization is required
the :ref:`label_logging_service` needs to be used, which allows multiple loggers,
logger grouping and custom logging sinks.

See :ref:`label_demo_core_easy` for an example how to use the
:cpp:class:`fep3::logging::arya::EasyLogging`.

If :cpp:class:`fep3::core::arya::ElementBase` or :cpp:class:`fep3::core::arya::ElementConfigurable`
is used then :cpp:class:`fep3::logging::arya::EasyLogging` is already included and does not need to
be initialized. To filter the logs from these two classes the logger name "element" has to be used.

.. _label_core_data_writer_data_reader:

Core DataWriter and DataReader
------------------------------

See :ref:`label_demo_core_easy` for an example how to use the core DataWriter and DataReader.

The :cpp:class:`fep3::core::arya::DataWriter` and :cpp:class:`fep3::core::arya::DataReader`
are helper classes to add and remove :cpp:class:`fep3::core::arya::IDataRegistry::IDataReader` and
:cpp:class:`fep3::core::arya::IDataRegistry::IDataWriter`.

Use the :cpp:func:`fep3::core::arya::addToComponents` to add Reader or Writer to the :ref:`label_data_registry`.
Use the :cpp:func:`fep3::core::arya::removeFromComponents` to remove Reader or Writer from the :ref:`label_data_registry`.

.. _label_core_data_writer:

DataWriter
~~~~~~~~~~

DataWriter are queues that will hold the written data samples before they are flushed to the
corresponding :cpp:class:`fep3::arya::IDataRegistry::IDataWriter`.
The data are written to the bus by calling the :cpp:func:`fep3::core::arya::DataReader::flushNow`.
If the capacity of the DataWriter
queue exceeded before it is flushed, the oldest sample will be removed.

The DataWriter can be used to write data with the streaming operator<< for
any :cpp:class:`fep3::core::arya::IDataSample` implementation,
but also for some other standard and class types.
It is designed for your convenience! If the :cpp:func:`fep3::core::arya::addToComponents` was used
it will automatically set the right timestamp to the data sample.
The streaming operator can be used for following data types:

+--------------------------------+--------------------------------------+
| Type                           | Recommended StreamType to use        |
+================================+======================================+
| IDataSample                    | <any>                                |
+--------------------------------+--------------------------------------+
| int8_t, uint8_t,               | StreamTypePlain<pain_c_type>         |
| int16_t, uint16_t,             |                                      |
| int32_t, uint32_t,             |                                      |
| int64_t, uint64_t,             |                                      |
| float, double                  |                                      |
+--------------------------------+--------------------------------------+
| std::string                    | StreamTypeString                     |
+--------------------------------+--------------------------------------+
| structured (standard layout)   | StreamTypeDDL                        |
+--------------------------------+--------------------------------------+
| structured data array          | StreamTypeDDLArray                   |
| (standard layout)              |                                      |
| use fep3::StdVectorSampleType  |                                      |
+--------------------------------+--------------------------------------+

.. _label_core_data_reader:

DataReader
~~~~~~~~~~

DataReader are queues that will hold the reading data samples from the corresponding
:cpp:class:`fep3::arya::IDataRegistry::IDataReader`.
The reading is triggered by a :cpp:func:`fep3::core::arya::DataReader::receiveNow` call.
This will read the data from the :cpp:class:`fep3::arya::IDataRegistry::IDataReader`
only before the given simulation time and keep it in the queue. If the capacity of the DataReader
queue exceeded, the oldest sample will be removed.

To read the DataReaders content the streaming operator >> may be used for
any :cpp:class:`fep3::core::arya::IDataSample` implementation as well as for some other standard
and class types.
See table in chapter :ref:`label_core_data_writer` which types are possible.

The streaming operator >> returns the latest sample available. All options to retrieve samples
from a data reader are listed in the following table:

+--------------------------------------------------------------------------+--------------------------------------------------------+
| Data Reader Access                                                       | Description                                            |
+==========================================================================+========================================================+
| :cpp:func:`fep3::core::arya::DataReader::>>`                             | Returns and pops the latest data sample.               |
+--------------------------------------------------------------------------+--------------------------------------------------------+
| :cpp:func:`fep3::core::arya::DataReader::popSampleLatest`                | Pops the latest data samples and returns it.           |
+--------------------------------------------------------------------------+--------------------------------------------------------+
| :cpp:func:`fep3::core::arya::DataReader::popSampleOldest`                | Pops the oldest data samples and returns it.           |
+--------------------------------------------------------------------------+--------------------------------------------------------+
| :cpp:func:`fep3::core::arya::DataReader::readSampleLatest`               | Returns the latest data sample without popping it.     |
+--------------------------------------------------------------------------+--------------------------------------------------------+
| :cpp:func:`fep3::core::arya::DataReader::readSampleOldest`               | Returns the oldest data sample without popping it.     |
+--------------------------------------------------------------------------+--------------------------------------------------------+
| :cpp:func:`fep3::core::arya::DataReader::readSampleBefore`               | Returns the oldest data sample with a timestamp below  |
|                                                                          | a given upper bound timestamp without popping it.      |
+--------------------------------------------------------------------------+--------------------------------------------------------+
| :cpp:func:`fep3::core::arya::DataReader::readType`                       | Returns the current stream type.                       |
+--------------------------------------------------------------------------+--------------------------------------------------------+

.. _label_core_job:

Adding Jobs
-----------

Jobs are small runnable units to implement a functionalit that needs to be part of a distributed scheduler.
At the moment jobs configurations are only possible for cyclic time triggered events. In future there might
be a possiblity to add data triggered jobs as well.

Use the :cpp:func:`fep3::core::arya::addToComponents` to add a job to the :ref:`label_job_registry`.
Use the :cpp:func:`fep3::core::arya::removeFromComponents` to remove the job from the :ref:`label_job_registry`.

See :ref:`label_demo_core_easy` for an example to use the core Jobs.

The :cpp:func:`fep3::arya::IJob::executeDataIn` is to read
and prepare the incoming data for the *execute* call. Do *not* implement any functional behavior in here,
but read all data from the IDataRegistry::IDataReader.

The :cpp:func:`fep3::arya::IJob::execute` is to implement the jobs functional behavior.

The :cpp:func:`fep3::arya::IJob::executeDataOut` is to flush the
:cpp:class:`fep3::core::arya::DataWriter` queues.


.. _label_cpp_api:

FEP Interface CPP API
_____________________

.. _label_cpp_cmake:

FEP Interface CPP CMake Config
------------------------------

The FEP Interface CPP API comes with its own target and can be used as follows:

::

  # find the package
  find_package(fep3_participant_cpp REQUIRED)

  # ... define your target and other stuff

  # link against the fep3_participant_cpp
  target_link_libraries(my_target PRIVATE
                        fep3_participant_cpp)


.. _label_cpp_participant:

FEP Interface CPP Participant
-----------------------------

Use :cpp:func:`fep3::cpp::arya::createParticipant` to create and use a
FEP Participant.

See :ref:`label_demo_cpp_easy` for an example how to use the
:cpp:class:`fep3::cpp::arya::Participant`.

.. _label_cpp_element_base_datajob:

DataJob Element
---------------

See :ref:`label_demo_cpp_easy` for an example how to use the
:cpp:class:`fep3::cpp::arya::ElementBase`.

The :cpp:class:`fep3::cpp::arya::DataJobElement` is a convenience class that
implements the :ref:`label_element_interface` and supports one :ref:`label_cpp_datajob`.

.. _label_cpp_datajob:

DataJob
-------

The DataJob is a FEP Interface CPP programming model to implement a functionality in a easy way.
See :ref:`label_demo_cpp_easy` for an example how to use the
:cpp:class:`fep3::cpp::arya::DataJob`.



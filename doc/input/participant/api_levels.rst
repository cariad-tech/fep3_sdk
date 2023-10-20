.. Copyright @ 2022 VW Group. All rights reserved.
..
.. This Source Code Form is subject to the terms of the Mozilla 
.. Public License, v. 2.0. If a copy of the MPL was not distributed 
.. with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
.. _Platform Notes:

==========
API Levels 
==========


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
:cpp:class:`fep3::core::ElementBase`.

The :cpp:class:`fep3::core::ElementBase` is a convenience class that
implements the :ref:`label_element_interface`. It handles the type name and the version
information of a FEP Element (see :cpp:func:`fep3::core::ElementBase::getTypename`,
:cpp:func:`fep3::core::ElementBase::getVersion`) and provides access to
:term:`FEP Components<FEP Component>` via :cpp:func:`fep3::core::ElementBase::getComponents`.
Calls to :cpp:func:`fep3::base::IElement::loadElement` in the context of the
:term:`FEP Participant`'s :ref:`label_participant_state_machine` are tranferred to calls to
:cpp:func:`fep3::core::ElementBase::load` (:cpp:func:`fep3::base::IElement::unloadElement`
to :cpp:func:`fep3::core::ElementBase::unload` respectively).

.. _label_core_element_configurable:

Element Configurable
--------------------

The :cpp:class:`fep3::core::ElementConfigurable` base class provides a convenience class
to use the :cpp:class:`fep3::arya::IConfigurationService`.

See :ref:`label_demo_core_easy` for an example how to use the :cpp:class:`fep3::base::Configuration`.

.. _label_core_easy_logging:

Easy Logging
------------

The :cpp:class:`fep3::base::EasyLogging` base class provides a convenience class and macros
to use the :cpp:class:`fep3::arya::ILoggingService`. If more customization is required
the :ref:`label_logging_service` needs to be used, which allows multiple loggers,
logger grouping and custom logging sinks.

See :ref:`label_demo_core_easy` for an example how to use the
:cpp:class:`fep3::base::EasyLogging`.

If :cpp:class:`fep3::core::ElementBase` or :cpp:class:`fep3::core::ElementConfigurable`
is used then :cpp:class:`fep3::base::EasyLogging` is already included and does not need to
be initialized. To filter the logs from these two classes the logger name "element" has to be used.

.. _label_core_data_writer_data_reader:

Core DataWriter and DataReader
------------------------------

See :ref:`label_demo_core_easy` for an example how to use the core *DataWriter* and *DataReader*.

The :cpp:class:`fep3::core::arya::DataWriter` and :cpp:class:`fep3::core::arya::DataReader`
are helper classes to add and remove :cpp:class:`fep3::core::arya::IDataRegistry::IDataReader` and
:cpp:class:`fep3::core::arya::IDataRegistry::IDataWriter`.

Use the :cpp:func:`fep3::core::arya::addToComponents` to add Reader or Writer to the :ref:`label_data_registry`.
Use the :cpp:func:`fep3::core::arya::removeFromComponents` to remove Reader or Writer from the :ref:`label_data_registry`.

.. _label_core_data_writer:

DataWriter
~~~~~~~~~~

*DataWriter* are queues that will hold the written data samples before they are flushed to the
corresponding :cpp:class:`fep3::arya::IDataRegistry::IDataWriter`.
The data are written to the bus by calling the :cpp:func:`fep3::core::arya::DataReader::flushNow`.
If the capacity of the *DataWriter*
queue exceeded before it is flushed, the oldest sample will be removed.

The *DataWriter* can be used to write data with the streaming operator<< for
any :cpp:class:`fep3::core::arya::IDataSample` implementation,
but also for some other standard and class types.
It is designed for the users convenience! If the :cpp:func:`fep3::core::arya::addToComponents`
was used it will automatically set the right timestamp to the data sample.
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

*DataReader* are queues that will hold the reading data samples from the corresponding
:cpp:class:`fep3::arya::IDataRegistry::IDataReader`.
The reading is triggered by a :cpp:func:`fep3::core::arya::DataReader::receiveNow` call.
This will read the data from the :cpp:class:`fep3::arya::IDataRegistry::IDataReader`
only before the given simulation time and keep it in the queue. If the capacity of the *DataReader*
queue exceeded, the oldest sample will be removed.

To read the *DataReaders* content the streaming operator >> may be used for
any :cpp:class:`fep3::core::arya::IDataSample` implementation as well as for some other standard
and class types.
See table in chapter :ref:`label_core_data_writer` which types are possible.

The streaming operator >> returns the latest sample available. 

All options to retrieve samples from a data reader are listed in the following table:

+--------------------------------------------------------------------+--------------------------------------------------------------+
| Data Reader Access                                                 | Description                                                  |
+====================================================================+==============================================================+
| :cpp:func:`fep3::core::arya::DataReader::popSampleLatest`          | Pops the latest data samples and returns it.                 |
+--------------------------------------------------------------------+--------------------------------------------------------------+
| :cpp:func:`fep3::core::arya::DataReader::popSampleOldest`          | Pops the oldest data samples and returns it.                 |
+--------------------------------------------------------------------+--------------------------------------------------------------+
| :cpp:func:`fep3::core::arya::DataReader::readSampleLatest`         | Returns the latest data sample without popping it.           |
+--------------------------------------------------------------------+--------------------------------------------------------------+
| :cpp:func:`fep3::core::arya::DataReader::readSampleOldest`         | Returns the oldest data sample without popping it.           |
+--------------------------------------------------------------------+--------------------------------------------------------------+
| :cpp:func:`fep3::core::arya::DataReader::readSampleBefore`         | Returns the oldest data sample with a timestamp below        |
|                                                                    | a given upper bound timestamp without popping it.            |
+--------------------------------------------------------------------+--------------------------------------------------------------+
| :cpp:func:`fep3::core::arya::DataReader::purgeAndPopSampleBefore`  | Pops and returns the oldest sample with a timestamp below an |
|                                                                    | upper bound timestamp and removes older samples from queue.  |
+--------------------------------------------------------------------+--------------------------------------------------------------+
| :cpp:func:`fep3::core::arya::DataReader::readType`                 | Returns the current stream type.                             |
+--------------------------------------------------------------------+--------------------------------------------------------------+

Usage of purgeAndPopSampleBefore
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The samples in the title of the Scenarios are named after their simulation timestamp.
 - tnow means "current cycle"
 - tnow-tc means "last cycle"
 - tnow+tc means "next cycle"
 - tnow-2tc means "next to last cycle"

"Take" means "use an remove from buffer".

For all scenarios, the timestamp used as parameter is tnow-tc.
Scenario 1: One tnow-tc sample in buffer.
    Result: Take sample tnow-tc. Buffer is empty.

Scenario 2: One tnow-tc and one tnow-2tc sample in buffer.
    Result: Take sample tnow-tc and remove sample tnow-2tc from queue. Buffer is empty.

Scenario 3: One tnow-tc and one tnow sample in buffer.
    Result: Take sample tnow-tc. One sample remains in buffer.

Scenario 4: One tnow-2tc, One tnow-tc and one tnow sample in buffer.
    Result: Take sample tnow-tc, remove older samples from queue. One sample remains in buffer.

Scenario 5: One tnow sample in buffer.
    Result: a null pointer for 'no result'. One sample remains in buffer.

Scenario 6: No sample in buffer.
    Result: a null pointer for 'no result'. Buffer is empty.

.. _label_core_job:

Adding Jobs
-----------
Use the :cpp:func:`fep3::core::arya::addToComponents` to add a job to the :ref:`label_job_registry`.
Use the :cpp:func:`fep3::core::arya::removeFromComponents` to remove the job from the :ref:`label_job_registry`.

See :ref:`label_demo_core_easy` for an example to use the core Jobs.

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
:cpp:class:`fep3::base::Participant`.

.. _label_cpp_element_base_datajob:

DataJob Element
---------------

See :ref:`label_demo_cpp_easy` for an example how to use the
:cpp:class:`fep3::core::ElementBase`.

The :cpp:class:`fep3::cpp::DataJobElement` is a convenience class that
implements the :ref:`label_element_interface` and supports one :ref:`label_cpp_datajob`.

.. _label_cpp_datajob:

DataJob
-------

The *DataJob* is a FEP Interface CPP programming model to implement a functionality in an easy way.
See :ref:`label_demo_cpp_easy` for an example how to use the
:cpp:class:`fep3::cpp::DataJob`.

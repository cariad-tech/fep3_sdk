.. Copyright 2023 CARIAD SE.
.. 
.. This Source Code Form is subject to the terms of the Mozilla 
.. Public License, v. 2.0. If a copy of the MPL was not distributed 
.. with this file, You can obtain one at https://mozilla.org/MPL/2.0/.


.. highlight:: cpp

.. _label_data_registry:

=============
Data Registry
=============

.. sidebar:: Data Registry

        .. contents::

Summary
=======

+------------------------------------------------------+-----------------------------------------------------------------+
| Name                                                 |  Data Registry                                                  |
+------------------------------------------------------+-----------------------------------------------------------------+
| Component Interface                                  |  :cpp:class:`fep3::arya::IDataRegistry`                         |
+------------------------------------------------------+-----------------------------------------------------------------+
| RPC Service Interface                                |  :cpp:class:`fep3::rpc::arya::IRPCDataRegistryDef`              |
|                                                      |  :cpp:class:`fep3::rpc::arya::IRPCDataRegistry`                 |
+------------------------------------------------------+-----------------------------------------------------------------+
| RPC Service Description                              |  :ref:`label_rpc_data_registry`                                 |
+------------------------------------------------------+-----------------------------------------------------------------+
| native delivery                                      |  CPP-plugin                                                     |
+------------------------------------------------------+-----------------------------------------------------------------+
| CPP-plugin possible                                  |  yes                                                            |
+------------------------------------------------------+-----------------------------------------------------------------+
| C-plugin possible                                    |  no                                                             |
+------------------------------------------------------+-----------------------------------------------------------------+

Component Interface
~~~~~~~~~~~~~~~~~~~

The :cpp:class:`fep3::arya::IDataRegistry` interface offers functionality to register data in and data out points,
register data receive listener for data triggered reception and get reader and writer objects from the simulation bus.

RPC Service Interfaces
~~~~~~~~~~~~~~~~~~~~~~

The :cpp:class:`fep3::rpc::arya::IRPCDataRegistryDef` interface offers functionality to get a vector of all input or output signals that are registered
and the Stream Type of a particular signal with all its properties.

Overview
========

The *DataRegistry* is responsible for registering data in access and data out access of the element implementation.
The container to write and read data is :cpp:class:`fep3::arya::IDataSample` .
This data is classified via the :cpp:class:`fep3::arya::IStreamType` .

The main functionality of the data registry is to add the data ins and outs to the simulation bus at the correct initialization time.
Data ins and outs can be added to the :ref:`label_data_registry` when the :ref:`label_element_interface` is

* loading (:cpp:func:`fep3::base::IElement::loadElement`)
* or initializing (:cpp:func:`fep3::base::IElement::initialize`).

After that signals can still be added but will only be registered at the simulation bus upon the next initialization (call to :cpp:func:`fep3::arya::IComponent::initialize`).
Usually each data in and data out is named and pre-classified (see :cpp:func:`fep3::arya::IDataRegistry::registerDataIn`, :cpp:func:`fep3::arya::IDataRegistry::registerDataOut`).

.. literalinclude:: ../snippets/snippet_data_registry.cpp
    :start-after: //Begin(register data signal)
    :end-before: //End(register data signal)

.. _label_data_samples:

Data Samples
============

Data Samples (see also :cpp:class:`fep3::arya::IDataSample`) are plain raw memory containers with 3 important information:

* time (in nano seconds) - see :cpp:func:`fep3::arya::IDataSample::getTime`
* size (in bytes) - see :cpp:func:`fep3::arya::IDataSample::getSize`
* counter (counting at writers time) - see :cpp:func:`fep3::arya::IDataSample::getCounter`

These samples are written to a writer queue using simulation bus memory with an instance of :cpp:class:`fep3::arya::IDataRegistry::IDataWriter` (see :cpp:func:`fep3::arya::IDataRegistry::getWriter`).
Samples are read from a queue through an instance of :cpp:class:`fep3::arya::IDataRegistry::IDataReader` (see :cpp:func:`fep3::arya::IDataRegistry::getReader`).

Data Writer / Data Reader
~~~~~~~~~~~~~~~~~~~~~~~~~

To read and write samples from or to the simulation bus the :cpp:class:`fep3::arya::IDataRegistry::IDataReader` and :cpp:class:`fep3::arya::IDataRegistry::IDataWriter` classes from the data registry should be used.
This ensures that the corresponding signal is registered and prohibits invalid pointer access. A data reader or writer from the data registry is a nullptr if a signal is not registered with DataRegistry::addDataIn
beforehand and function calls will return an error if they are not valid.

To get the data reader or data writer object either call :cpp:func:`fep3::arya::IDataRegistry::getReader` / :cpp:func:`fep3::arya::IDataRegistry::getWriter` or
use the :cpp:func:`fep3::arya::addDataIn` / :cpp:func:`fep3::arya::addDataOut` functions which also register the signal at the same time:

.. literalinclude:: ../snippets/snippet_data_registry.cpp
    :start-after: //Begin(get data reader)
    :end-before: //End(get data reader)

Is equivalent to:

.. literalinclude:: ../snippets/snippet_data_registry.cpp
    :start-after: //Begin(add data in)
    :end-before: //End(add data in)

.. _label_default_streamtypes:

Default Stream Types
====================

Current FEP 3 support
~~~~~~~~~~~~~~~~~~~~~

The following list of Stream Meta Types is natively supported in FEP3 by the referenced Stream Type classes:

+-----------------------------------------------+------------------------------------------------------------------------+----------------------------------------------------+
| Stream Meta Type name                         | Description                                                            | Stream Type class implementing                     |
|                                               |                                                                        | the Stream Meta Type                               |
+===============================================+========================================================================+====================================================+
| meta_type_ddl                                 | Main use case. For structured memory types which are described by DDL. | :cpp:class:`fep3::arya::StreamTypeDDL`             |
|                                               | The description is shipped within a Stream Type property.              |                                                    |
+-----------------------------------------------+------------------------------------------------------------------------+----------------------------------------------------+
| meta_type_ddl_fileref                         | For structured memory types which are described by DDL.                | :cpp:class:`fep3::arya::StreamTypeDDLFileRef`      |
|                                               | The description has to be loaded from a file.                          |                                                    |
+-----------------------------------------------+------------------------------------------------------------------------+----------------------------------------------------+
| meta_type_ddl_array                           | For structured array memory types                                      | :cpp:class:`fep3::arya::StreamTypeDDLArray`        |
+-----------------------------------------------+------------------------------------------------------------------------+----------------------------------------------------+
| meta_type_ddl_array_fileref                   | For structured array memory types                                      | :cpp:class:`fep3::arya::StreamTypeDDLArrayFileRef` |
+-----------------------------------------------+------------------------------------------------------------------------+----------------------------------------------------+
| meta_type_raw                                 | For unspecified data only the elements itself know the content         | :cpp:class:`fep3::arya::StreamTypeRaw`             |
+-----------------------------------------------+------------------------------------------------------------------------+----------------------------------------------------+
| meta_type_plain                               | For plain c-type based samples                                         | :cpp:class:`fep3::arya::StreamTypePlain`           |
+-----------------------------------------------+------------------------------------------------------------------------+----------------------------------------------------+
| meta_type_plain_array                         | For plain c-type arrays                                                | :cpp:class:`fep3::arya::StreamTypePlainArray`      |
+-----------------------------------------------+------------------------------------------------------------------------+----------------------------------------------------+
| meta_type_string                              | For dynamic string data                                                | :cpp:class:`fep3::arya::StreamTypeString`          |
+-----------------------------------------------+------------------------------------------------------------------------+----------------------------------------------------+
| meta_type_video                               | For video data                                                         | not implemented                                    |
+-----------------------------------------------+------------------------------------------------------------------------+----------------------------------------------------+
| meta_type_audio                               | For audio data                                                         | not implemented                                    |
+-----------------------------------------------+------------------------------------------------------------------------+----------------------------------------------------+
| Other :cpp:class:`fep3::arya::StreamMetaType` | User types which are described by the user but are unspecified         | :cpp:class:`fep3::arya::StreamTypeDDLFileRef`      |
|                                               | for the simulation bus                                                 |                                                    |
+-----------------------------------------------+------------------------------------------------------------------------+----------------------------------------------------+

It depends on the simulation bus implementation used which Meta Type is supported.
The default Stream Meta Types and Stream Type helper classes are defined in the file fep3/base/stream_type/default_stream_type.h

Native Implementations
======================

The native data registry implementation offers following functionality which is not part the of component interface :cpp:class:`fep3::arya::IDataRegistry`.

.. _label_signal_renaming:

Signal Renaming
~~~~~~~~~~~~~~~

Signals registered at the native :ref:`label_data_registry` may be renamed using the :ref:`label_configuration_service` and its properties without having to adapt code or recompile participants.
Renaming a signal adds an alias name for the signal which will be used for registration at the simulation bus and for RPC requests.
Renamed signals are not available at the simulation bus via their original name and the original names are not considered if available signals or stream types for specific signals are requested via RPC.
Alias names have to be unique and are applied when the :ref:`label_data_registry` is initializing (:cpp:func:`fep3::arya::IComponent::initialize()`).
To reset a signal name, the entry in the corresponding signal renaming property can be deleted and the participant reinitialized.
The signal will use its original name instead of the previously configured alias. 

Properties
----------

Following properties may be used to rename signals.

.. list-table::
   :widths: 20 60 20
   :header-rows: 1

   * - Name
     - Code Macro
     - Default Value
   * - renaming_input
     - :c:macro:`FEP3_DATA_REGISTRY_SIGNAL_RENAMING_INPUT_CONFIGURATION`
     - ""
   * - renaming_output
     - :c:macro:`FEP3_DATA_REGISTRY_SIGNAL_RENAMING_OUTPUT_CONFIGURATION`
     - ""

Usage
-----

The renaming properties use following scheme to derive names of signals to be renamed and the corresponding name aliases:

.. code-block:: html

  "old_name_signal_1:new_name_signal_1,old_name_signal_2:new_name_signal_2"

To rename an existing signal the corresponding property has to be set, stating the old signal name to be renamed and the new signal alias, both
divided by a colon. Multiple signals may be renamed by providing a list of old/new signal name pairs separated by commas.

E.g. to rename two input signals named 'old_in_1' and 'old_in_2' to 'new_in_1' and 'new_in_2' and two output signals named 'old_out_1' and
'old_out_2' to 'new_out_1' and 'new_out_2', the properties have to be set accordingly:


.. list-table::
   :widths: 20 20 20
   :header-rows: 1

   * - Property
     - Value
     - Description
   * - :c:macro:`FEP3_DATA_REGISTRY_SIGNAL_RENAMING_INPUT_CONFIGURATION`
     - "old_in_1:new_in_1,old_in_2:new_in_2"
     - Input signal 'old_in_1' is renamed to 'new_in_1' and input signal 'old_in_2' is renamed to 'new_in_2'. 
   * - :c:macro:`FEP3_DATA_REGISTRY_SIGNAL_RENAMING_OUTPUT_CONFIGURATION`
     - "old_out_1:new_out_1,old_out_2:new_out_2"
     - Output signal 'old_out_1' is renamed to 'new_out_1' and output signal 'old_out_2' is renamed to 'new_out_2'.

.. note::
    Signal renaming properties :c:macro:`FEP3_DATA_REGISTRY_SIGNAL_RENAMING_INPUT_CONFIGURATION` and :c:macro:`FEP3_DATA_REGISTRY_SIGNAL_RENAMING_OUTPUT_CONFIGURATION` have to be set
    before the :ref:`label_data_registry` is initializing (:cpp:func:`fep3::arya::IComponent::initialize()`) when signal renaming properties are parsed and signal renaming is applied.

Interaction with Signal Mapping
-------------------------------

While signal renaming may be used in combination with signal mapping, most use cases should be covered by one of those two features already.
If special use cases require usage of both features, renaming of input signals happens before mapping and the resulting signal flow looks as follows:

.. code-block:: html

  Incoming signal -> Renaming -> Registered source signal -> Mapping -> Registered target signal -> Job process method

This means signal renaming is applied for source signals of :ref:`label_data_registry` 's mapping feature only.

.. note::
  A special use case which requires usage of signal renaming and signal mapping might e.g. be the need to map a source signal to a target signal of the same name which is not possible using signal mapping only.

Signal renaming for FEP Participant Developers
----------------------------------------------

Signal renaming and the corresponding signal name aliases are relevant for FEP System Developers only as they are applied after registration of signals and considered by the :ref:`label_simulation_bus` and the RPC interface :ref:`label_rpc_data_registry` later on. 
FEP Participant Developers using the :ref:`label_data_registry` component interface :cpp:class:`fep3::arya::IDataRegistry` have to use original signal names instead of signal names aliases.

.. note::
  FEP Participant Developers should have in mind that signal names can be renamed later on.

.. _label_signal_mapping:

Signal Mapping
~~~~~~~~~~~~~~

Incoming signals may be mapped onto new signals using the DDL's Signal Mapping functionality which is integrated into the native :ref:`label_data_registry` component.
Signals available within the FEP System can be dissolved and reassembled on a DDL struct and even element level.
The actual mapping of signals is configured using a xml-based configuration format. 
For further details regarding the DDL Signal Mapping functionality, the configuration format and an example, please have a look at the Signal Mapping Format Specification:

.. _Signal Mapping Format Specification: https://github.com/cariad-tech/dev_essential/blob/main/doc/extern/ddl/mapping_specification.md

For further details regarding the DDL specification, please have a look at:

.. _DDL Specification: https://github.com/cariad-tech/dev_essential/blob/main/doc/extern/ddl/ddl_specification.md

Properties
----------

Following properties may be used to configure Signal Mapping within the native :ref:`label_data_registry` component.

.. list-table::
   :widths: 20 60 20
   :header-rows: 1

   * - Name
     - Code Macro
     - Default Value
   * - mapping_configuration_file_path
     - :c:macro:`FEP3_DATA_REGISTRY_MAPPING_CONFIGURATION_FILE_PATH`
     - ""
   * - mapping_ddl_file_paths
     - :c:macro:`FEP3_DATA_REGISTRY_MAPPING_DDL_FILE_PATHS`
     - ""

The mapping configuration file path property shall contain a path to a mapping configuration file which contains meta information about the file and further information regarding
source signal declarations, target signal definitions and whether to apply transformations to specific signals during the mapping process.

The mappping ddl file paths property shall contain a list of semicolon-separated paths to ddl description files which will be used during the mapping process e.g. for registration of source signals.
Relative paths will be resolved starting from the directory of the FEP Components plugin, which contains the native :ref:`label_data_registry` implementation.
The property has to be set before a mapped signal is registered as the property will be evaluated during registration of mapped signals.
If problems arise while parsing the ddl description files, errors are logged by the :ref:`label_logging_service` and returned within code.

.. note::
   The input signals for a certain mapping can be registerd as data readers as well.
   It is not neccessary to have a specific order to register both input data and its mapped data as data readers in one participant.

Problems may concern invalid or not existing ddl description files or merge conflicts between already registered ddls and a ddl description to be added.
Merge conflicts occur if a datatype of name x has been registered at the native data registry and another datatype of the same name is part of a ddl description file provided via the mapping ddls file paths property.

.. note::
    Merging structs of the same name results will throw an exception.

Inhomogeneous source signal update rates
----------------------------------------

The mapping feature provides functionality to combine source signals into new combinations of target signals.
Depending on the mapping configuration either parts of a source signal or the signal as a whole is used to create the mapped target signal.
If a mapped target signal consists of multiple source signals, one of the source signals has to be configured to act as trigger for the forwarding of the target signal.
The trigger signal can be configured using the corresponding tag of the mapping configuration:

.. code-block:: xml

  <trigger type="signal" variable="source_signal_b" />

As long as all source signals are available at trigger time, the corresponding source signals will be used to create the target signal.
If source signals show inhomogeneous update rates, a target signal will only be triggered and therefore receive a sample if the source signal configured as trigger receives a sample.

If we assume a target signal C is created from source signals A and B and source signal B is configured to be the trigger for target signal C, we can observe following behaviour:

If only a sample for the source signal A, which is not configured to trigger target signal C, is received, the target sample will not be created and target signal C will not be triggered.
The sample of source signal A will be stored and used once the trigger source signal B receives a sample.
Once source signal B is triggered, a sample for target signal C will be created using source signals A and B, as configured within the mapping configuration, and forwarded to target signal C and its data receivers.

Samples received for source signals are stored and used until a new sample for the corresponding trigger source signal is received:
If two samples for source signal B are received while no new sample for source signal A is received, the latest sample received for source signal A is used to create the samples for target signal C.

Samples overwritten by new samples without being used for creation of target samples are ignored:
If two samples for source signal A are received without source signal B receiving a sample and therefore triggering creation of a sample for target signal C, the first sample received for source signal A will be overwritten by the second sample received.
The first sample received will never be used for creation of a target sample and therefore is lost.

FEP Data Registry over RPC
==========================

Information regarding registered input/output signals and their stream types can be accessed from :term:`RPC Service` "data_registry".
The examples are using the command line of :term:`FEP Control` which you find in fep_sdk_base_utilities/3.1.0.

.. note::
   Code snippets within '<>' have to be replaced according to the actual use case.

In order to receive the list of registered input signals for a :term:`FEP Participant` <fep_participant_name> which is part of a :term:`FEP System` <fep_system_name>, use:

.. code-block:: console

    callRPC <fep_system_name> <fep_participant_name> data_registry data_registry.arya.fep3.iid getSignalInNames

In order to receive the list of registered output signals, use:

.. code-block:: console

    callRPC <fep_system_name> <fep_participant_name> data_registry data_registry.arya.fep3.iid getSignalOutNames

In order to receive the stream type for a specific signal name <signal_name>, the following RPC call can be used:

.. code-block:: console

    callRPC <fep_system_name> <fep_participant_name> data_registry data_registry.arya.fep3.iid getStreamType '{"signal_name" : "<signal_name>"}'

.. note::
   In order to retrieve values for RPC requests, the corresponding signals must already have been registered by the :term:`FEP Participant`.

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
| native delivery                                      |  built-in                                                       |
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

* loading (:cpp:func:`fep3::arya::IElement::loadElement`)
* or initializing (:cpp:func:`fep3::arya::IElement::initialize`).

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
   * - InputSignalRenaming
     - :c:macro:`FEP3_DATA_REGISTRY_SIGNAL_RENAMING_INPUT_CONFIGURATION`
     - ""
   * - OutputSignalRenaming
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
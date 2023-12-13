.. Copyright @ 2021 VW Group. All rights reserved.
.. 
.. This Source Code Form is subject to the terms of the Mozilla 
.. Public License, v. 2.0. If a copy of the MPL was not distributed 
.. with this file, You can obtain one at https://mozilla.org/MPL/2.0/.


.. |br| raw:: html

  <br/>

.. highlight:: cpp

.. _label_guide_migration:

====================================================
How to migrate a FEP Participant from FEP 2 to FEP 3
====================================================

This guide shows how to port a FEP 2 participant to a FEP 3 participant,
how the :ref:`components<Default_Components_vs_Foreign_Components>` and features have changed and it offers specific code
examples for both versions.

Instantiating a FEP Participant
===============================

In FEP 2 the term module was ambiguous and could either refer to the FEP Participant or the FEP Element.
In FEP 3 the term module is therefore not being used anymore and the terms
:term:`FEP Participant` and :term:`FEP Element` are now clearly defined.

For a basic :term:`FEP Participant` it is recommended to use the
:ref:`CPP Library<FEP_Participant_Library>` first since it offers a more simple and convenient method to
create a FEP Participant. The :ref:`Core Library<label_core_api>` offers more functionality but one
has to write the :term:`FEP Element` himself.

+--------------------------------------------------------------------------------+--------------------------------------------------------------------------------+
| FEP 2 Implementation                                                           | FEP 3 Implementation                                                           |
+--------------------------------------------------------------------------------+--------------------------------------------------------------------------------+
| .. literalinclude:: ../../snippets/snippet_migration_guide_fep_2.cpp           | **Variant 1: CPP Library**                                                     |
|     :start-after: //Begin(Instantiation)                                       |                                                                                |
|     :end-before: //End(Instantiation)                                          | Create a custom class that is derived from :ref:`label_cpp_datajob` to         |
|     :dedent: 4                                                                 | implement the functionality of the FEP Element (*MyDataJob* in this example).  |
|                                                                                |                                                                                |
|                                                                                | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_3.cpp           |
|                                                                                |     :start-after: //Begin(CppInstantiation)                                    |
|                                                                                |     :end-before: //End(CppInstantiation)                                       |
|                                                                                |     :dedent: 4                                                                 |
|                                                                                |                                                                                |
|                                                                                | **Variant 2: Core Library**                                                    |
|                                                                                |                                                                                |
|                                                                                | Create a custom class that is derived from :ref:`label_core_element_base` or   |
|                                                                                | :ref:`label_core_element_configurable` (allows custom properties) to modify    |
|                                                                                | the FEP Element itself. (*MyElement* in this example)                          |
|                                                                                |                                                                                |
|                                                                                | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_3.cpp           |
|                                                                                |     :start-after: //Begin(CoreInstantiation)                                   |
|                                                                                |     :end-before: //End(CoreInstantiation)                                      |
|                                                                                |     :dedent: 4                                                                 |
+--------------------------------------------------------------------------------+--------------------------------------------------------------------------------+

Writing an FEP Element
======================

The :term:`FEP Element` implementation replaces the module and gets loaded by the
FEP Participant during the state transition *load*. It communicates with the FEP Participant via the
:ref:`component interfaces<Default_Components_vs_Foreign_Components>`.



+------------------------------+--------------------------------------------------------------------------------+--------------------------------------------------------------------------------+
|                              | FEP 2 Implementation                                                           | FEP 3 Implementation                                                           |
+------------------------------+--------------------------------------------------------------------------------+--------------------------------------------------------------------------------+
| **Class declaration**        | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_2.cpp           | Derive from :ref:`label_core_element_base`.                                    |
|                              |     :start-after: //Begin(Declaration)                                         |                                                                                |
|                              |     :end-before: //End(Declaration)                                            | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_3.cpp           |
|                              |                                                                                |     :start-after: //Begin(Declaration)                                         |
|                              |                                                                                |     :end-before: //End(Declaration)                                            |
|                              |                                                                                |                                                                                |
|                              |                                                                                | To create and use custom properties :ref:`label_core_element_configurable`     |
|                              |                                                                                | has to be used as base class instead. But access to the                        |
|                              |                                                                                | :ref:`label_configuration_service` (formerly PropertyTree)                     |
|                              |                                                                                | is available to both base classes.                                             |
+------------------------------+--------------------------------------------------------------------------------+--------------------------------------------------------------------------------+
| **Create method**            | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_2.cpp           | A new state called Unloaded has been added.                                    |
|                              |     :start-after: //Begin(Create)                                              | Formerly the Create() method called ProcessStartupEntry() right away.          |
|                              |     :end-before: //End(Create)                                                 | This does not happen anymore and the FEP Participant will stay in this new     |
|                              |     :dedent: 4                                                                 | state until load() is called. |br|                                             |
|                              |     :caption: class cMyModule                                                  | Use the class constructor for anything the Create() method did before.         |
|                              |                                                                                |                                                                                |
|                              |                                                                                | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_3.cpp           |
|                              |                                                                                |     :start-after: //Begin(Constructor)                                         |
|                              |                                                                                |     :end-before: //End(Constructor)                                            |
|                              |                                                                                |     :dedent: 4                                                                 |
+------------------------------+--------------------------------------------------------------------------------+--------------------------------------------------------------------------------+
| **State transitions**        | The state machine has changed substantially. An overview of the new state machine can be found here: :ref:`label_participant_state_machine` |br|                |
|                              | The state *entry* and *exit* functions have been replaced by state *transition* functions which makes the *tState* parameter obsolete. |br|                     |
|                              | A standalone mode that ignores any state changes does not exist anymore.                                                                                        |
|                              +--------------------------------------------------------------------------------+--------------------------------------------------------------------------------+
|                              | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_2.cpp           | The states Startup and Idle have been replaced by the state Loaded.            |
|                              |     :start-after: //Begin(Load)                                                |                                                                                |
|                              |     :end-before: //End(Load)                                                   | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_3.cpp           |
|                              |     :dedent: 8                                                                 |     :start-after: //Begin(Load)                                                |
|                              |                                                                                |     :end-before: //End(Load)                                                   |
|                              |                                                                                |     :dedent: 4                                                                 |
|                              |                                                                                |     :caption: class MyElement                                                  |
|                              +--------------------------------------------------------------------------------+--------------------------------------------------------------------------------+
|                              | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_2.cpp           | The states Initializing and Ready have been replaced by the state Initialized. |
|                              |     :start-after: //Begin(Initialize)                                          |                                                                                |
|                              |     :end-before: //End(Initialize)                                             | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_3.cpp           |
|                              |     :dedent: 8                                                                 |     :start-after: //Begin(Initialize)                                          |
|                              |                                                                                |     :end-before: //End(Initialize)                                             |
|                              |                                                                                |     :dedent: 4                                                                 |
|                              |                                                                                |     :caption: class MyElement                                                  |
|                              +--------------------------------------------------------------------------------+--------------------------------------------------------------------------------+
|                              | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_2.cpp           | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_3.cpp           |
|                              |     :start-after: //Begin(Run)                                                 |     :start-after: //Begin(Run)                                                 |
|                              |     :end-before: //End(Run)                                                    |     :end-before: //End(Run)                                                    |
|                              |     :dedent: 8                                                                 |     :dedent: 4                                                                 |
|                              |                                                                                |     :caption: class MyElement                                                  |
|                              |                                                                                |                                                                                |
|                              |                                                                                |                                                                                |
|                              +--------------------------------------------------------------------------------+--------------------------------------------------------------------------------+
|                              | It is not possible to go from ``FS_RUNNING`` to ``FS_READY`` in FEP 2.         | In FEP 3 it is not possible to go directly from Running to Loaded.             |
|                              |                                                                                |                                                                                |
|                              | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_2.cpp           | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_3.cpp           |
|                              |     :start-after: //Begin(Stop)                                                |     :start-after: //Begin(Stop)                                                |
|                              |     :end-before: //End(Stop)                                                   |     :end-before: //End(Stop)                                                   |
|                              |     :dedent: 8                                                                 |     :dedent: 4                                                                 |
|                              |                                                                                |     :caption: class MyElement                                                  |
|                              +--------------------------------------------------------------------------------+--------------------------------------------------------------------------------+
|                              | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_2.cpp           | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_3.cpp           |
|                              |     :start-after: //Begin(Deinitialize)                                        |     :start-after: //Begin(Deinitialize)                                        |
|                              |     :end-before: //End(Deinitialize)                                           |     :end-before: //End(Deinitialize)                                           |
|                              |     :dedent: 8                                                                 |     :dedent: 4                                                                 |
|                              |                                                                                |     :caption: class MyElement                                                  |
|                              |                                                                                |                                                                                |
|                              |                                                                                |                                                                                |
|                              +--------------------------------------------------------------------------------+--------------------------------------------------------------------------------+
|                              | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_2.cpp           | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_3.cpp           |
|                              |     :start-after: //Begin(Unload)                                              |     :start-after: //Begin(Unload)                                              |
|                              |     :end-before: //End(Unload)                                                 |     :end-before: //End(Unload)                                                 |
|                              |     :dedent: 8                                                                 |     :dedent: 4                                                                 |
|                              |                                                                                |     :caption: class MyElement                                                  |
|                              | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_2.cpp           |                                                                                |
|                              |     :start-after: //Begin(Unload2)                                             |                                                                                |
|                              |     :end-before: //End(Unload2)                                                |                                                                                |
|                              |     :dedent: 4                                                                 |                                                                                |
|                              +--------------------------------------------------------------------------------+--------------------------------------------------------------------------------+
|                              | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_2.cpp           | The Error state does not exist anymore.                                        |
|                              |     :start-after: //Begin(Error)                                               | A health service is planned to be added with a later minor version.            |
|                              |     :end-before: //End(Error)                                                  |                                                                                |
|                              |     :dedent: 4                                                                 |                                                                                |
|                              |                                                                                |                                                                                |
|                              |                                                                                |                                                                                |
|                              |                                                                                |                                                                                |
+------------------------------+--------------------------------------------------------------------------------+--------------------------------------------------------------------------------+

Reading and Writing Signals
===========================

FEP 3 introduces :ref:`jobs<label_job>` and the :ref:`label_job_registry` which combine the
functionality of the *StepListener* and *DataListener*. The basic :cpp:class:`fep3::cpp::Job`
class only provides a callback function while :cpp:class:`fep3::cpp::DataJob`
(an extension to the Job class) also offers functionality for receiving and sending data.

.. note::
    Any use of the data type int32_t in the code examples is exemplarily for all valid data types.

+------------------------------+--------------------------------------------------------------------------------+--------------------------------------------------------------------------------+
|                              | FEP 2 Implementation                                                           | FEP 3 Implementation                                                           |
+------------------------------+--------------------------------------------------------------------------------+--------------------------------------------------------------------------------+
| **Setup Callback Function**  | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_2.cpp           | The :cpp:class:`fep3::cpp::Job` class can only execute a callback              |
|                              |     :start-after: //Begin(StepListener)                                        | function. If the purpose of the *StepListener* was to send                     |
|                              |     :end-before: //End(StepListener)                                           | :ref:`label_data_samples`, a :cpp:class:`fep3::cpp::DataJob`                   |
|                              |     :dedent: 8                                                                 | is recommended.                                                                |
|                              |                                                                                |                                                                                |
|                              |                                                                                | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_3.cpp           |
|                              |                                                                                |     :start-after: //Begin(Job)                                                 |
|                              |                                                                                |     :end-before: //End(Job)                                                    |
|                              |                                                                                |     :dedent: 8                                                                 |
|                              |                                                                                |     :caption: class MyElement                                                  |
|                              |                                                                                |                                                                                |
|                              |                                                                                | .. note::                                                                      |
|                              |                                                                                |     FEP 3 uses ``std::chrono`` objects now for timestamps and duration.        |
|                              |                                                                                |     This enables simple and type safe casting either explicitly with           |
|                              |                                                                                |     ``std::chrono::duration_cast<>`` or implicitly                             |
|                              |                                                                                |     with ``std::chrono_literals``.                                             |
+------------------------------+--------------------------------------------------------------------------------+--------------------------------------------------------------------------------+
| **Writing a signal**         | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_2.cpp           | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_3.cpp           |
|                              |     :start-after: //Begin(OutputSignal)                                        |     :start-after: //Begin(DataWriter)                                          |
|                              |     :end-before: //End(OutputSignal)                                           |     :end-before: //End(DataWriter)                                             |
|                              |     :dedent: 8                                                                 |     :dedent: 8                                                                 |
|                              |     :caption: class cMyModule                                                  |     :caption: class MyDataJob                                                  |
|                              |                                                                                |                                                                                |
|                              | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_2.cpp           | For an overview of all natively supported stream types                         |
|                              |     :start-after: //Begin(Callback)                                            | see the chapter :ref:`label_default_streamtypes`.                              |
|                              |     :end-before: //End(Callback)                                               |                                                                                |
|                              |     :dedent: 8                                                                 | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_3.cpp           |
|                              |     :caption: pCallbackFunction                                                |     :start-after: //Begin(ProcessWriter)                                       |
|                              |                                                                                |     :end-before: //End(ProcessWriter)                                          |
|                              |                                                                                |     :dedent: 8                                                                 |
|                              |                                                                                |     :caption: MyDataJob::process()                                             |
+------------------------------+--------------------------------------------------------------------------------+--------------------------------------------------------------------------------+
| **Reading a signal**         | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_2.cpp           | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_3.cpp           |
|                              |     :start-after: //Begin(InputSignal)                                         |     :start-after: //Begin(DataReader)                                          |
|                              |     :end-before: //End(InputSignal)                                            |     :end-before: //End(DataReader)                                             |
|                              |     :dedent: 8                                                                 |     :dedent: 8                                                                 |
|                              |     :caption: class cMyModule                                                  |     :caption: class MyDataJob                                                  |
|                              |                                                                                |                                                                                |
|                              | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_2.cpp           | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_3.cpp           |
|                              |     :start-after: //Begin(Update)                                              |     :start-after: //Begin(ProcessReader)                                       |
|                              |     :end-before: //End(Update)                                                 |     :end-before: //End(ProcessReader)                                          |
|                              |     :dedent: 8                                                                 |     :dedent: 8                                                                 |
|                              |     :caption: cMyModule::Update(fep::IUserDataSample* pSample)                 |     :caption: MyDataJob::process()                                             |
+------------------------------+--------------------------------------------------------------------------------+--------------------------------------------------------------------------------+
| **Writing a signal to        | The sending of a DDB signal is almost identical to regular signals in FEP 2.   | The Distributed Data Buffer does not exist anymore.                            |
| the DDB**                    |                                                                                | Instead vector samples can be used now. But the streaming operator             |
|                              | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_2.cpp           | is not defined for vectors, so the data has to be written manually.            |
|                              |     :start-after: //Begin(DDBCallback)                                         |                                                                                |
|                              |     :end-before: //End(DDBCallback)                                            | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_3.cpp           |
|                              |     :dedent: 8                                                                 |     :start-after: //Begin(ProcessVectorOut)                                    |
|                              |     :caption: pCallbackFunction                                                |     :end-before: //End(ProcessVectorOut)                                       |
|                              |                                                                                |     :dedent: 8                                                                 |
|                              |                                                                                |     :caption: MyDataJob::process()                                             |
+------------------------------+--------------------------------------------------------------------------------+--------------------------------------------------------------------------------+
| **Reading a signal from      | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_2.cpp           | As with writing the signal the streaming operator is not defined for vectors,  |
| the DDB**                    |     :start-after: //Begin(SyncListener)                                        | so the data has to be read manually.                                           |
|                              |     :end-before: //End(SyncListener)                                           |                                                                                |
|                              |     :dedent: 8                                                                 | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_3.cpp           |
|                              |     :caption: class cMyModule                                                  |     :start-after: //Begin(ProcessVectorIn)                                     |
|                              |                                                                                |     :end-before: //End(ProcessVectorIn)                                        |
|                              | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_2.cpp           |     :dedent: 8                                                                 |
|                              |     :start-after: //Begin(ProcessDDB)                                          |     :caption: MyDataJob::process()                                             |
|                              |     :end-before: //End(ProcessDDB)                                             |                                                                                |
|                              |     :dedent: 8                                                                 |                                                                                |
|                              |     :caption: cMyModule::ProcessDDBSync(handle_t hSignal,                      |                                                                                |
|                              |                                         fep::IDDBFrame oDDBFrame)              |                                                                                |
+------------------------------+--------------------------------------------------------------------------------+--------------------------------------------------------------------------------+

Properties
==========

The Property Tree has been replaced by the :ref:`label_configuration_service`.
It is now possible to have arrays as properties in the form of std::vector<T>.

.. note::
    Any use of the data type int32_t in the code examples is exemplarily for all valid data types.

+------------------------------+--------------------------------------------------------------------------------+--------------------------------------------------------------------------------+
|                              | FEP 2 Implementation                                                           | FEP 3 Implementation                                                           |
+------------------------------+--------------------------------------------------------------------------------+--------------------------------------------------------------------------------+
| **Setting a property value** | The property path is separated by dots. E.g. "Vehicle.Size.Length"             | The property path is separated by slashes. E.g. "Vehicle/Size/Length"          |
|                              |                                                                                |                                                                                |
|                              | The only possible data types are const char*, double, int32_t and bool.        | Currently supported data types are std::string, double, int32_t, bool and      |
|                              |                                                                                | ``std::vector<T>`` of these four.                                              |
|                              | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_2.cpp           |                                                                                |
|                              |     :start-after: //Begin(SetProperty)                                         | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_3.cpp           |
|                              |     :end-before: //End(SetProperty)                                            |     :start-after: //Begin(SetProperty)                                         |
|                              |     :dedent: 8                                                                 |     :end-before: //End(SetProperty)                                            |
|                              |                                                                                |     :dedent: 8                                                                 |
+------------------------------+--------------------------------------------------------------------------------+--------------------------------------------------------------------------------+
| **Getting a property value** | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_2.cpp           | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_3.cpp           |
|                              |     :start-after: //Begin(GetProperty)                                         |     :start-after: //Begin(GetProperty)                                         |
|                              |     :end-before: //End(GetProperty)                                            |     :end-before: //End(GetProperty)                                            |
|                              |     :dedent: 8                                                                 |     :dedent: 8                                                                 |
+------------------------------+--------------------------------------------------------------------------------+--------------------------------------------------------------------------------+
| **Creating a custom          | Identical to setting a property value.                                         | .. _label_custom_property:                                                     |
| property**                   |                                                                                |                                                                                |
|                              |                                                                                | Derive from :cpp:class:`fep3::base::Configuration`. This is already the case   |
|                              |                                                                                | for :cpp:class:`fep3::cpp::arya::DataJob` and                                  |
|                              |                                                                                | :cpp:class:`fep3::core::ElementConfigurable`.                                  |
|                              |                                                                                |                                                                                |
|                              |                                                                                | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_3.cpp           |
|                              |                                                                                |     :start-after: //Begin(CreateProperty)                                      |
|                              |                                                                                |     :end-before: //End(CreateProperty)                                         |
|                              |                                                                                |     :dedent: 8                                                                 |
|                              |                                                                                |                                                                                |
|                              |                                                                                | To set a new value for the ``PropertyVariable<T>`` use the                     |
|                              |                                                                                | :cpp:func:`fep3::arya::setPropertyValue<T>()` function like with               |
|                              |                                                                                | any other property.                                                            |
|                              |                                                                                |                                                                                |
|                              |                                                                                | To get the current value it is also possible to use the                        |
|                              |                                                                                | :cpp:func:`fep3::arya::getPropertyValue<T>()` function                         |
|                              |                                                                                | instead of the ``static_cast<T>()``.                                           |
+------------------------------+--------------------------------------------------------------------------------+--------------------------------------------------------------------------------+
| **Listening to property      | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_2.cpp           | FEP 3 does not have a passive listener for properties.                         |
| changes**                    |     :start-after: //Begin(DeclarationListener)                                 |                                                                                |
|                              |     :end-before: //End(DeclarationListener)                                    | A :cpp:class:`fep3::arya::IPropertyObserver` can be registered at custom       |
|                              |                                                                                | properties but has to be triggered manually. If the above pattern with         |
|                              | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_2.cpp           | PropertyVariables is used, the observer will be registered automatically.      |
|                              |     :start-after: //Begin(RegisterListener)                                    |                                                                                |
|                              |     :end-before: //End(RegisterListener)                                       | For more information about property observers see the chapter                  |
|                              |     :dedent: 8                                                                 | :ref:`label_property_observers`.                                               |
|                              |     :caption: class cMyModule                                                  |                                                                                |
|                              |                                                                                |                                                                                |
|                              | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_2.cpp           |                                                                                |
|                              |     :start-after: //Begin(PropertyChange)                                      |                                                                                |
|                              |     :end-before: //End(PropertyChange)                                         |                                                                                |
|                              |     :dedent: 4                                                                 |                                                                                |
+------------------------------+--------------------------------------------------------------------------------+--------------------------------------------------------------------------------+
| **Access to the property     | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_2.cpp           | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_3.cpp           |
| tree structure**             |     :start-after: //Begin(PropertyStructure)                                   |     :start-after: //Begin(PropertyStructure)                                   |
|                              |     :end-before: //End(PropertyStructure)                                      |     :end-before: //End(PropertyStructure)                                      |
|                              |     :dedent: 8                                                                 |     :dedent: 8                                                                 |
+------------------------------+--------------------------------------------------------------------------------+--------------------------------------------------------------------------------+
| **Getting a remote           | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_2.cpp           | Not supported.                                                                 |
| property**                   |     :start-after: //Begin(RemoteProperty)                                      |                                                                                |
|                              |     :end-before: //End(RemoteProperty)                                         | FEP Participants have no knowledge about other FEP Participants and can        |
|                              |     :dedent: 8                                                                 | therefore not access them via :ref:`RPC<label_service_bus>`.                   |
+------------------------------+--------------------------------------------------------------------------------+                                                                                |
| **Mirroring a remote         | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_2.cpp           |                                                                                |
| property**                   |     :start-after: //Begin(MirrorProperty)                                      |                                                                                |
|                              |     :end-before: //End(MirrorProperty)                                         |                                                                                |
|                              |     :dedent: 8                                                                 |                                                                                |
+------------------------------+--------------------------------------------------------------------------------+--------------------------------------------------------------------------------+
| **FEP Participant Header     | FEP Participant Header properties:                                             | :cpp:class:`ParticipantInfo<fep3::rpc::arya::IRPCParticipantInfo>` component:  |
| Information**                |                                                                                |                                                                                |
|                              | * ElementVersion                                                               | * FEP Participant name                                                         |
|                              | * ElementName                                                                  | * FEP System name                                                              |
|                              | * ElementDescription                                                           |                                                                                |
|                              | * FEPVersion                                                                   | fep3_participant_version.h:                                                    |
|                              | * Platform                                                                     |                                                                                |
|                              | * ElementContext                                                               | * FEP Participant Library version (usually identical to the FEP version)       |
|                              | * ElementContextVersion                                                        |                                                                                |
|                              | * ElementVendor                                                                | FEP Element related properties have to be added by the user himself to the     |
|                              | * ElementDisplayName                                                           | :ref:`label_configuration_service`.                                            |
|                              | * ElementCompilationDate                                                       | See :ref:`Creating a custom property<label_custom_property>` above.            |
|                              | * CurrentState                                                                 |                                                                                |
|                              | * Host                                                                         |                                                                                |
|                              | * InstanceID                                                                   |                                                                                |
|                              | * TypeID                                                                       |                                                                                |
+------------------------------+--------------------------------------------------------------------------------+--------------------------------------------------------------------------------+

Logging
=======

+------------------------------+--------------------------------------------------------------------------------+--------------------------------------------------------------------------------+
|                              | FEP 2 Implementation                                                           | FEP 3 Implementation                                                           |
+------------------------------+--------------------------------------------------------------------------------+--------------------------------------------------------------------------------+
| **Writing a log**            | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_2.cpp           | Derive from :cpp:class:`fep3::base::EasyLogging` and call                      |
|                              |     :start-after: //Begin(InvokeIncident)                                      | :cpp:func:`~fep3::base::EasyLogging::initLogger()`. If more than one           |
|                              |     :end-before: //End(InvokeIncident)                                         | logger is needed the :ref:`label_logging_service` has to be used directly.     |
|                              |     :dedent: 8                                                                 |                                                                                |
|                              |     :caption: class cMyModule                                                  | :cpp:class:`fep3::cpp::arya::DataJob` and                                      |
|                              |                                                                                | :cpp:class:`fep3::core::ElementBase` are already derived from                  |
|                              |                                                                                | :ref:`label_core_easy_logging` and do not need to be initialized manually.     |
|                              |                                                                                |                                                                                |
|                              |                                                                                | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_3.cpp           |
|                              |                                                                                |     :start-after: //Begin(Logging)                                             |
|                              |                                                                                |     :end-before: //End(Logging)                                                |
|                              |                                                                                |     :dedent: 8                                                                 |
|                              |                                                                                |                                                                                |
|                              |                                                                                | Meta information like the FEP Participant name, line number, timestamp etc.    |
|                              |                                                                                | will be added to the log automatically.                                        |
+------------------------------+--------------------------------------------------------------------------------+--------------------------------------------------------------------------------+
| **Configuring the logging    | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_2.cpp           | Logging filters cannot be set by the FEP Element itself. Use the FEP System    |
| mechanism**                  |     :start-after: //Begin(ConfigureLogging)                                    | Library to set the filters via RPC.                                            |
|                              |     :end-before: //End(ConfigureLogging)                                       |                                                                                |
|                              |     :dedent: 8                                                                 | .. hint::                                                                      |
|                              |                                                                                |     For more details see :ref:`label_demo_logging`                             |
+------------------------------+--------------------------------------------------------------------------------+--------------------------------------------------------------------------------+
| **Creating a custom incident | Implement ``fep::IIncidentStrategy``:                                          | Derive from ``fep3::Properties<fep3::ILoggingService::ILoggingSink>``.         |
| strategy**                   |                                                                                |                                                                                |
|                              | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_2.cpp           | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_3.cpp           |
|                              |     :start-after: //Begin(CustomStrategy)                                      |     :start-after: //Begin(LoggingSink)                                         |
|                              |     :end-before: //End(CustomStrategy)                                         |     :end-before: //End(LoggingSink)                                            |
|                              |                                                                                |                                                                                |
|                              | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_2.cpp           | For this feature the :ref:`label_logging_service` has to be used.              |
|                              |     :start-after: //Begin(AssociateStrategy)                                   | :ref:`label_core_easy_logging` does not support custom logging sinks.          |
|                              |     :end-before: //End(AssociateStrategy)                                      |                                                                                |
|                              |     :dedent: 8                                                                 | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_3.cpp           |
|                              |     :caption: class cMyModule                                                  |     :start-after: //Begin(RegisterSink)                                        |
|                              |                                                                                |     :end-before: //End(RegisterSink)                                           |
|                              | The custom incident strategy can then be configured with the property tree.    |     :dedent: 8                                                                 |
|                              |                                                                                |     :caption: class MyElement                                                  |
|                              |                                                                                |                                                                                |
|                              |                                                                                | The custom logging sink can then be configured with the RPC                    |
|                              |                                                                                | ``setSinkProperty()`` via the FEP System Library similar to setting            |
|                              |                                                                                | logging filters.                                                               |
|                              |                                                                                |                                                                                |
|                              |                                                                                | .. hint::                                                                      |
|                              |                                                                                |     For an example take a look at the implementation of the native file sink   |
|                              |                                                                                |     class ``fep3::LoggingSinkFile``, which can also be used as a base class    |
|                              |                                                                                |     for custom logging sinks, or use the template from the chapter             |
|                              |                                                                                |     :ref:`label_logging_sink`.                                                 |
+------------------------------+--------------------------------------------------------------------------------+--------------------------------------------------------------------------------+

Automation Interface
====================

The automation interface has been removed in FEP 3 and will not be added again. Use the :ref:`FEP System Library <FEP_System_Library>`
to control a FEP System. A FEP Participant is not allowed to have any control over the FEP System in FEP 3.

+------------------------------+--------------------------------------------------------------------------------+--------------------------------------------------------------------------------+
|                              | FEP 2 Implementation                                                           | FEP 3 Implementation                                                           |
+------------------------------+--------------------------------------------------------------------------------+--------------------------------------------------------------------------------+
| **Muting**                   | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_2.cpp           | Muting is not natively supported. But a custom FEP Element / job can be        |
|                              |     :start-after: //Begin(Muting)                                              | programmed to have the same functionality as so:                               |
|                              |     :end-before: //End(Muting)                                                 |                                                                                |
|                              |     :dedent: 8                                                                 | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_3.cpp           |
|                              |                                                                                |     :start-after: //Begin(MutingVariable)                                      |
|                              |                                                                                |     :end-before: //End(MutingVariable)                                         |
|                              |                                                                                |     :dedent: 8                                                                 |
|                              |                                                                                |                                                                                |
|                              |                                                                                | .. literalinclude:: ../../snippets/snippet_migration_guide_fep_3.cpp           |
|                              |                                                                                |     :start-after: //Begin(MuteSignal)                                          |
|                              |                                                                                |     :end-before: //End(MuteSignal)                                             |
|                              |                                                                                |     :dedent: 8                                                                 |
+------------------------------+--------------------------------------------------------------------------------+--------------------------------------------------------------------------------+

Testing
=======

To test a FEP Element in FEP 3 one can either use the :cpp:class:`fep3::core::ParticipantExecutor`
to do unit testing or use the :ref:`FEP System Library <FEP_System_Library>` / :term:`FEP Base Utilities`
to test several FEP Participants together.

Unit Testing
------------

The FEP Participant Library provides the :cpp:class:`fep3::core::ParticipantExecutor` class,
which will call the exec function asynchronously, so it is possible to continue testing.
It can also control the :ref:`label_participant_state_machine` of the FEP Participant to
start and stop it.

.. literalinclude:: ../../snippets/snippet_migration_guide_fep_3.cpp
    :start-after: //Begin(ParticipantExecutor)
    :end-before: //End(ParticipantExecutor)
    :dedent: 4
    :caption: main()

System Testing
--------------

To test an entire FEP System with more than one FEP Participant, the FEP Base Utilities control tool or
the FEP System Library is recommended.

* The FEP System can not only be controlled directly using the fep control tool, but also using a script, e.g. fep_control < control.txt.
* One can also write an executable with the :ref:`FEP System Library <FEP_System_Library>`, which has access to the whole FEP System
  and FEP Participant APIs.

If the :ref:`FEP System Library <FEP_System_Library>` is being used, the :cpp:class:`fep3::core::ParticipantExecutor`
is recommended again to start the FEP Participants in separate threads. But use the FEP System Library API
to start and stop the FEP Participants in this case and not the *ParticipantExecutor* functions!
It is also possible to start all FEP Participants in their own executables.

Discovering
-----------

FEP2 uses only RTI DDS Discovering, whereas FEP3 by default provide discovering based on customized multicasting.
If multicasting is not possible, FEP3 can switch to the RTI DDS Discovering via configurating the :ref:`CPP Plugin<label_component_cpp_plugins>`.
For more information about discovering, please refer to :ref:`label_service_bus`.

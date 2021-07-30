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


.. _label_component_system:

====================
FEP Component System
====================

The :term:`FEP Component System` makes a :term:`FEP Participant` work with a certain set of :term:`FEP Components<FEP Component>`
and :term:`FEP Component Implementations<FEP Component Implementation>`.
The *purpose* of a :term:`FEP Participant` is defined by the set of :term:`FEP Components<FEP Component>` it provides,
while the *behavior* of a :term:`FEP Participant` depends on the :term:`FEP Component Implementations<FEP Component Implementation>`
that are loaded by the :term:`FEP Component System`.

For example

* the *purpose* of a :term:`FEP Participant` in the context of :term:`FEP` is to be a participant of a
  simulation and integration platform for the development and for the test of highly cross-linked vehicle functions
* the *behavior* of a :term:`FEP Participant` can be e. g.

  * simulation data is transferred via RTI DDS
  * or simulation data is transferred via ZeroMQ.

Using the :term:`FEP Component System` the purpose and/or the behavior of the :term:`FEP Participant` can be changed or extended
by configuring the :term:`FEP Participant` to use other :term:`FEP Components<FEP Component>` than the :ref:`label_default_components`.

.. note:
  For each :term:`FEP Component Interface` only one :term:`FEP Component Implementation` can be loaded by the :term:`FEP Component System`.

.. _label_components_default_vs_native_vs_foreign_components:

Default :term:`FEP Components<FEP Component>` vs. Native :term:`FEP Components<FEP Component>` vs. Foreign :term:`FEP Components<FEP Component>`
________________________________________________________________________________________________________________________________________________

:ref:`label_native_components` are shipped with the :term:`FEP Participant Library`. The :term:`FEP Participant` loads a subset of those
:ref:`label_native_components` by default, i. e. if no :ref:`label_component_configuration_file` is provided. The :term:`FEP Components<FEP Component>` in such
subset are called :ref:`label_default_components`. Additionally to the :term:`FEP Components<FEP Component>` in the set of :ref:`label_default_components`
the :term:`FEP Participant Library` ships the following :ref:`label_native_components`:

+--------------------------------------------------+--------------------------------------+-------------------------------------------------------+
| :term:`FEP Component Implementation`             | :term:`FEP Component`                | FEP Component Interface                               |
+==================================================+======================================+=======================================================+
| :ref:`HTTP Service Bus`                          | Service Bus                          | :cpp:class:`fep3::arya::IServiceBus`                  |
+--------------------------------------------------+--------------------------------------+-------------------------------------------------------+
| :ref:`RTI Connext DDS Simulation Bus`            | Simulation Bus                       | :cpp:class:`fep3::arya::ISimulationBus`               |
+--------------------------------------------------+--------------------------------------+-------------------------------------------------------+

Besides those :ref:`label_native_components` the :term:`FEP Participant` can be configured to load so-called "Foreign :term:`FEP Components<FEP Component>`".
Such :term:`FEP Components<FEP Component>` may be delivered by other departments of the :term:`FEP` project or by third parties.

Using the :ref:`label_component_system`
_______________________________________

To make a :term:`FEP Participant` use other :term:`FEP Components<FEP Component>` than the :ref:`label_default_components`,
a :ref:`label_component_configuration_file` has to be provided. This file contains information on which :term:`FEP Components<FEP Component>` shall be
used by a :term:`FEP Participant`.

.. _label_component_configuration_file:

FEP Component Configuration File
--------------------------------
The :term:`FEP Component Configuration File` is an XML file with the following content (exemplary):
::

    <?xml version="1.0" encoding="utf-8"?>
    <components xmlns="http://fep.vwgroup.com/fep_sdk/3.0/components">
        <schema_version>1.0.0</schema_version>
        <component>
            <source type="built-in"/>
            <iid>clock_service.arya.fep3.iid</iid>
        </component>
        <component>
            <source type="cpp-plugin">
                my_component_plugin
            </source>
            <iid>service_bus.arya.fep3.iid</iid>
        </component>
        <component>
            <source type="c-plugin">
                third_party_component_plugin
            </source>
            <iid>simulation_bus.arya.fep3.iid</iid>
        </component>
    </components>

where

* the ``components/component`` element can occur any number of times
* the ``components/component/iid`` element contains the interface identifier of the component to be loaded
* the ``components/component/source`` element contains information on where to load the component implementation from:

  * the ``components/component/source@type`` attribute defines the :ref:`label_component_instantiation` and
    takes the values ``built-in``, ``cpp-plugin`` or ``c-plugin``
  * if the ``components/component/source@type`` attribute is ``cpp-plugin`` or ``c-plugin``,
    the data of ``components/component/source`` has to contain the file path of the :ref:`FEP Component Plugin's<label_component_plugins>`
    shared library that provides the component implementation; such file path has to be either absolute or relative to the directory
    the :term:`FEP Component Configuration File` resides in

During startup the :term:`FEP Component System` of the :term:`FEP Participant` tries to find a :ref:`label_component_configuration_file` as follows:

* If the environment variable "FEP3_PARTICIPANT_COMPONENTS_FILE_PATH" is set and it contains an absolute
  file path it tries to find a :ref:`label_component_configuration_file` at this absolute file path.

* If the environment variable "FEP3_PARTICIPANT_COMPONENTS_FILE_PATH" is set and it contains a relative
  file path it tries to find a :ref:`label_component_configuration_file` at this file path relative to

  * the current working directory
  * the directory where the fep3_participant shared library file resides in

* If the environment variable "FEP3_PARTICIPANT_COMPONENTS_FILE_PATH" is not set it tries to find
  a :ref:`label_component_configuration_file` with the name "fep3_participant.fep_components" in

  * the current working directory
  * the directory where the fep3_participant shared library file resides in

If a :ref:`label_component_configuration_file` is found according to those rules, the :term:`FEP Components<FEP Component>`
are loaded according to the rules in that file. Otherwise, i. e. if no :ref:`label_component_configuration_file` is found, the
:ref:`label_default_components` are loaded.

To ensure consistency, all :term:`FEP Participants<FEP Participant>` of a :term:`FEP System` should use the same
:ref:`label_component_configuration_file`. Therefore the :ref:`label_component_configuration_file`

* has to be provided by the :term:`FEP System Developer`
* has to be deployed to the :term:`FEP Participants<FEP Participant>` by the deployment process of the :term:`FEP System`.

.. _label_component_instantiation:

Component instantiation
-----------------------

Upon creation of the participant (see :ref:`label_participant_creation`), the :term:`FEP Components<FEP Component>` are instantiated
by the :ref:`label_component_system`. The order of the :term:`FEP Component` instantiation follows the order of the ``components/component`` elements in the
:ref:`label_component_configuration_file`.

The :term:`FEP Participant Library` provides three mechanisms for the instantiation of :term:`FEP Components<FEP Component>`:

* "built-in": the :term:`FEP Component` instance is being created from the pool of
  :term:`FEP Component Implementations<FEP Component Implementation>` that are built into the :term:`FEP Participant Library`
* "CPP plugin": the :term:`FEP Component` instance is being created from a :ref:`Component CPP Plugin<label_component_cpp_plugins>`
* "C plugin": the :term:`FEP Component` instance is being created from a :ref:`Component C Plugin<label_component_c_plugins>`

See :ref:`label_native_components` for a list and description of :term:`FEP Components<FEP Component>` that are delivered
with the :term:`FEP Participant Library`.

The destruction of the :term:`FEP Components<FEP Component>` takes place when the participant instance is destroyed.
The order of the :term:`FEP Component` destruction is done in the revers order of the ``components/component`` elements in the
:ref:`label_component_configuration_file`.

.. _label_calls_to_component:

Calls to the :cpp:class:`fep3::arya::IComponent` interface
----------------------------------------------------------

When the participant is executed (see :ref:`label_participant_execution`), the method :cpp:func:`fep3::arya::IComponent::createComponent`
is called; when the participant execution has finished (:ref:`label_participant_state_machine` state ``Final``) the
method :cpp:func:`fep3::arya::IComponent::destroyComponent` is called.

All other calls to the methods of the :term:`FEP Component's<FEP Component>` interface :cpp:class:`fep3::arya::IComponent` (e. g. :cpp:func:`fep3::arya::IComponent::initialize`)
occur from within the context of the transitions of the :term:`FEP Participant`'s :ref:`label_participant_state_machine`.
The following table shows the calls to the FEP Component API as performed by the component-related guards and actions (postfixed by ``..._components`` in the state machine diagram):

+-------------------------------------------+----------------------------------------------------------+---------------+------------------------------------------------------+
| guard/action                              | calls on the FEP Component API                           | order         | rollback call                                        |
+===========================================+==========================================================+===============+======================================================+
| initialize_components                     | :cpp:func:`fep3::arya::IComponent::initialize`           | forward       | :cpp:func:`fep3::arya::IComponent::deinitialize`     |
+-------------------------------------------+----------------------------------------------------------+---------------+------------------------------------------------------+
| deinitialize_components                   | :cpp:func:`fep3::arya::IComponent::deinitialize`         | reverse       | none                                                 |
+-------------------------------------------+----------------------------------------------------------+---------------+------------------------------------------------------+
| tense_components                          | :cpp:func:`fep3::arya::IComponent::tense`                | forward       | :cpp:func:`fep3::arya::IComponent::relax`            |
+-------------------------------------------+----------------------------------------------------------+---------------+------------------------------------------------------+
| relax_components                          | :cpp:func:`fep3::arya::IComponent::relax`                | reverse       | none                                                 |
+-------------------------------------------+----------------------------------------------------------+---------------+------------------------------------------------------+
| start_components                          | :cpp:func:`fep3::arya::IComponent::start`                | forward       | :cpp:func:`fep3::arya::IComponent::stop`             |
+-------------------------------------------+----------------------------------------------------------+---------------+------------------------------------------------------+
| pause_components                          | :cpp:func:`fep3::arya::IComponent::pause`                | forward       | none                                                 |
+-------------------------------------------+----------------------------------------------------------+---------------+------------------------------------------------------+
| stop_components                           | :cpp:func:`fep3::arya::IComponent::stop`                 | reverse       | none                                                 |
+-------------------------------------------+----------------------------------------------------------+---------------+------------------------------------------------------+

Note that calls to the :term:`FEP Component` API happen on all :term:`FEP Components<FEP Component>` as used by the :term:`FEP Participant`.
As indicated in column ``order`` in the above table, these calls occur in

* the *forward* order
* the *reverse* order

of how the ``components/component`` elements appear in the :ref:`label_component_configuration_file`.
If a call to the :term:`FEP Component` API fails, the ``rollback call`` (if any is indicated in above table) occurs on
all :term:`FEP Components<FEP Component>` whose call in the current guard/action has succeeded before, e. g.:
For a :term:`FEP Participant` with the :term:`FEP Components<FEP Component>` ``A``, ``B`` and ``C``: The guard ``initialize_components``
calls :cpp:func:`fep3::arya::IComponent::initialize` on all :term:`FEP Components<FEP Component>` in forward order

1. A
2. B
3. C

If the call on C fails, :cpp:func:`fep3::arya::IComponent::deinitialize` is called on A and B in the reverse order:

1. B
2. A

.. _label_component_plugins:

FEP Component Plugins
_____________________
Foreign :term:`FEP Components<FEP Component>` can be provided to the :term:`FEP Participant`

* in a :ref:`Component CPP Plugin<label_component_cpp_plugins>` via the :ref:`label_component_cpp_plugin_api`
* or in a :ref:`Component C Plugin<label_component_c_plugins>` via the :ref:`label_component_c_plugin_api`.

The following table helps to decide which API to use:

+-------------------------------------------+----------------------------------------------------------+-------------------------------------------------------+
|                                           | :ref:`Component CPP Plugin<label_component_cpp_plugins>` | :ref:`Component C Plugin<label_component_c_plugins>`  |
+===========================================+==========================================================+=======================================================+
| Purpose                                   | * FEP internal purposes                                  | Third-party middleware                                |
|                                           | * Components not available in C plugins                  |                                                       |
|                                           |   (see limitations)                                      |                                                       |
+-------------------------------------------+----------------------------------------------------------+-------------------------------------------------------+
| Focus                                     | Extendability, changeability                             | Binary compatibility                                  |
+-------------------------------------------+----------------------------------------------------------+-------------------------------------------------------+
| Limitations                               | * Debug/Release not mixable                              | Available only for a subset of                        |
|                                           | * The :ref:`label_fep_3_sdk` defines which Compiler /    | Components, currently:                                |
|                                           |   Compiler Settings must be used (-> to                  |                                                       |
|                                           |   prevent binary incompatibilities the plugins           | * Clock Service                                       |
|                                           |   should be built using the FEP3 SDK’s build             | * Configuration Service                               |
|                                           |   facilities)                                            | * Job Registry                                        |
|                                           |                                                          | * Participant Info                                    |
|                                           |                                                          | * Scheduler Service                                   |
|                                           |                                                          | * Simulation Bus                                      |
+-------------------------------------------+----------------------------------------------------------+-------------------------------------------------------+
| value to be set to the attribute          | ``cpp-plugin``                                           | ``c-plugin``                                          |
| ``components/component/source@type`` in   |                                                          |                                                       |
| :ref:`label_component_configuration_file` |                                                          |                                                       |
+-------------------------------------------+----------------------------------------------------------+-------------------------------------------------------+

.. _label_component_cpp_plugins:

Component CPP Plugins
---------------------
CPP plugins use the C++ interface of the :ref:`label_component_system`. They offer the highest flexibility as they are not
restricted to a subset of :term:`FEP Components<FEP Component>`, i. e. the functionality of
a :term:`FEP Participant` can be changed or extended by any functionality via CPP plugins, even by defining completely new
:term:`FEP Components<FEP Component>`.
For CPP plugins the :term:`FEP Participant Library` does not guarantee full backward compatibility,
so CPP plugins might have to be rebuilt when switching to a newer version of the :term:`FEP Participant Library`.

.. _label_component_c_plugins:

Component C Plugin
------------------
C plugins use the C interface of the :ref:`label_component_system`. They ensure full binary compatibility for mixing
debug/release versions and different compilers and compiler settings.
The :term:`FEP Participant Library` guarantees full backward compatibility for C plugins, i. e. future versions of
the :term:`FEP Participant` can use existing C plugins. Therefore :term:`FEP` recommends using C plugins for connecting
:term:`FEP Participants<FEP Participant>` to third-party middleware.

.. note:: Components in C plugins are written in C++ programming language against the C++ :term:`FEP Component Interface`.
    Only the hidden interface between the plugin and the :term:`FEP Participant Library` is implemented in C language.

Creating a FEP Component Plugin in order to provide Foreign FEP Components
--------------------------------------------------------------------------
FEP Component Plugins must be provided as shared libraries.

.. note:: One FEP Component Plugin may contain multiple :term:`FEP Components<FEP Component>` but only one
    :term:`FEP Component Implementation` per :term:`FEP Component Interface`.

.. _label_component_cpp_plugin_api:

FEP Component CPP API
~~~~~~~~~~~~~~~~~~~~~
To create a :ref:`Component CPP Plugin<label_component_cpp_plugins>`, include
::

    #include <fep3/plugin/cpp/cpp_plugin_impl_arya.hpp>

in your plugin's C++ source file and implement the functions ``fep3_plugin_getPluginVersion``
and ``fep3_plugin_cpp_arya_getFactory``, e. g. like so:
::

    void fep3_plugin_getPluginVersion(void(*callback)(void*, const char*), void* destination)
    {
        callback(destination, "my component plugin, version 1.0");
    }

    fep3::plugin::cpp::ICPPPluginComponentFactory* fep3_plugin_cpp_arya_getFactory()
    {
        return new fep3::plugin::cpp::CPPPluginComponentFactory<MyComponentInterface>;
    }

To create your :term:`FEP Component Implementation` include
::

    #include <fep3/components/base/component.h>

and derive your :term:`FEP Component Implementation` class publicly from
::

    fep3::base::Component<MyComponentInterface>

The :term:`FEP Component Interface` ("MyComponentInterface" in the examples above) may be one of the
interfaces defined by the :term:`FEP Participant Library` (see :ref:`label_default_components`)
or any other class calling the macro ``FEP_COMPONENT_IID``, e. g.
::

    class MyComponentInterface
    {
    protected:
        virtual ~MyComponentInterface() = default;

    public:
        FEP_COMPONENT_IID("my_component");

        // methods of the component
        virtual int get() const = 0;
    };

.. note:: The destructor of such :term:`FEP Component Interface` class shall be protected
    to prevent destruction by the :term:`FEP Participant Developer` or the :term:`FEP Component Developer`.

.. _label_component_c_plugin_api:

FEP Component C API
~~~~~~~~~~~~~~~~~~~
To create a :ref:`Component C Plugin<label_component_c_plugins>`, include
::

  #include <fep3/plugin/c/c_plugin.h>

in your plugin's C++ source file and implement the function ``fep3_plugin_getPluginVersion``, e. g. like so:
::

    void fep3_plugin_getPluginVersion(void(*callback)(void*, const char*), void* destination)
    {
        callback(destination, "my component plugin, version 1.0");
    }

To create your :term:`FEP Component Implementation` include
::

    #include <fep3/components/base/c_access_wrapper/transferable_component_base.h>

and derive your :term:`FEP Component Implementation` class publicly from
::

    fep3::plugin::c::TransferableComponentBase<...>

, passing the appropriate :term:`FEP Component Interface` (e. g. :cpp:class:`fep3::arya::ISimulationBus`)
as template parameter.

In your :term:`FEP Component Implementation` file include the appropriate access wrapper header, e. g. for a Simulation Bus Component
::

    #include <fep3/components/simulation_bus/c_access_wrapper/simulation_bus_c_access_wrapper.h>

and implement the component creation function, e. g. ``fep3_plugin_c_arya_createSimulationBus``, like so:
::

    fep3_plugin_c_CInterfaceError fep3_plugin_c_arya_createSimulationBus
        (fep3_arya_SISimulationBus* access
        , fep3_plugin_c_arya_SISharedBinary shared_binary_access
        , const char* iid
        )
    {
        return fep3::plugin::c::wrapper::arya::createSimulationBus
            ([]()
                {
                    return new MySimulationBus;
                }
            , access
            , shared_binary_access
            , iid
            );
    }

where ``MySimulationBus`` is your :term:`FEP Component Implementation` class.

.. Copyright 2023 CARIAD SE.
.. 
.. This Source Code Form is subject to the terms of the Mozilla 
.. Public License, v. 2.0. If a copy of the MPL was not distributed 
.. with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

=========
Component
=========

The functional parts of the :term:`FEP Participant` are called :term:`FEP Components<FEP Component>`.
Each :term:`FEP Component` covers some specific functionality, e.g. functionality to exchange simulation data with other participants.

:term:`FEP Components<FEP Component>` can be accessed via the :term:`FEP Participant` API's
template method getComponent() in :cpp:class:`fep3::base::Participant`, passing the :term:`FEP Component Interface`
as template argument.

.. _label_component_system:

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
by configuring the :term:`FEP Participant` to use other :term:`FEP Components<FEP Component>` than the :ref:`Default_Components_vs_Foreign_Components`.

.. note:
  For each :term:`FEP Component Interface` only one :term:`FEP Component Implementation` can be loaded by the :term:`FEP Component System`.

.. _Default_Components_vs_Foreign_Components:

Default :term:`FEP Components<FEP Component>` vs. Foreign FEP Components
------------------------------------------------------------------------


By default the :term:`FEP Participant` loads a set of the FEP Default Component plugins , which consists of the
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
| :ref:`label_job_registry`            | :cpp:class:`fep3::catelyn::IJobRegistry`         |                                                       |
+--------------------------------------+--------------------------------------------------+-------------------------------------------------------+
| :ref:`label_logging_service`         | :cpp:class:`fep3::arya::ILoggingService`         | :cpp:class:`fep3::rpc::arya::IRPCLoggingService`      |
+--------------------------------------+--------------------------------------------------+-------------------------------------------------------+
| :ref:`label_participant_info`        | :cpp:class:`fep3::arya::IParticipantInfo`        |                                                       |
+--------------------------------------+--------------------------------------------------+-------------------------------------------------------+
| :ref:`label_scheduler_service`       | :cpp:class:`fep3::catelyn::ISchedulerService`    |                                                       |
+--------------------------------------+--------------------------------------------------+-------------------------------------------------------+
| :ref:`label_service_bus`             | :cpp:class:`fep3::catelyn::IServiceBus`          | :cpp:class:`fep3::rpc::arya::IRPCParticipantInfo`     |
+--------------------------------------+--------------------------------------------------+-------------------------------------------------------+
| :ref:`label_simulation_bus`          | :cpp:class:`fep3::arya::ISimulationBus`          |                                                       |
+--------------------------------------+--------------------------------------------------+-------------------------------------------------------+
| :ref:`label_health_service`          | :cpp:class:`fep3::catelyn::IHealthService`       |  :cpp:class:`fep3::rpc::catelyn::IRPCHealthServiceDef`|
+--------------------------------------+--------------------------------------------------+-------------------------------------------------------+


Besides those :ref:`label_native_components` the :term:`FEP Participant` can be configured to load so-called "Foreign :term:`FEP Components<FEP Component>`".
Such :term:`FEP Components<FEP Component>` may be delivered by other departments of the :term:`FEP` project or by third parties.

.. _label_backwards_compatibility:

Backwards compatibility
-----------------------

The :term:`FEP Component System` is designed to support full backward compatibility of :ref:`FEP Component Plugin's<label_component_plugin_main>`
, i. e. existing :term:`FEP Participants<FEP Participant>` can use future versions of :ref:`FEP Component Plugin's<label_component_plugin_main>`.

.. _label_component_configuration_file:

FEP Component Configuration File
================================

To make a :term:`FEP Participant` use other :term:`FEP Components<FEP Component>` than the :ref:`Default_Components_vs_Foreign_Components`,
a :ref:`label_component_configuration_file` has to be provided. This file contains information on which FEP Components shall be
used by a :term:`FEP Participant`.

The :term:`FEP Component Configuration File` is an XML file with the following content (exemplary):
::

    <?xml version="1.0" encoding="utf-8"?>
    <components xmlns="http://fep.vwgroup.com/fep_sdk/3.0/components">
        <schema_version>1.0.0</schema_version>
        <component>
            <source type="cpp-plugin">
                my_component_plugin
            </source>
            <iid>service_bus.arya.fep3.iid</iid>
        </component>
        <component>
            <source type="cpp-plugin">
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
        takes one of the following the values

        * ``cpp-plugin``
        * ``c-plugin`` [deprecated]. Loading a c-plugin will result in an exception.
        
  * the data of ``components/component/source`` has to contain the file path of the :ref:`FEP Component Plugin's<label_component_plugin_main>`
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
are loaded according to the rules in that file. 

To ensure consistency, all :term:`FEP Participants<FEP Participant>` of a :term:`FEP System` should use the same
:ref:`label_component_configuration_file`. Therefore the :ref:`label_component_configuration_file`

* has to be provided by the :term:`FEP System Developer`
* has to be deployed to the :term:`FEP Participants<FEP Participant>` by the deployment process of the :term:`FEP System`.

.. _label_component_instantiation:

Component instantiation
=======================

Upon creation of the :term:`FEP Participant` (see :ref:`label_create_participant`), the :term:`FEP Components<FEP Component>` are instantiated
by the :ref:`label_component_system`. The order of the :term:`FEP Component` instantiation follows the order of the ``components/component`` elements in the
:ref:`label_component_configuration_file`.

The :term:`FEP Participant Library` provides three mechanisms for the instantiation of :term:`FEP Components<FEP Component>`:

* "CPP plugin": the :term:`FEP Component` instance is being created from a :ref:`Component CPP Plugin<label_component_cpp_plugins>`
* "C plugin" [deprecated]: the :term:`FEP Component` instance is being created from a :ref:`Component C Plugin<label_component_c_plugins>`

See :ref:`label_native_components` for a list and description of :term:`FEP Components<FEP Component>` that are delivered
with the :term:`FEP Participant Library`.

The destruction of the :term:`FEP Components<FEP Component>` takes place when the participant instance is destroyed.
The order of the :term:`FEP Component` destruction is done in the revers order of the ``components/component`` elements in the
:ref:`label_component_configuration_file`.

.. _label_calls_to_component:

Calls to the :cpp:class:`fep3::arya::IComponent` interface
----------------------------------------------------------

When the participant is executed (see :ref:`label_participant_execution`), the method :cpp:func:`fep3::arya::IComponent::createComponent`
is called; when the participant execution has finished (:ref:`label_participant_state_machine` state ``Finalized``) the
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

.. _label_component_plugin_main:

FEP Component Plugins
=====================

Foreign :term:`FEP Components<FEP Component>` can be provided to the :term:`FEP Participant`

* in a :ref:`Component CPP Plugin<label_component_cpp_plugins>` via the :ref:`label_component_cpp_plugin_api_main`

.. _label_component_cpp_plugins:

Component CPP Plugins
---------------------

CPP plugins use the C++ interface of the :ref:`label_component_system` and may be used in order to change or extend the functionality of
a :term:`FEP Participant` by any functionality, even by defining completely new :term:`FEP Components<FEP Component>`.

.. note::
    * Debug/Release versions are not mixable, i. e. a :term:`FEP Participant` built in Debug 
      configuration cannot load a CPP Plugin built in Release configuration and vice versa.
    * The :ref:`label_fep_3_sdk` defines which Compiler / Compiler Settings must be used. 
      In order to prevent binary incompatibilities CPP Plugins should be built using 
      the FEP3 SDK’s build facilities.

.. _label_component_cpp_plugin_api_main:

FEP Component CPP API
^^^^^^^^^^^^^^^^^^^^^

To create your :term:`FEP Component Implementation` derive your 
:term:`FEP Component Implementation` class publicly from
:cpp:class:`fep3::base::Component<MyComponentInterface>`, e. g.
::

    #include <fep3/components/base/component.h>
    class MyComponent : public fep3::base::Component<MyComponentInterface>
    {
    public:
        // methods implementing MyComponentInterface
        int get() const override
        {
            return 999;
        }
    };

The :term:`FEP Component Interface` ("MyComponentInterface" in the example above) may be one of the
interfaces as defined by the :term:`FEP Participant Library` (see :ref:`Default_Components_vs_Foreign_Components`)
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

A single :term:`FEP Component Implementation` may implement multiple  
:term:`FEP Component Interfaces<FEP Component Interface>` (aka. :term:`FEP Super 
Component`) by listing them as template arguments of 
:cpp:class:`fep3::base::Component`, e. g. 
::
    class MySuperComponent : public fep3::base::Component<MyComponentInterface, MyAdditionalComponentInterface>
    {
        ...
    };

If multiple :term:`FEP Component Implementations<FEP Component Implementation>` 
use the same underlying resources (e. g. in case of separate implementations 
of multiple versions of a :term:`FEP Component Interface`), the implementations 
can be treated as a :term:`FEP Super Component` by wrapping them in  
:cpp:class:`fep3::plugin::cpp::catelyn::ComponentWrapper`, e. g. 
::

    // with
    class MyNewComponentInterface
    {
    protected:
        virtual ~MyNewComponentInterface() = default;

    public:
        FEP_COMPONENT_IID("my_new_component");

        // methods of the component
        virtual float get() const = 0;
    };
    
    class MySuperComponent : public fep3::plugin::cpp::catelyn::ComponentWrapper<MyNewComponent, MyComponentInterface>
    {
        ...
        // forward methods of MyComponentInterface to the wrapped component
        int get() const override
        {
            return std::lround(_component.get());
        }
    };

To create your :ref:`Component CPP Plugin<label_component_cpp_plugins>` 
implement the functions :cpp:func:`fep3_plugin_getPluginVersion` 
and :cpp:func:`fep3_plugin_cpp_catelyn_getFactory`, e. g. like so:
::

    #include <fep3/plugin/cpp/cpp_plugin_impl_base.hpp>

    void fep3_plugin_getPluginVersion(void(*callback)(void*, const char*), void* destination)
    {
        callback(destination, "my component plugin, version 1.0");
    }

    fep3::plugin::cpp::catelyn::IComponentFactory* fep3_plugin_cpp_catelyn_getFactory()
    {
        return new fep3::plugin::cpp::catelyn::ComponentFactory<MyComponent>();
    }

[optional] If your plugin shall be loadable by FEP Participants built against 
:term:`FEP Participant Library` < 3.2, you have to additionally implement the
arya interface :cpp:func:`fep3_plugin_cpp_arya_getFactory`, e. g. like so:
::

    #include <fep3/plugin/cpp/cpp_plugin_impl_arya.hpp>
    fep3::plugin::cpp::arya::ICPPPluginComponentFactory* fep3_plugin_cpp_arya_getFactory()
    {
        static auto component_factory = std::make_shared<fep3::plugin::cpp::arya::ComponentFactory<MyComponent>>();
        return new fep3::plugin::cpp::arya::ComponentFactoryWrapper(component_factory);
    }

.. _label_component_c_plugins:

Component C Plugin [deprecated]
-------------------------------

.. warning::

   C plugins use the C interface of the :ref:`label_component_system` are deprecated and removed from FEP SDK Participant library.
   Loading a c-plugin from :term:`FEP Component Configuration File` will result in an exception.


Creating a FEP Component Plugin in order to provide Foreign FEP Components
--------------------------------------------------------------------------

FEP Component Plugins must be provided as shared libraries.

.. note:: One FEP Component Plugin may contain multiple :term:`FEP Components<FEP Component>` but only one
    :term:`FEP Component Implementation` per :term:`FEP Component Interface`.


.. _label_multiple_components:

Create a FEP Component Plugin which provides multiple FEP Component interfaces
------------------------------------------------------------------------------

In order to write a FEP Component Plugin which provides multiple FEP Component interfaces, you should first define multiple component interfaces, e.g.

.. literalinclude:: ../snippets/snippet_component_plugin.cpp
    :start-after: //Begin(componentA interface)
    :end-before: //End(componentA interface)

.. literalinclude:: ../snippets/snippet_component_plugin.cpp
    :start-after: //Begin(componentB interface)
    :end-before: //End(componentB interface)

The :cpp:func:`FEP_COMPONENT_IID` is the component interface ID. This ID must be unique.

After that, you could create your own components by inheriting :cpp:func:`fep3::base::Component<component_interface_types>`,
whereas the ``component_interface_types`` is your defined component interface, e.g.

.. literalinclude:: ../snippets/snippet_component_plugin.cpp
    :start-after: //Begin(componentA)
    :end-before: //End(componentA)

.. literalinclude:: ../snippets/snippet_component_plugin.cpp
    :start-after: //Begin(componentB)
    :end-before: //End(componentB)

Once you have your own components, you could integrate them into a component plugin.
The function :cpp:func:`createComponent()` will then create the corresponding plugin for you.
With the help of ``component interface ID``, your plugin could refer to different components at runtime.

Furthermore, in order to make your plugin work, you need to provide the function
:cpp:func:`fep3_plugin_getPluginVersion`

.. literalinclude:: ../snippets/snippet_component_plugin.cpp
    :start-after: //Begin(my plugin functions)
    :end-before: //End(my plugin functions)

Now you have successfully created your own component plugin.
To use your own component plugin, you need first add the ``source`` (path where 
the component lib located) and ``ID`` of your component interfaces 
in ``fep3_participant.fep_components``:

.. code-block:: xml

    <component>
        <source type="cpp-plugin">
        ./demo_components_plugin
        </source>
        <iid>IComponentA</iid>
    </component>
    <component>
        <source type="cpp-plugin">
        ./demo_components_plugin
        </source>
        <iid>IComponentB</iid>
    </component>

In your fep element implementation, you could acquire your components in function :cpp:func:`fep3::core::ElementBase::load` by calling :cpp:func:`fep3::getComponent()`:

.. literalinclude:: ../snippets/snippet_component_plugin.cpp
    :start-after: //Begin(my plugin get Component)
    :end-before: //End(my plugin get Component)

The fully worked example can be found in :ref:`label_demo_component_plugin`.

C++ FEP Component development best practices
============================================

* The component interface class should not export additional symbols apart from the free functions referenced in :ref:`_label_component_cpp_plugin_api_main`. Be aware that gcc exports all symbols by default in contrast to Visual Studio.

* Component Interfaces should not use non ABI stable classes in the public API. Regarding FEP Components and Participant library, all classes under namespace *fep3::version* (ex. fep3::arya, fep3::bronn) are ABI stable and can be used in the public API. For example :cpp:class:`fep3::core::DataWriter` should not be used in the public API, whereas :cpp:class:`fep3::arya::IDataRegistry::IDataWriter` can be used. This also means that no *PUBLIC* dependency but only *PRIVATE* dependency allowed to fep3_participant_core, fep3_participant_cpp, fep3_participant_cpp.

* A version should be included in the IID (C++ and RPC) of your component interface. If an ABI increase/change is provided, then the IID should be incremented/changed. Please follow the FEP SDK convention (e.g. simulation_bus.arya.fep3.iid). It is also advised to use a dedicated namespace for each version of your interface and pack your newest namespace version in a type alias that the consumers of the interface should use.

.. code-block:: cpp

    namespace my_component_namespace::version_1{
        // Version 1 code here
        class MyComponentInterface{
        };
    }

    namespace my_component_namespace::version_2{
        // Version 2 code here
        class MyComponentInterface{
        };
    }

    using my_component_namespace::MyComponentInterface = my_component_namespace::version_2::MyComponentInterface;

.. note::
    For more information you could read the `guidelines for FEP Component interfaces <https://www.cip.audi.de/wiki/pages/viewpage.action?pageId=93589002>`_ and the `FEP Component Updates <https://www.cip.audi.de/wiki/display/FEPSDK/FEP+Component+updates#FEPComponentupdates-Severitiesofinterfaceupdates>`_ .

* For the easier usage of the component, it is advisable to create a CMake interface target with the include directories of you component interface headers. It should be adequate in most use cases for the users of the component to link only to the interface target. Also the component itself should not directly link to other component dynamic libraries, just their interface targets.

* It is advised to put each interface is in a separate header. In addition to increased readability, this could make configuring and using an ABI checker easier in case it is introduced the future.

* Make the destructor of the component protected if possible. Rationale: Lifetime of Components is handled by :cpp:class:`fep3::arya::ComponentRegistry`.

* In case the component has dependencies to dynamic libraries, be aware that during installation CMake strips the RPATH information of the installed library. This could lead to problems when the plugin is used externally (for example from a conan package) and its dependencies (for example *libfep3_participantd3.0.so*), may not be found by the loader. More information can be found in the `CMake documentation <https://gitlab.kitware.com/cmake/community/-/wikis/doc/cmake/RPATH-handling>`_ .

FEP Component Registry over RPC
===============================

Information on the loaded :term:`FEP Components<FEP Component>` can be accessed from :term:`RPC Service` component_registry.
The following examples can be used to get information of a :term:`FEP Component Implementation` plugin for a FEP Component IID. The examples are using the command line of fep_control which you find in fep_sdk_base_utilities/3.1.0.

In order to get the file path of the FEP Component Plugin, the :term:`FEP Component` for a FEP Component IID is loaded from, use:

.. code-block:: console

    callRPC demo_system demo_cpp_receiver component_registry component_registry.catelyn.fep3.iid getFilePath '{"service_iid" : "service_bus.catelyn.fep3.iid"}'

In order to get the version of the loaded :term:`FEP Component` for a :term:`FEP Component` IID, the following RPC call can be used:

.. code-block:: console

    callRPC demo_system demo_cpp_receiver component_registry component_registry.catelyn.fep3.iid getPluginVersion '{"service_iid" : "service_bus.catelyn.fep3.iid"}'

In order to get the participant library version used to build the loaded FEP Component Plugin of a :term:`FEP Component` IID, the following RPC call can be used:

.. code-block:: console

    callRPC demo_system demo_cpp_receiver component_registry component_registry.catelyn.fep3.iid getParticipantLibraryVersion '{"service_iid" : "service_bus.catelyn.fep3.iid"}'

In order to get the FEP Component IIDs of all :term:`FEP Components<FEP Component>` as loaded by the participant, the following RPC call can be used:

.. code-block:: console

    callRPC demo_system demo_cpp_receiver component_registry component_registry.catelyn.fep3.iid getComponentIIDs '{}'

.. note::
   *getComponentIIDs* is available in Participant version 3.2.0 or later. The rest of the :term:`RPC Service` functions are available from version
   3.1.0 or later.

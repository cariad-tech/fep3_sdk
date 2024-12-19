.. Copyright 2023 CARIAD SE.
..
.. This Source Code Form is subject to the terms of the Mozilla 
.. Public License, v. 2.0. If a copy of the MPL was not distributed 
.. with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
========================
Configurable FEP Element
========================

.. note:: 
   
   The :cpp:class:`fep3::core::ElementConfigurable` and :cpp:class:`fep3::cpp::DataJobElement` will be deprecated in the future. Please use :ref:`customized_element`.

The :cpp:class:`fep3::core::ElementConfigurable` provides the ability to develop a FEP Element that has a configuration. A configuration contains properties that are not
meant to be changed periodically, but can be used to set static data, for more details see Configuration Service :ref:`label_configuration_service_overview`.

The convenience class :cpp:class:`fep3::core::ElementConfigurable` can be used for easy setting up a configurable FEP Element:

.. literalinclude:: ../snippets/snippet_core_sender.cpp
    :start-at: class EasyCoreSenderElement
    :end-at: class EasyCoreSenderElement

The properties should be then member of the element class:

.. literalinclude:: ../snippets/snippet_core_sender.cpp
    :start-at: base::PropertyVariable<int32_t>
    :end-at: base::PropertyVariable<int32_t>

and registered in the constructor:

.. literalinclude:: ../snippets/snippet_core_sender.cpp
    :start-at: registerPropertyVariable();
    :end-at: registerPropertyVariable();

Then the updated element value can be accessed only after calling  :cpp:func:`fep3::base::Configuration::updatePropertyVariables`

.. literalinclude:: ../snippets/snippet_core_sender.cpp
    :start-at: updatePropertyVariables();
    :end-at: updatePropertyVariables();

.. literalinclude:: ../snippets/snippet_core_sender.cpp
    :start-at: = _prop_to_send_as_integer
    :end-at: = _prop_to_send_as_integer

By inheriting from cpp:class:`fep3::core::ElementConfigurable`, the participant will have a property node named *element*. In this example, assuming the system is called *demo_system*
and the participant *demo_participant*, the property value can be accessed from the *fep_control* as follows:

.. code-block:: console

    getParticipantProperty demo_system demo_core_sender element/integer_value

If the property value has to be set to *5* for example, this can also be done from the *fep_control*:

.. code-block:: console

    setParticipantProperty demo_system demo_core_sender element/integer_value 5

Properties can also be changed programmatically as shown in :ref:`label_configuration_service_overview`.
It is also possible to have an observer reacting on a property value change, see :ref:`label_property_observers`.

.. note:: Regarding the proper use cases for properties please see also :ref:`label_using_properties_in_fep`

.. _label_component_property_table:

Component Properties
====================

The following table summarizes the properties that are used by the FEP components. Along with the member function of the compoment, in which the
:cpp:func:`fep3::base::Configuration::updatePropertyVariables` is called, to let the new value take effect.

+------------------------------------+----------------+-------------------------------------------------+-----------------------+-----------------------------------+---------------+
|  Component                         | Type           |          Property Name                          | Default Value         | updatePropertyVariables() called  |  Reference    |
+====================================+================+=================================================+=======================+===================================+===============+
| :ref:`label_clock_service`         | std::string    | clock/main_clock                                | local_system_realtime | tense()                           |               |
+------------------------------------+----------------+-------------------------------------------------+-----------------------+-----------------------------------+---------------+
| :ref:`label_clock_service`         | int64_t        | clock/time_update_timeout                       | 5000000000            | tense()                           |               |
+------------------------------------+----------------+-------------------------------------------------+-----------------------+-----------------------------------+---------------+
| :ref:`label_clock_service`         | double         | clock/time_factor                               | 1.0                   | tense()                           |               |
+------------------------------------+----------------+-------------------------------------------------+-----------------------+-----------------------------------+---------------+
| :ref:`label_clock_service`         | int64_t        | clock/step_size                                 | 100000000             | tense()                           |               |
+------------------------------------+----------------+-------------------------------------------------+-----------------------+-----------------------------------+---------------+
| :ref:`label_clock_sync_service`    | std::string    | clock_synchronization/timing_master             | ""                    | initialize()                      |               |
+------------------------------------+----------------+-------------------------------------------------+-----------------------+-----------------------------------+---------------+
| :ref:`label_clock_sync_service`    | int64_t        | clock_synchronization/sync_cycle_time           | 100000000             | initialize()                      |               |
+------------------------------------+----------------+-------------------------------------------------+-----------------------+-----------------------------------+---------------+
| :ref:`label_data_registry`         | std::string    | data_registry/renaming_input                    | ""                    | initialize(), registerDataIn()    |               |
+------------------------------------+----------------+-------------------------------------------------+-----------------------+-----------------------------------+---------------+
| :ref:`label_data_registry`         | std::string    | data_registry/renaming_output                   | ""                    | initialize(), registerDataIn()    |               |
+------------------------------------+----------------+-------------------------------------------------+-----------------------+-----------------------------------+---------------+
| :ref:`label_data_registry`         | std::string    | data_registry/mapping_configuration_file_path   | ""                    | initialize(), registerDataIn()    |               |
+------------------------------------+----------------+-------------------------------------------------+-----------------------+-----------------------------------+---------------+
| :ref:`label_data_registry`         | vactor<string> | data_registry/mapping_ddl_file_paths            | ""                    | initialize(), registerDataIn()    |               |
+------------------------------------+----------------+-------------------------------------------------+-----------------------+-----------------------------------+---------------+
| :ref:`label_logging_service`       | std::string    | logging/default_sinks                           | console               | initialize()                      |               |
+------------------------------------+----------------+-------------------------------------------------+-----------------------+-----------------------------------+---------------+
| :ref:`label_logging_service`       | int32_t        | logging/default_severity                        | LoggerSeverity::info  | initialize()                      |               |
+------------------------------------+----------------+-------------------------------------------------+-----------------------+-----------------------------------+---------------+
| :ref:`label_logging_service`       | std::string    | logging/default_sink_file                       | ""                    | initialize()                      |               |
+------------------------------------+----------------+-------------------------------------------------+-----------------------+-----------------------------------+---------------+
| :ref:`label_scheduler_service`     | std::string    | scheduling/scheduler                            | clock_based_scheduler | tense()                           |               |
+------------------------------------+----------------+-------------------------------------------------+-----------------------+-----------------------------------+---------------+
| :ref:`label_simulation_bus`        | int32_t        | rti_dds_simulation_bus/participant_domain       | 5                     | initialize()                      |               |
+------------------------------------+----------------+-------------------------------------------------+-----------------------+-----------------------------------+---------------+
| :ref:`label_simulation_bus`        | int64_t        | rti_dds_simulation_bus/datawriter_ready_timeout | 0                     | tense(), registerDataIn()         |               |
+------------------------------------+----------------+-------------------------------------------------+-----------------------+-----------------------------------+---------------+
| :ref:`label_simulation_bus`        | vector<string> | rti_dds_simulation_bus/must_be_ready_signals    | "" (empty list)       | tense(), registerDataIn()         |               |
+------------------------------------+----------------+-------------------------------------------------+-----------------------+-----------------------------------+---------------+
| :ref:`label_simulation_bus`        | boolean        | rti_dds_simulation_bus/use_async_waitset        | false                 | tense()                           |               |
+------------------------------------+----------------+-------------------------------------------------+-----------------------+-----------------------------------+---------------+
| :ref:`label_simulation_bus`        | int32_t        | rti_dds_simulation_bus/async_waitset_threads    | 8                     | tense()                           |               |
+------------------------------------+----------------+-------------------------------------------------+-----------------------+-----------------------------------+---------------+

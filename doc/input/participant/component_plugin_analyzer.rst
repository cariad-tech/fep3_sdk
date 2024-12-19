.. Copyright 2023 CARIAD SE.
..
.. This Source Code Form is subject to the terms of the Mozilla 
.. Public License, v. 2.0. If a copy of the MPL was not distributed 
.. with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
.. _Platform Notes:


==========================
Component Plugin Analyzer
==========================

Overview
_________

The FEP Participant library also delivers a :term:`FEP Component` analysis tool.
This tool could be useful in case you want to get an overview of the :term:`inter-component<FEP Component>` 
dependencies, and the properties and signals that each :term:`FEP Component` is creating.
The tool is also able to generate a FEP metamodel schema compatible output. 

Using the analysis tool in your project
________________________________________

The analysis tool comes as a library that you can link to.


CMake Config
-------------

The library has a cmake target that you can use in your cmake target by writing the following
lines in your cmake file.

.. literalinclude:: ../snippets/CMakeLists.txt
    :start-after: #start_cmake_snippet_component_plugin_analyzer
    :end-before: #end_cmake_snippet_component_plugin_analyzer

Calling the analyzer
--------------------

Include the header in your code 

.. literalinclude:: ../snippets/snippet_component_plugin_analyzer.cpp
    :start-after: //start_include_snippet_component_plugin_analyzer
    :end-before: //end_include_snippet_component_plugin_analyzer


And then construct the object. The set of :term:`FEP components<FEP Component>` given should be able to be cycled
through states, meaning that the set should satisfy all the :term:`component<FEP Component>` interdependencies.

.. literalinclude:: ../snippets/snippet_component_plugin_analyzer.cpp
    :start-after: //start_construct_snippet_component_plugin_analyzer
    :end-before: //end_construct_snippet_component_plugin_analyzer



In order to trigger a :cpp:class:`fep3::arya::ComponentRegistry` state transition you have to call the function 
:cpp:func:`fep3::base::ComponentPluginAnalyzer::analyzeStateTransition` with a corresponding callback:

.. literalinclude:: ../snippets/snippet_component_plugin_analyzer.cpp
    :start-after: //start_analyze_state_snippet_component_plugin_analyzer
    :end-before: //end_analyze_state_snippet_component_plugin_analyzer

In case you need to trigger a separate :term:`FEP Component` function, for example a property needs to be set or
an additional :term:`component<FEP Component>` function needs to be called because it adds new properties or signals, the
function :cpp:func:`fep3::base::ComponentPluginAnalyzer::analyzeComponentFunction` can be used as
follows:

.. literalinclude:: ../snippets/snippet_component_plugin_analyzer.cpp
    :start-after: //start_analyze_comp_function_snippet_component_plugin_analyzer
    :end-before: //end_analyze_comp_function_snippet_component_plugin_analyzer

.. note::
   The analyzer does not perform any rollbacks as the FEP State Machine does in 
   case a :term:`FEP Component` state transition fails, but breaks the execution immediately on
   a :term:`FEP Component` transition failure.

**After** all the state transition functions and :term:`FEP Component` function calls are executed,
then the last call will be to retrieve the analysis results and have them printed in the console: 

.. literalinclude:: ../snippets/snippet_component_plugin_analyzer.cpp
    :start-after: //start_print_snippet_component_plugin_analyzer
    :end-before: //end_print_snippet_component_plugin_analyzer

The program output should look similar to the following:


.. code-block:: bash

    Analysis Result
    components:
        - iid:
            - logging_service.arya.fep3.iid
            type: cpp-plugin
            path: lib
            required_components:
            - clock_service.arya.fep3.iid
            - clock_service.experimental.fep3.iid
            - configuration_service.arya.fep3.iid
            - service_bus.catelyn.fep3.iid
            configure:
            "/logging/default_sinks":
                description: ""
                type: string
                available_in_state: UNLOADED
                default_value: console,rpc
            "/logging/default_severity":
                description: ""
                type: int32
                available_in_state: UNLOADED
                default_value: 4
            "/logging/default_sink_file":
                description: ""
                type: string
                available_in_state: UNLOADED
                default_value: ""
        - iid:
            - configuration_service.arya.fep3.iid
            type: cpp-plugin
            path: lib
            required_components:
            - service_bus.catelyn.fep3.iid


How everything works
---------------------

The tool performs a couple of checks before and after each :term:`FEP Component` state transition.
The same applies for calls to :cpp:func:`fep3::base::ComponentPluginAnalyzer::analyzeComponentFunction` .
These checks identify the properties and signals that are inserted after each :term:`component<FEP Component>` call.
Additionally, the calls to :cpp:func:`fep3::arya::IComponents::findComponent` during the :term:`component<FEP Component>` function call
are saved as :term:`component<FEP Component>` dependencies. The analyzer does not track any used environment variables, and therefore
it cannot provide a full :term:`FEP Component` Description.

.. note::
   The analyzer does not escape any characters contained for example in property names or values
   that would make the value yaml compatible.

.. note::
   The analyzer tracks all the :term:`FEP Component` iids that a consuming :term:`component<FEP Component>` tries to access. This
   however does not mean, that without this :term:`component<FEP Component>`, the consumer :term:`FEP Component` does not function.
   A :term:`FEP Component` could have  fall-back mechanisms in case a :term:`component<FEP Component>` dependency is not found.

.. note::
   The analyzer can analyze only properties, signals or dependencies that are invoked during the state
   transition, this means that operations happening outside the thread calling the state transition cannot be
   tracked (at least reliably).

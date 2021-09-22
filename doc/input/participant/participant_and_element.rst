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


.. highlight:: cpp

.. _label_participant:

===========
Participant
===========

.. _label_participant_creation:

Creation
========

A participant instance has to be created using the function :cpp:func:`fep3::arya::createParticipant` or any of its overloads.
There are various convenience wrappers of this function with the same name in other namespaces:

* :cpp:func:`fep3::core::arya::createParticipant`
* :cpp:func:`fep3::cpp::arya::createParticipant`

These convenience wrappers serve common use cases and require less arguments to be passed. For stand-alone participant executables
it is recommended to use those overloads that take argc and argv as arguments in order to enable command line argument parsing,
so that parameters like participant or system name can be changed on startup and are not hard-coded into the participant.

createParticipant convenience wrappers of the Cpp Library
----------------------------------------------------------

These convenience wrappers are for simple participants where the entire implementation can be described with a :cpp:class:`fep3::cpp::arya::DataJob` and no custom element is required.

.. literalinclude:: ../snippets/snippet_participant.cpp
    :start-after: //Begin(CppBasic)
    :end-before: //End(CppBasic)

To add user defined command line arguments a parser object has to be created and passed to the createParticipant function.

.. literalinclude:: ../snippets/snippet_participant.cpp
    :start-after: //Begin(CppUserArg)
    :end-before: //End(CppUserArg)

createParticipant convenience wrappers of the Core Library
----------------------------------------------------------

These convenience wrappers are for participants with custom elements and also allow to define the element version and a default value for the system address url.

.. literalinclude:: ../snippets/snippet_participant.cpp
    :start-after: //Begin(CoreBasic)
    :end-before: //End(CoreBasic)

This will make the optional arguments required and changes how the executable is called:

With default values defined:

.. code-block:: none

    > myParticipantExecutable -n MyParticipant -s MySystem

Without default values defined:

.. code-block:: none

    > myParticipantExecutable MyParticipant MySystem

If command line parsing is not required, possible or sensible then this overload without argc/argv can be used:

.. literalinclude:: ../snippets/snippet_participant.cpp
    :start-after: //Begin(CoreNoParsing)
    :end-before: //End(CoreNoParsing)

Adding user defined command line arguments is identical to the cpp library. Only the :cpp:func:`fep3::arya::createParticipant` call is slightly different.

.. literalinclude:: ../snippets/snippet_participant.cpp
    :start-after: //Begin(CoreUserArg)
    :end-before: //End(CoreUserArg)

.. _label_participant_execution:

Execution
=========

After instantiating the participant, it has to be executed via :cpp:func:`fep3::arya::Participant::exec`. If the execution succeeds,
this method blocks until the :ref:`label_participant_state_machine` reaches the state ``Final``.


.. _label_participant_state_machine:

State Machine
=============

A participant instance has the following state machine:

.. image:: images/participant_statemachine.svg

.. attention::
    State 'Paused' is not supported yet. Trying to set a participant and its components to state 'Paused' results in an error.

Once the participant instance has been created, its state machine is in state "Unloaded". The state machine can then be controlled by events via
the :ref:`label_service_bus`.

The following table shows the calls to the FEP Participant API as performed by the element-related guards and actions (postfixed by ``..._element`` in the state machine diagram):

+-------------------------------------------+----------------------------------------------------------+
| guard/action                              | calls on the FEP Participant API                         |
+===========================================+==========================================================+
| load_element                              | :cpp:func:`fep3::arya::IElementFactory::createElement`   |
|                                           | :cpp:func:`fep3::arya::IElement::loadElement`            |
+-------------------------------------------+----------------------------------------------------------+
| unload_element                            | :cpp:func:`fep3::arya::IElement::unloadElement`          |
|                                           | :cpp:func:`fep3::arya::~IElement`                        |
+-------------------------------------------+----------------------------------------------------------+
| initialize_element                        | :cpp:func:`fep3::arya::IElement::initialize`             |
+-------------------------------------------+----------------------------------------------------------+
| deinitialize_element                      | :cpp:func:`fep3::arya::IElement::deinitialize`           |
+-------------------------------------------+----------------------------------------------------------+
| run_element                               | :cpp:func:`fep3::arya::IElement::run`                    |
+-------------------------------------------+----------------------------------------------------------+
| stop_element                              | :cpp:func:`fep3::arya::IElement::stop`                   |
+-------------------------------------------+----------------------------------------------------------+

Note: For a description of the calls to the FEP Component API as performed by the component-related guards and actions (postfixed by ``..._components`` in the state machine diagram),
please refer to :ref:`label_calls_to_component`.

.. _label_element_interface:

=================
Element Interface
=================

The element interface describes the structure of an element.
The participant is able to load and manage a FEP Element implementation via an Element Factory.
This interface is described as follows:

.. doxygenclass:: fep3::arya::IElement
   :members:




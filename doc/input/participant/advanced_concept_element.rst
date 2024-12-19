.. Copyright 2023 CARIAD SE.
..
.. This Source Code Form is subject to the terms of the Mozilla 
.. Public License, v. 2.0. If a copy of the MPL was not distributed 
.. with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
===========
FEP Element
===========

Embedding the element in a FEP participant
==========================================

A custom Element should inherit from :cpp:class:`fep3::core::ElementConfigurable` or :cpp:class:`fep3::core::ElementBase`.

.. literalinclude:: ../snippets/snippet_core_receiver_main.cpp
    :start-at: class EasyCoreReceiverElement
    :end-before: //Implementation of the CTOR!

.. literalinclude:: ../snippets/snippet_core_receiver_main.cpp
    :start-at: EasyCoreReceiverElement()
    :end-at: FEP3_PARTICIPANT_LIBRARY_VERSION_STR)

In order to create an object of :cpp:class:`fep3::base::Participant`  there are some overloads of the function to choose from in
:cpp:func:`fep3::arya::createParticipant` , :cpp:func:`fep3::cpp::arya::createParticipant`, :cpp:func:`fep3::core::arya::createParticipant`,
providing different alternatives for initialization.

In this example the following overload is used
:cpp:class:`template\<typename element_factory> createParticipant(int , char const *const *, const std::string& , const fep3::core::ParserDefaultValues&)`
that provides also the possibility of parsing the command line arguments.

.. literalinclude:: ../snippets/snippet_core_receiver_main.cpp
    :start-at: createParticipant
    :end-at: "" });

The :cpp:class:`fep3::core::IElementFactory` can either be passed as a template parameter or as a constructor argument and it is the interface that will eventually call
the constructor of the custom Element Class and create an object instance during the *load_element* action of the :ref:`label_participant_state_machine`.
The destructor of the created object is called in the *unload_element* action of the :ref:`label_participant_state_machine`.
The lifetime of the created Factory object on the other hand is equal to the lifetime of the participant, meaning there is only one instance of :cpp:class:`fep3::core::IElementFactory`
created during the :cpp:class:`fep3::base::Participant`'s lifetime.

.. _label_create_participant:

Creating participant
====================

A participant instance has to be created using the function :cpp:func:`fep3::arya::createParticipant` or any of its overloads.
There are various convenience wrappers of this function with the same name in other namespaces:

* :cpp:func:`fep3::core::arya::createParticipant`
* :cpp:func:`fep3::cpp::arya::createParticipant`

These convenience wrappers serve common use cases and require less arguments to be passed. For stand-alone participant executables
it is recommended to use those overloads that take 'argc' and 'argv' as arguments in order to enable command line argument parsing,
so that parameters like name of participant or system can be changed on startup and are not hard-coded into them.

*createParticipant* convenience wrappers of the CPP Library
-----------------------------------------------------------

These convenience wrappers are for simple participants where the entire implementation can be described with a :cpp:class:`fep3::cpp::arya::DataJob` and no custom element is required.

.. literalinclude:: ../snippets/snippet_participant.cpp
    :start-after: //Begin(CppBasic)
    :end-before: //End(CppBasic)

To add user defined command line arguments a parser object has to be created and passed to the *createParticipant* function.

.. literalinclude:: ../snippets/snippet_participant.cpp
    :start-after: //Begin(CppUserArg)
    :end-before: //End(CppUserArg)

*createParticipant* convenience wrappers of the Core Library
------------------------------------------------------------

These convenience wrappers are for participants with custom elements and also allow to define the element version and a default value for the system address URL.

.. literalinclude:: ../snippets/snippet_participant.cpp
    :start-after: //Begin(CoreBasic)
    :end-before: //End(CoreBasic)

The element executable can be started by for example:

.. code-block:: none

    > myParticipantExecutable --element_name MyParticipant --system_name MySystem

Element name can be given by ``--element_name``, ``--name`` or ``-n``,
while system name can be given by ``--system_name``, ``--system`` or ``-s``.

If no default values are defined (i.e. empty strings are given), these arguments are mandatory.

When having access to 'argc/argv', they are strongly recommended to be passed to your participant,
unless other means are provided to configure the name of your participant(s) and system, like reading them
from a file or from environment variables.

.. literalinclude:: ../snippets/snippet_participant.cpp
    :start-after: //Begin(CoreNoParsing)
    :end-before: //End(CoreNoParsing)

Adding user defined command line arguments is identical to the CPP library. Only the :cpp:func:`fep3::arya::createParticipant` call is slightly different.

.. literalinclude:: ../snippets/snippet_participant.cpp
    :start-after: //Begin(CoreUserArg)
    :end-before: //End(CoreUserArg)


.. _label_participant_execution:

Execution
=========

After instantiating the participant, it has to be executed via :cpp:func:`fep3::base::Participant::exec`. If the execution succeeds,
this method blocks until the :ref:`label_participant_state_machine` reaches the state ``Finalized``.


.. _label_participant_state_machine:

State Machine
=============

A participant instance has the following state machine:

.. image:: images/participant_statemachine.svg

.. attention::
    State 'Paused' is not supported yet. Trying to set a participant and its components to state 'Paused' results in an error.

Once the participant instance has been created, its state machine is in state 'Unloaded'. The state machine can then be controlled by events via
the :ref:`label_service_bus`.

The following table shows the calls to the FEP Participant API as performed by the element-related guards and actions (postfixed by ``..._element`` in the state machine diagram):

+-------------------------------------------+----------------------------------------------------------+
| guard/action                              | calls on the FEP Participant API                         |
+===========================================+==========================================================+
| load_element                              | :cpp:func:`fep3::core::IElementFactory::createElement`   |
|                                           | :cpp:func:`fep3::base::IElement::loadElement`            |
+-------------------------------------------+----------------------------------------------------------+
| unload_element                            | :cpp:func:`fep3::base::IElement::unloadElement`          |
|                                           | :cpp:func:`fep3::base::~IElement`                        |
+-------------------------------------------+----------------------------------------------------------+
| initialize_element                        | :cpp:func:`fep3::base::IElement::initialize`             |
+-------------------------------------------+----------------------------------------------------------+
| deinitialize_element                      | :cpp:func:`fep3::base::IElement::deinitialize`           |
+-------------------------------------------+----------------------------------------------------------+
| run_element                               | :cpp:func:`fep3::base::IElement::run`                    |
+-------------------------------------------+----------------------------------------------------------+
| stop_element                              | :cpp:func:`fep3::base::IElement::stop`                   |
+-------------------------------------------+----------------------------------------------------------+

Note: For a description of the calls to the FEP Component API as performed by the component-related guards and actions (postfixed by ``..._components`` in the state machine diagram),
please refer to :ref:`label_calls_to_component`.

What to do in what state?
-------------------------

Load
~~~~
This state was introduced in FEP3 to solve a specific workflow problem, which arises when the simulation loads big external models and needs to be configurable afterwards.
If you have an external model, you should load it here and create the needed properties for configuration of the model being loaded beforehand in the state unloaded. 
See :ref:`label_configuration_service`. It is also good practice to register your FEP Job(s) and FEP signals here, so they can be reconfigured.

Initialize
~~~~~~~~~~
The user configuration process should be finished by calling initialize, so you can execute a 0th simulation step (without reading and writing to FEP), if you expect a lot of initialization going on in the model.

Start
~~~~~
This transition should be almost instantaneous and everything should be ready by now, so that your participant does not hold back the whole system start.

Stop
~~~~
Reset the simulation state of the interface, so a start→ stop→start does not result in a pseudo "Pause".

Deinitialize
~~~~~~~~~~~~
Unregister everything that was done in initialize.

Unload
~~~~~~
Unregister all properties and everything that was done in load. If you loaded a model, clean up its resources.

.. _label_element_interface:


Implementing the :cpp:func:`fep3::core::ElementBase::load`  and :cpp:func:`fep3::core::ElementBase::initialize`
===============================================================================================================

Typically in the :cpp:func:`fep3::core::ElementBase::load` overload of the custom Element class the job is registered

.. literalinclude:: ../snippets/snippet_core_receiver_main.cpp
    :start-at: load()
    :end-before: //End(load)

whereas in the :cpp:func:`fep3::core::ElementBase::initialize` any data receivers or writers of the Element are registered.

.. literalinclude:: ../snippets/snippet_core_receiver_main.cpp
    :start-at: initialize()
    :end-before: //End(initialize)

For loading external data in the Element class (for example from an external file), either of these two functions could be used.

Cleanup in state transitions and error handling
-----------------------------------------------

A common mistake is to forget to clean up after a state machine change, especially, if it was cancelled somewhere in the middle, 
due to a user error. If these measures are not taken, the state machine will be stuck until the participant is restarted.

You need to think about a rollback mechanism.

1. Always check, that your state machine callbacks are cleaned up, when leaving the same state. Example: If you register your jobs in initialize, 
check that you deregister them in deinitialize.

2. If your implemented state callback functions returns a fep3::Result code which differs from 0 == OK always cleanup everything you did, 
whether it is registering anything at any FEP Components, or acquiring memory. If you're developing in C++, you can use the RAII pattern for this.

.. Use "Partial Example for robust State Changes with Error Handling" but change it to RAII 


Using a custom *ElementFactory*
===============================

In some use cases the implementation of a custom *ElementFactory* should be necessary. For example, in cases that additional arguments should be
passed to the constructor of the Element Class.

In this exemplary use case, some command line arguments have to be forwarded to the Element Class constructor.

The :cpp:class:`CustomElementFactory` is defined

.. literalinclude:: ../snippets/snippet_element_factory.cpp
    :start-at: class CustomElementFactor
    :end-before: //End(CustomElementFactory)

The constructor of :cpp:class:`EasyCoreReceiverElement` is now modified to accommodate for the additional arguments

.. literalinclude:: ../snippets/snippet_element_factory.cpp
    :start-at: EasyCoreReceiverElement(
    :end-at: file_path))

that are used in the :cpp:func:`EasyCoreReceiverElement::process` function

.. literalinclude:: ../snippets/snippet_element_factory.cpp
    :start-at: fep3::Optional<int32_t> received_plain_value;
    :end-before: return {};

The constructor simply stores the command line parameters and forwards them in the constructor :cpp:func:`EasyCoreReceiverElement::EasyCoreReceiverElement` when
:cpp:func:`CustomElementFactory::createElement` is called during the *load_element* action of :ref:`label_participant_state_machine`.

.. literalinclude:: ../snippets/snippet_element_factory.cpp
    :start-at: createElement(
    :end-before: //End(createElement)

So, by using a custom factory, the main function should look like this:

.. literalinclude:: ../snippets/snippet_element_factory.cpp
    :start-at: int main(

.. _label_configurable_fep_element:

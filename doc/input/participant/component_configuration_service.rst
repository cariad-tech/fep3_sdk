.. Copyright @ 2021 VW Group. All rights reserved.
.. 
.. This Source Code Form is subject to the terms of the Mozilla 
.. Public License, v. 2.0. If a copy of the MPL was not distributed 
.. with this file, You can obtain one at https://mozilla.org/MPL/2.0/.


.. _label_configuration_service:

=====================
Configuration Service
=====================

.. sidebar::  Configuration Service

        .. contents::

Summary
-------

+-------------------------+----------------------------------------------------+-----+
| Name                    | Configuration Service                              |     |
+-------------------------+----------------------------------------------------+-----+
| Component Interface     | :cpp:class:`fep3::arya::IConfigurationService`     |     |
+-------------------------+----------------------------------------------------+-----+
| RPC Service Interface   | :cpp:class:`fep3::rpc::arya::IRPCConfigurationDef` |     |
|                         | :cpp:class:`fep3::rpc::arya::IRPCConfiguration`    |     |
+-------------------------+----------------------------------------------------+-----+
| RPC Service Description | :ref:`label_rpc_service_configuration_service`     |     |
+-------------------------+----------------------------------------------------+-----+
| native delivery         | CPP-plugin                                         |     |
+-------------------------+----------------------------------------------------+-----+
| CPP-plugin possible     | yes                                                |     |
+-------------------------+----------------------------------------------------+-----+
| C-plugin possible       | yes                                                |     |
+-------------------------+----------------------------------------------------+-----+

.. _label_configuration_service_overview:

Overview
--------

The purpose of the :ref:`label_configuration_service` is to store any static data that does not change periodically during runtime.
This can be configuration data, additional information regarding a simulation (e.g. physical dimension of objects) or other types of data.
Abusing the property tree by storing frequently updated simulation data is not recommended.

The :ref:`label_configuration_service` stores data as properties.
A property consists of a name, a type, a value and may contain multiple children.
Properties are typed and support the following data types:

* *int32*
* *int64*
* *double*
* *bool*
* *string*
* *node*

A property of type *node* does not contain a value but may have children in form of sub properties.
Once a property node is typed it cannot be modified to contain value of a different type.

Those properties are arranged in a tree-like structure and can therefore represent hierarchical units of information.
A property which is a child of another property is called a *subproperty*.
To access a property, the path to that property has to be known.
The path is constructed of all parent properties of your target property, separated by a "/".
Here's an example of a property path:

.. code-block:: html

  "Vehicle/Size/Length"

"Vehicle" is the top-level property in the path and a *subproperty* of the root property node "/".
It has a *subproperty* called "Size" which itself has a *subproperty* "Length".

Component Interface
-------------------

The :cpp:class:`fep3::arya::IConfigurationService` interface has a root node which can be thought of as "/".
This root node can contain an arbitrary amount of property nodes.
The interface offers the following functionality:

* Register property nodes at the root node
* Unregister property nodes
* Check node existence
* Retrieve property nodes

Properties
----------

Assuming we want to create the following property hierarchy:

.. list-table::
   :header-rows: 1
   :widths: 60 20 20

   * - Property
     - Type
     - Value
   * - Vehicle/Type
     - string
     - car
   * - Vehicle/Size/Length
     - double
     - 4.5
   * - Vehicle/Size/Width
     - double
     - 2.1
   * - Vehicle/Seats
     - int32
     - 5
   * - Vehicle/Available
     - bool
     - false

After retrieving the :cpp:class:`fep3::arya::IConfigurationService` component:

.. code-block:: cpp

  fep3::base::Participant participant = [...]
  auto configuration_service = participant.getComponent<fep3::arya::IConfigurationService>();

we can create, modify and read properties via this component.

.. note:: While we can read and modify properties which we receive from the configuration service, we can neither extend these properties by adding sub properties nor can we register property observers or variables for them. To do so we have to create property nodes ourselves and register them at the configuration service.

Create properties
~~~~~~~~~~~~~~~~~

Using the configuration service component and a convenience function *makeNativePropertyNode* we can create typed property nodes.

First of all, we need a property node which will be registered at the root node ("/") of the configuration service.
The configuration service handles property nodes as shared pointers.
Therefore, we create a shared pointer of a *NativePropertyNode* which will contain our sub properties:

.. literalinclude:: ../snippets/snippet_config_service.cpp
  :start-after: //Begin(Create main property)
  :end-before: //End(Create main property)
  :emphasize-lines: 1

Afterwards, we create an empty *subproperty* of type *node* called "Size" which will contain two more sub properties of type *double* called "Length" and "Width".
Additionally, we create typed *subproperty* nodes and set them as children for the corresponding parent property nodes.
As a result, we will have a property hierarchy as shown above:

.. literalinclude:: ../snippets/snippet_config_service.cpp
  :start-after: //Begin(Create subproperties)
  :end-before: //End(Create subproperties)
  :emphasize-lines: 1, 2, 3, 4, 7, 8

Finally, we need to register the main property node "Vehicle" at the configuration service:

.. literalinclude:: ../snippets/snippet_config_service.cpp
  :start-after: //Begin(Register main node)
  :end-before: //End(Register main node)
  :emphasize-lines: 1

Set property values
~~~~~~~~~~~~~~~~~~~

After creation of properties, we can utilize the configuration service and a convenience function *setPropertyValue* to set property values.
Due to the fact that properties are typed, we cannot set mismatching property values for property nodes.
If setting a property value fails, an error is returned.

In the state unloaded the full feature set of the Configuration Service is available, so it is possible 
to create properties. This can be very useful, if you want to load an external model,
and e.g. you add a property called "model_path", which is then evaluated in load.


To set a property value we can provide the *configuration service*, *property path* and *value*:

.. literalinclude:: ../snippets/snippet_config_service.cpp
  :start-after: //Begin(Set property value by service)
  :end-before: //End(Set property value by service)
  :emphasize-lines: 1

Alternatively, we can provide a *property node* and the corresponding *value*:

.. literalinclude:: ../snippets/snippet_config_service.cpp
  :start-after: //Begin(Set property value by node)
  :end-before: //End(Set property value by node)
  :emphasize-lines: 1


Read property values
~~~~~~~~~~~~~~~~~~~~

After creation of properties, we can utilize the configuration service and a convenience function *getPropertyValue* to read property values.
Values are returned in form of typed optional. We may check for a value and retrieve the value.
Alternatively, we may simply dereference it to retrieve the corresponding value:

.. literalinclude:: ../snippets/snippet_config_service.cpp
  :start-after: //Begin(Reading property by service)
  :end-before: //End(Reading property by service)
  :emphasize-lines: 1

Alternatively, we can provide a *property node*.
In this case no optional but the value itself is returned.
In case of failure a default value is returned.

.. literalinclude:: ../snippets/snippet_config_service.cpp
  :start-after: //Begin(Reading property by node)
  :end-before: //End(Reading property by node)
  :emphasize-lines: 1

Trying to get a property with non-existing path returns an optional with no value:

.. literalinclude:: ../snippets/snippet_config_service.cpp
  :start-after: //Begin(get not existing property)
  :end-before: //End(get not existing property)
  :emphasize-lines: 1

Retrieve property nodes
~~~~~~~~~~~~~~~~~~~~~~~

Existing property nodes can be retrieved from the configuration service.
Retrieved nodes may be used to read information like *value*, *type* and *children* or to modify values:

.. literalinclude:: ../snippets/snippet_config_service.cpp
  :start-after: //Begin(retrieve property node)
  :end-before: //End(retrieve property node)
  :emphasize-lines: 1

If we do not have to modify values, we can retrieve const nodes to retrieve information without being able to modify the node:

.. literalinclude:: ../snippets/snippet_config_service.cpp
  :start-after: //Begin(retrieve const property node)
  :end-before: //End(retrieve const property node)
  :emphasize-lines: 1

Modify properties at runtime
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Changing the value of properties at runtime is possible:

.. literalinclude:: ../snippets/snippet_config_service.cpp
  :start-after: //Begin(changing value)
  :end-before: //End(changing value)
  :emphasize-lines: 1

Changing the type of a property at runtime is **not** possible.
The following property was declared as *double* and trying to change it to *int32_t* returns an error:

.. literalinclude:: ../snippets/snippet_config_service.cpp
  :start-after: //Begin(changing type)
  :end-before: //End(changing type)
  :emphasize-lines: 1

Property variables
------------------
Property variables represent variables which are connected to a specific property node.
Their value is updated with the corresponding property node value every time we call *updateObservers* on the property node.

.. note:: With this feature, you don't have to implement a manual synchronization from properties to internal variables in the targeted state change, like in FEP2.

To use a property variable, we create and register it at a property node.
During registration the value of the property node is set to the value of the property variable.

.. note:: The property node to register the property variable at has to be created by us for registration property variables.

.. literalinclude:: ../snippets/snippet_config_service.cpp
  :start-after: //Begin(register property variable)
  :end-before: //End(register property variable)
  :emphasize-lines: 2

To synchronize the property variable with the corresponding property value, we call *updateObservers* on the property node.

.. literalinclude:: ../snippets/snippet_config_service.cpp
  :start-after: //Begin(read property variable)
  :end-before: //End(read property variable)
  :emphasize-lines: 1

.. _label_property_observers:

Property observers
------------------
Property observers allow reacting to property changes by listening to *updateObservers* calls of property nodes.

To react on property node updates, we have to provide an implementation of the *IPropertyObserver* interface
which overrides an *onUpdate* functionality taking the updated property node as function parameter.

.. note:: The property node to register the property observer at has to be created by us for registration of property observers.

.. literalinclude:: ../snippets/snippet_config_service.cpp
  :start-after: //Begin(create property observer)
  :end-before: //End(create property observer)
  :emphasize-lines: 1, 3

We have to register our property observer implementation at the property node which we want to observe.

.. literalinclude:: ../snippets/snippet_config_service.cpp
  :start-after: //Begin(register property observer)
  :end-before: //End(register property observer)
  :emphasize-lines: 2

To trigger the property observers registered at a property node, we call *updateObservers* of the corresponding node.

.. literalinclude:: ../snippets/snippet_config_service.cpp
  :start-after: //Begin(trigger property observer)
  :end-before: //End(trigger property observer)
  :emphasize-lines: 1


.. _label_using_properties_in_fep:

(Mis)Using Properties in FEP
----------------------------
.. note:: In case the Element should interact with the components, it is strongly advised not to directly change the properties of the Component, rather using the component interface itself.
          It could be the case that properties could be read from the component only on particular transitions of the :ref:`label_participant_state_machine`, for example only during *initialize_element*.
          Also, the property value change may not be immediately visible in the component.
          **Properties are only meant as an interface to the \"world\" outside the FEP System and that is strongly advised**
          **not to be used for inter or intra FEP Participant communication / information exchange and therefore only the entities that created a property should also access it.**

RPC Service Interface
---------------------

The :cpp:class:`fep3::rpc::arya::IRPCConfigurationDef` offers the following functionality:

.. list-table::
   :header-rows: 1
   :widths: 30 70

   * - Name
     - Description
   * - string *getProperties* (property_path)
     - Retrieve all property names as comma separated list.
   * - bool *exists* (property_path)
     - Check if the property path exists
   * - (value, type) *getProperty* (property_path)
     - For a given property path get its value and its type
   * - error_code *setProperty* (property_path, value, type)
     - Set a property to given value. *type* is checked. *property_path* must exist.

Have a look at :ref:`label_rpc_service_configuration_service` for the currently supported functionality.


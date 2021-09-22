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

.. _label_logging_service:

===================
FEP Logging Service
===================

.. sidebar:: Data Registry

        .. contents::

Summary
-------

+------------------------------------------------------+-----------------------------------------------------------------+
| Name                                                 |  Logging Service                                                |
+------------------------------------------------------+-----------------------------------------------------------------+
| Component Interface                                  |  :cpp:class:`fep3::arya::ILoggingService`                       |
+------------------------------------------------------+-----------------------------------------------------------------+
| RPC Service Interface                                |  :cpp:class:`fep3::rpc::arya::IRPCLoggingServiceDef`            |
|                                                      |  :cpp:class:`fep3::rpc::arya::IRPCLoggingService`               |
+------------------------------------------------------+-----------------------------------------------------------------+
| RPC Service Description                              |  :ref:`label_rpc_logging_service`                               |
+------------------------------------------------------+-----------------------------------------------------------------+
| native delivery                                      |  built-in                                                       |
+------------------------------------------------------+-----------------------------------------------------------------+
| cpp-plugin possible                                  |  no                                                             |
+------------------------------------------------------+-----------------------------------------------------------------+
| c-plugin possible                                    |  no                                                             |
+------------------------------------------------------+-----------------------------------------------------------------+

Component Interface
~~~~~~~~~~~~~~~~~~~

The :cpp:class:`fep3::arya::ILoggingService` interface offers functionality to create a logger, writing logs,
retrieving the configured severity level of a logger and to register custom logging sinks that can be logged to.

RPC Service Interfaces
~~~~~~~~~~~~~~~~~~~~~~

The :cpp:class:`fep3::rpc::arya::IRPCLoggingService` interface offers functionality to set and get a logger filter and to
get a list of all loggers, logging sinks or logging sink properties.

Overview
--------

The FEP Logging Service provides resources to write logs to various destinations and
automatically appends metadata like participant name, date, time and simulation time. Natively supported
destinations, called logging sinks in the interface, include the console, files and the system (via RPC).
Each logger can be configured individually by the system to filter logs based on the severity level
and to choose a set of sinks that should be used. This configuration design is based on the design of log4j.

How to write logs
-----------------

.. note::
    If the customization of the logging service is not needed, it is recommended to use :ref:`label_core_easy_logging` instead.

To write a log a logger object has be created first, which can be done like this:

.. literalinclude:: ../snippets/snippet_logging_service.cpp
    :start-after: //Begin(create logger)
    :end-before: //End(create logger)

During creation a name has to be given to the logger. The name does *not* have to be unique and will
be used for only two things: To identify the logger for configuration and to add it to the logs the logger writes.
Loggers can be grouped together by adding a domain to the logger name as shown in the example above. To do this write a
period `'.'` after the logger name and add a custom domain name. From left to right the name must go
from the most to the least precise level in the hierarchy.

The actual log can then be written by calling one of the log function from the logger object:

.. literalinclude:: ../snippets/snippet_logging_service.cpp
    :start-after: //Begin(log)
    :end-before: //End(log)

Should the construction of the string message be very performance intensive, it's also possible to
retrieve the severity level of the current configuration beforehand:

.. literalinclude:: ../snippets/snippet_logging_service.cpp
    :start-after: //Begin(check severity)
    :end-before: //End(check severity)

A full example of a participant with an element and a job that uses the logging service can be found in the :ref:`label_demo_logging` example.

Severity Level
~~~~~~~~~~~~~~

To indicate the importance of a log the FEP SDK uses five severity levels and one to turn logging off.
The distinction between them is not enforced but internally the FEP SDK Library uses them as described in the table below.

The Severity Levels are defined in :cpp:enum:`fep3::logging::arya::Severity`. Note: A severity level will also always
include the more severe levels. So if e.g. the severity level is set to warning, error and fatal will also be enabled.

+-------------+------------------------------------------------------------------------------+
| Level       |  Distinction                                                                 |
+-------------+------------------------------------------------------------------------------+
| off         | Intended to turn off logging                                                 |
+-------------+------------------------------------------------------------------------------+
| fatal       | Very severe error event that will presumably lead the application to abort   |
+-------------+------------------------------------------------------------------------------+
| error       | Error event that might still allow the application to continue running       |
+-------------+------------------------------------------------------------------------------+
| warning     | Designates potentially harmful situations                                    |
+-------------+------------------------------------------------------------------------------+
| info        | Informational message that highlights the progress of the application        |
+-------------+------------------------------------------------------------------------------+
| debug       | Fine-grained informational event that is most useful to debug an application |
+-------------+------------------------------------------------------------------------------+

.. _label_logging_service_configuration:

Logging Service Configuration
-----------------------------

Each logger can be configured individually or a group of loggers can be configured all at once. If no
configuration has been set for a particular logger, the configuration of its domain will be used. If the domain
doesn't have one either the next higher domain will be used and so on. If there is no configuration at all
the default configuration will be used.

The following parameters can be configured:

* Severity Level Filter
* A list of all enabled logging sinks

To set the configuration of a logger the function :cpp:func:`fep3::rpc::arya::IRPCLoggingService::setLoggerFilter()`
of the FEP SDK System Library has to be called. It has the following implications:

* If the logger is already configured the old configuration will be overwritten
* If the logger name is a group, all members of the group will be set to the new configuration

Example:

.. literalinclude:: ../snippets/snippet_logging_service.cpp
    :start-after: //Begin(set logger filter)
    :end-before: //End(set logger filter)

This will allow all loggers that are part of the "SubDomain.Domain" group (including loggers named "SubDomain.Domain") to log warnings, errors and fatals to the console and file sink.
A full example on how to set a logging filter can be found in the :ref:`label_demo_logging` example.

To change the default configuration an empty string can be passed as the logger_name to the function.
Caution: Passing an empty string will set all loggers to the new configuration and overwrite all existing configurations!

.. _label_logging_sink:

Logging Sinks
-------------

The FEP SDK Participant Library includes three native logging sinks that can be used without registering them:

+---------+------------------------------------------------------------------------------------------------------+
| Name    | Description                                                                                          |
+---------+------------------------------------------------------------------------------------------------------+
| console | Logs Severity::error and Severity::fatal to std::cerr and everything else to std::cout               |
+---------+------------------------------------------------------------------------------------------------------+
| file    | Logs to a user specified file. It will append the log to the file content if the file already exists |
|         | or creates a new file. See also `Logging Sink Configuration`_                                        |
+---------+------------------------------------------------------------------------------------------------------+
| rpc     | Sends the log to the system logger. See also `How to send logs via RPC`_                             |
+---------+------------------------------------------------------------------------------------------------------+

It is also possible to create custom logging sinks by extending :cpp:class:`fep3::arya::Properties<fep3::arya::ILoggingService::ILoggingSink>`
or one of the native logging sinks. These have to be registered at the Logging Service before they can be used:

.. literalinclude:: ../snippets/snippet_logging_service.cpp
    :start-after: //Begin(custom logging sink)
    :end-before: //End(custom logging sink)
.. literalinclude:: ../snippets/snippet_logging_service.cpp
    :start-after: //Begin(register sink)
    :end-before: //End(register sink)

Loggers can now be configured to use this custom logging sink by using the name
given to :cpp:func:`fep3::arya::ILoggingService::registerSink` ("my_sink" in this example).

.. literalinclude:: ../snippets/snippet_logging_service.cpp
    :start-after: //Begin(set custom sink logger filter)
    :end-before: //End(set custom sink logger filter)

Logging Sink Configuration
~~~~~~~~~~~~~~~~~~~~~~~~~~

Some logging sinks have properties that can be configured. From the three native sinks only the file sink has a property:
The file path to the log file to which the logs should be written to.

To set a property, first retrieve all properties from the logging sink you want to configure
and then use :cpp:func:`fep3::arya::IProperties::setProperty` from the :cpp:class:`fep3::arya::IProperties` Interface:

.. literalinclude:: ../snippets/snippet_logging_service.cpp
    :start-after: //Begin(set sink property)
    :end-before: //End(set sink property)

How to send logs via RPC
~~~~~~~~~~~~~~~~~~~~~~~~

When a participant gets added to the system (:cpp:func:`fep3::System::add`),
the participant proxy registers the system logger to the native rpc logging sink of the participant.
This enables the rpc logging on system side. For the participant to then send logs, the rpc logging sink needs to be enabled
in the `Logging Service Configuration`_ which is enabled by default for the default configuration.

Note: The system logger needs an event monitor to actual write the logs it receives. (See :cpp:func:`fep3::System::registerMonitoring`)

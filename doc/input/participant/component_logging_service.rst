.. Copyright @ 2021 VW Group. All rights reserved.
.. 
.. This Source Code Form is subject to the terms of the Mozilla 
.. Public License, v. 2.0. If a copy of the MPL was not distributed 
.. with this file, You can obtain one at https://mozilla.org/MPL/2.0/.


.. highlight:: cpp

.. _label_logging_service:

===============
Logging Service
===============

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
| native delivery                                      |  CPP-plugin                                                     |
+------------------------------------------------------+-----------------------------------------------------------------+
| cpp-plugin possible                                  |  no                                                             |
+------------------------------------------------------+-----------------------------------------------------------------+
| c-plugin possible                                    |  no                                                             |
+------------------------------------------------------+-----------------------------------------------------------------+

Component Interface
~~~~~~~~~~~~~~~~~~~

The :cpp:class:`fep3::arya::ILoggingService` interface offers functionality to create a logger, write logs,
retrieve the configured severity level of a logger and to register custom logging sinks that can be logged to.

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

+-------------+------------------------------------------------------------------------------+--------+
| Level       |  Distinction                                                                 |  Value |
+-------------+------------------------------------------------------------------------------+--------+
| off         | Intended to turn off logging                                                 |    0   |
+-------------+------------------------------------------------------------------------------+--------+
| fatal       | Very severe error event that will presumably lead the application to abort   |    1   |
+-------------+------------------------------------------------------------------------------+--------+
| error       | Error event that might still allow the application to continue running       |    2   |
+-------------+------------------------------------------------------------------------------+--------+
| warning     | Designates potentially harmful situations                                    |    3   |
+-------------+------------------------------------------------------------------------------+--------+
| info        | Informational message that highlights the progress of the application        |    4   |
+-------------+------------------------------------------------------------------------------+--------+
| debug       | Fine-grained informational event that is most useful to debug an application |    5   |
+-------------+------------------------------------------------------------------------------+--------+

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


.. _label_changing_the_severity_level_filter:

Changing the Severity Level Filter
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


Apart from setting the severity level from inside the code, you can change it by the following means:

* Changing the property *logging/default_severity* as listed in :ref:`label_component_property_table` (for example through fep control tool). This will affect the default severity of the default logger filter (and not of additional logging filters), but the severity will be changed once the component is in state initialized.

* If you are using the fep control tool, you can directly change the severity level of a particular or all loggers by sending an  :ref:`RPC call <label_rpc_logging_service>` from the command line and has exactly the same effect as `setting the severity from RPC with the C++ API <label_logging_service_configuration>`_ . For example this will change the severity of the clock sync service to *debug* and direct all logged messages only to the console:

.. code-block:: console

    callRPC  demo_system demo_cpp_receiver logging_service logging_service.arya.fep3.iid setLoggerFilter '{"logger_name": "clock_sync_service.component","severity": 5,"enable_sinks": "console"}' 


whereas this will change the severity of all loggers:

.. code-block:: console

    callRPC  demo_system demo_cpp_receiver logging_service logging_service.arya.fep3.iid setLoggerFilter '{"logger_name": "","severity": 5,"enable_sinks": "console"}' 
    
    
* The above two methods will change the severity after the component is at least in state loaded and means that you probably can change the properties or communicate with the Participant with RPC calls, which could not be the case if the participant is for example not discovered. In this case, you can temporarly set the environment variable *FEP3_DEFAULT_LOGGING_SEVERITY* to 5 for debug (or any other severity level) in order set the default severity for **ALL** loggers from participant startup. This includes also the Participant Console Logger, which for example writes in the console debug information during the component loading that takes place in the :cpp:func:`fep3::arya::createParticipant` call.

.. _label_logging_sink:

Logging Sinks
-------------

The FEP SDK Participant Library includes three native logging sinks that can be used without registering them:

+------------+------------------------------------------------------------------------------------------------------+
| Name       | Description                                                                                          |
+------------+------------------------------------------------------------------------------------------------------+
| console    | Logs Severity::error and Severity::fatal to std::cerr and everything else to std::cout               |
+------------+------------------------------------------------------------------------------------------------------+
| file       | Logs to a user specified file. It will append the log to the file content if the file already exists |
|            | or creates a new file. See also `Logging Sink Configuration`_                                        |
+------------+------------------------------------------------------------------------------------------------------+
| rpc        | Sends the log to the system logger. See also `How to send logs via RPC`_                             |
+------------+------------------------------------------------------------------------------------------------------+
| file_json  | Logs in line oriented JSON format.                                                                   |
|            | For details see also `How to write JSON log files using the file_json sink`_                         |
+------------+------------------------------------------------------------------------------------------------------+

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

To set a property, first all properties from the logging sink to configure have to be retrieved
and then :cpp:func:`fep3::arya::IProperties::setProperty` from the :cpp:class:`fep3::arya::IProperties` Interface has to be used:

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

How to write JSON log files using the file_json sink
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The fileJson sink, logs in a line oriented JSON format, and each log entry is written as an one line JSON element which looks as following:

.. code-block:: json

   [{"timestamp": "2021-09-02T16:22:48,947112+02","severity_level": "Info","logger_name": "participant","message": "Successfully loaded element","participant_name": "test_transmitter_one","log_type": "message"},
   {"timestamp": "2021-09-02T16:22:48,949063+02","severity_level": "Info","logger_name": "participant","message": "Successfully initialized element","participant_name": "test_transmitter_one","log_type": "message"}]

In order to write the log in json format, the first step is to set the path to the log file.

.. literalinclude:: ../snippets/snippet_logging_service.cpp
    :start-after: //Begin(set json sink property)
    :end-before: //End(set json sink property)

In case the file is not existing, the file is created and the log will be written in JSON format. If the log file is already existing, it should already contain a valid JSON formatted log in order
for the file to be appended. In case the file does not contain a valid JSON formatted log or is empty, then the call to :cpp:func:`fep3::arya::IProperties::setProperty` will return false and an error entry
will be written in the log file. The JSON formatted log, will be appended at the end of the existing file. For this reason it is not advised to manually create or manipulate a JSON formatted log.

The final step is to configure a logger so that it uses the fileJson logging sink.

.. literalinclude:: ../snippets/snippet_logging_service.cpp
    :start-after: //Begin(set logger json filter)
    :end-before: //End(set logger json filter)

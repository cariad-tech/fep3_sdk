.. Copyright @ 2022 VW Group. All rights reserved.
.. 
.. This Source Code Form is subject to the terms of the Mozilla 
.. Public License, v. 2.0. If a copy of the MPL was not distributed 
.. with this file, You can obtain one at https://mozilla.org/MPL/2.0/.


.. _label_health_service:

==============
Health Service
==============

.. sidebar:: Health Service

        .. contents::


Summary
=======

+------------------------------------------------------+-----------------------------------------------------------------+
| Name                                                 |  Health Service                                                 |
+------------------------------------------------------+-----------------------------------------------------------------+
| Component Interface                                  |  :cpp:class:`fep3::catelyn::IHealthService`                     |
+------------------------------------------------------+-----------------------------------------------------------------+
| RPC Service Interface                                |  :cpp:class:`fep3::rpc::catelyn::IRPCHealthServiceDef`          |
|                                                      |  :cpp:class:`fep3::rpc::catelyn::IRPCHealthService`             |
+------------------------------------------------------+-----------------------------------------------------------------+
| RPC Service Description                              |  health.json                                                    |
+------------------------------------------------------+-----------------------------------------------------------------+
| native delivery                                      |  CPP-plugin                                                     |
+------------------------------------------------------+-----------------------------------------------------------------+
| CPP-plugin possible                                  |  yes                                                            |
+------------------------------------------------------+-----------------------------------------------------------------+
| C-plugin possible                                    |  no                                                             |
+------------------------------------------------------+-----------------------------------------------------------------+

Overview
========

The :ref:`label_health_service` stores the returned results of the executed jobs and offers the possibility to retrieve
the last and the total number of returned errors of :cpp:func:`fep3::arya::IJob::executeDataIn()` , :cpp:func:`fep3::arya::IJob::execute()` and :cpp:func:`fep3::arya::IJob::executeDataOut()` of each Job running in a Participant.

.. note:: For accessing the health of the participants in a system, see :ref:`label_accessing_participants_health`.

.. _label_health_service_component_interface:

Component Interface
===================

The :cpp:class:`fep3::catelyn::IHealthService` interface offers the following functionality:

* Update the Health Service with the last job execution error codes.
* Reset the last Job execution error and error counter.

.. _label_health_service_rpc_interface:

RPC Service Interface
=====================

The :cpp:class:`fep3::rpc::catelyn::IRPCHealthServiceDef` offers the following functionality:

* Get the last and the total number of returned errors using the *getHealth* function.
* Reset the last error and error counter using the *resetHealth* function.

Native Implementations
======================

The :term:`FEP Participant Library` provides a native implementation of a :ref:`label_health_service`, which will be used per default.

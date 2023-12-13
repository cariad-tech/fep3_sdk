.. Copyright @ 2021 VW Group. All rights reserved.
.. 
.. This Source Code Form is subject to the terms of the Mozilla 
.. Public License, v. 2.0. If a copy of the MPL was not distributed 
.. with this file, You can obtain one at https://mozilla.org/MPL/2.0/.


.. _label_participant_info:

=================
 Participant Info
=================

.. sidebar:: Participant Info

        .. contents::

Summary
=======

+------------------------------------------------------+-----------------------------------------------------------------+
| Name                                                 |  Participant Info                                               |
+------------------------------------------------------+-----------------------------------------------------------------+
| Component Interface                                  |  :cpp:class:`fep3::arya::IParticipantInfo`                      |
+------------------------------------------------------+-----------------------------------------------------------------+
| RPC Service Interface                                |  None                                                           |
+------------------------------------------------------+-----------------------------------------------------------------+
| native delivery                                      |  CPP-plugin                                                     |
+------------------------------------------------------+-----------------------------------------------------------------+
| CPP-plugin possible                                  |  yes                                                            |
+------------------------------------------------------+-----------------------------------------------------------------+
| C-plugin possible                                    |  no                                                             |
+------------------------------------------------------+-----------------------------------------------------------------+


Overview
========

The :ref:`label_participant_info` is a thin access wrapper over the information provided to the :ref: service bus for getting the participant name and the system name the participant is residing in.
Do not confuse it with the RPC Service Participant Info which the Service Bus is providing.

Component Interface
*******************

Nothing special here.

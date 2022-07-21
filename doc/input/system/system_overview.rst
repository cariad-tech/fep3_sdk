.. Copyright @ 2021 VW Group. All rights reserved.
.. 
..     This Source Code Form is subject to the terms of the Mozilla
..     Public License, v. 2.0. If a copy of the MPL was not distributed
..     with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
.. 
.. If it is not possible or desirable to put the notice in a particular file, then
.. You may include the notice in a location (such as a LICENSE file in a
.. relevant directory) where a recipient would be likely to look for such a notice.
.. 
.. You may add additional accurate notices of copyright ownership.


.. _FEP_System_Library:

==================
FEP System Library
==================

The :term:`FEP System Library` provides a standalone solution to control,
monitor and configure a :term:`FEP System` over the :ref:`label_service_bus`.
This library does not depend on the :ref:`label_participant_library`.

A :term:`FEP System` is a collection of participants that provide :term:`RPC Service` interfaces to control them.
Each service can be reached via the class :cpp:class:`fep3::ParticipantProxy` created within a :cpp:class:`fep3::System`.

Currently a participant is identified by its name and its system relation.
As long as participants take part in the same :ref:`label_service_bus` it can
be discovered via the discover mechanism provided by this service bus implementation.
Otherwise you need to know the name, system and address of the participant to connect it.

Creating a system to control it
===============================

The system library has no possibility to execute or start-up any other processes.
The provided functionality can *discover systems*, *connect already started participants* and *control participants and systems*.
If a participant shutdowned and stopped its service bus connection it could not be restarted!

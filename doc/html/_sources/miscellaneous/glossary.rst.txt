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


.. _Glossary: 

========
Glossary
========


.. glossary::

    FEP Participant
        A single unit in the :term:`FEP System`, links against the FEP implementation, can be user functionality or represent any other application which shall be part of a :term:`FEP System`

    FEP System
        A group of :term:`FEP Participant` s

    FEP Element
        Specific implementation of a :term:`FEP Participant` that gets loaded during the :ref:`state transition<label_participant_state_machine>` load.

    FEP Signal
        A signal within a FEP, that other :term:`FEP Participant` s can subscribe to, to receive samples of it

    FEP
        Functional Engineering Platform

    FEP Participant Developer
        A FEP Participant Developer uses the FEP Participant API of the FEP SDK to develop a :term:`FEP Participant`.

    FEP System Developer
        A FEP System Developer designs :term:`FEP System` s.

    FEP System Operator
        A FEP System Operator uses a :term:`FEP System` to execute a simulation.

    FEP Component Developer
        A FEP Component Developer uses the FEP Component API of the FEP SDK to develop FEP Components, e. g. to support (other) middleware.

    FEP Core Developer
        A FEP Core Developer develops the FEP SDK Core code.

    FEP Component
        Service functionality of a :term:`FEP Participant` is provided as component.

    FEP Component Implementation
        A concrete implementation of a :term:`FEP Component` provides concrete behavior for a specific :term:`FEP Component`.

    FEP Component Interface
        The interface of a :term:`FEP Component`.

    FEP Component System
        Facilities to control which :term:`FEP Components<FEP Component>` are used by a :term:`FEP Participant`

    FEP Component Configuration File
        File containing configuration information for the :term:`FEP Component System` defining
        which :term:`FEP Component` s shall be loaded by the :term:`FEP Participant`.

    FEP Service Bus
        The FEP Service Bus is the most important :term:`FEP Component` of a :term:`FEP Participant` to provide
        control access to the participant via remote procedure calls.

    FEP Participant Library
        Main library for developing :term:`FEP Participant` s.

    FEP System Library
        Main library for controlling :term:`FEP System` s.

    FEP Base Utilities
        Collection of helpful tools for developing and controlling :term:`FEP System` s.

    FEP VU
        FEP "Virtuelles Umfeld", standardized representation of a virtual environment using FEP SDK.

    FEP Utility
        For the comfortable deploying, launching and controlling of the FEP Simulation. 

    FEP Interfaces
        Collection of interfaces for different simulation platforms, like ADTF, MATLAB/Simulink, XIL, FMI, SCALEXIO and etc.

    Package RPC
        The Package RPC is a library to provide a request / response mechanism via JSON-RPC and comes with
        a simple HTTP server / HTTP client implementation.

    RPC Service
        Public interface via :term:`FEP Service Bus`.

    RPC Client
        Client object that can use a dedicated :term:`RPC Service` via :term:`FEP Service Bus`.

    RPC Requester
        Term of a :term:`RPC Client` created via :term:`FEP Service Bus`.

    CPP Plugin
        Dynamic library or shared object which is used to exchange :term:`FEP Component` functionality with custom and foreign implementations.

    CoRTEX
        Continuous toolchain for developing, testing, building and deploying products using Conan.

    DDL
        Data Definition Language



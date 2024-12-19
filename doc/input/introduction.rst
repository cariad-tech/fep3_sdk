.. Copyright 2023 CARIAD SE.
.. 
.. This Source Code Form is subject to the terms of the Mozilla 
.. Public License, v. 2.0. If a copy of the MPL was not distributed 
.. with this file, You can obtain one at https://mozilla.org/MPL/2.0/.


============
Introduction
============

What is FEP
===========

FEP stands for **Functional Engineering Platform**.
It is a product family of software libraries and tools that enables the composition of a distributed simulation system in an automotive context. It accompanies the development from the function implementation to real hardware integration. 

FEP is realized as a highly integrated product, which is based on a communication framework, provided by **FEP SDK**.
The other products in family are :term:`FEP VU`, :term:`FEP Interfaces` and :term:`FEP Utility`. They enhance the framework furthermore, to be able to design and control a simulation over a wide range of simulation platforms including ADTF, MATLAB/Simulink, XIL, FMI, SCALEXIO, VEOS and more.

This makes it possible to use FEP as a platform for numerous simulation use cases.

What is FEP SDK
===============
FEP SDK is the Software Development Kit of the FEP, written in C++. It provides the base libraries to combine different simulation environments, control the simulation and exchange simulation data based on a distributed networking system.

With FEP SDK the developer can build a sophisticated software for simulation and testing of automotive projects.

The **FEP SDK 3** is the latest major version of FEP SDK.
  
It mainly consists of following parts:

- :ref:`FEP Participant Library <FEP_Participant_Library>`
- :ref:`FEP System Library <FEP_System_Library>`

How to start
============

* :ref:`Installation <Installation>` will help you install FEP SDK and its development environment.
* :ref:`Tutorial <Tutorial>` will get you started to build a complete FEP use case by using FEP SDK libraries.
* Then you can :ref:`Dive into FEP SDK <Overview_SDK>`. It explains all the important concepts of FEP SDK in details, which will help you understand the implementation and usage of all concepts and their APIs.
* :ref:`Advanced Concepts <Advanced_Concepts>` elaborates the advanced theory of different topics within the FEP SDK. It's your chances to develop a deeper understanding of FEP SDK behind the scenes. It is highly recommended to read them if you want to develop a complicated and robust software using FEP or contribute to FEP.
* :ref:`Examples <Examples>` demonstrate different use cases and scenarios and offer you implementations you can base your development on.
* :ref:`HOWTO <HOWTO>` and :ref:`FAQ <FAQ>` will answer your questions and provide you a lot of practical tips about FEP SDK.
* :ref:`API References <API_References>` is the full API documentation of FEP SDK.
* Have a quick look-up in :ref:`Glossary` if you have doubts about some definitions.

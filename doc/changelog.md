<!--
  Copyright @ 2021 VW Group. All rights reserved.
  
      This Source Code Form is subject to the terms of the Mozilla
      Public License, v. 2.0. If a copy of the MPL was not distributed
      with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
  
  If it is not possible or desirable to put the notice in a particular file, then
  You may include the notice in a location (such as a LICENSE file in a
  relevant directory) where a recipient would be likely to look for such a notice.
  
  You may add additional accurate notices of copyright ownership.
  
  -->

# FEP SDK Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0) and this project adheres to [Semantic Versioning](https://semver.org/lang/en).

## [Unreleased]

## [3.1.0]

### Changes

- FEPSDK-3309 Based on FEP SDK Participant 3.1.0
- FEPSDK-3309 Based on FEP SDK System 3.1.0
- FEPSDK-3278 switch to c++ 17 standard
- FEPSDK-3081 Replaced deprecated DataReader operator>> in examples and snippets

## [3.0.1]

### Changes

- Based on FEP SDK Participant 3.0.1
- Based on FEP SDK System 3.0.1
- FEPSDK-3218 adapt yaml for the participant component plugin
- FEPSDK-3218 Use macro to link pthread and update doc
- FEPSDK-3137 Improve fep sdk timing documentation
- FEPSDK-3103 Add Windows_x64_vc142_VS2019 build profile
- FEPSDK-3101 Add Linux_armv8_gcc7 build profile
- FEPSDK-3089 Troubleshooting, start clock master last
- FEPSDK-3009 Communation section missing sections are added
- FEPSDK-3007 Configurable element documentation
- FEPSDK-2994 Added the basic concepts documentation
- FEPSDK-2963 Readme updated for building with cmake only
- FEPSDK-2922 Writing a FEP Element documentation
- FEPSDK-2922 Add recomendation about using argc
- FEPSDK-2910 Added signal renaming section to data registry prose doc.
- FEPSDK-2890 Add minimal example
- FEPSDK-2879 document what to do in case of sample loss
- FEPSDK-2708 Added discrete clock config defines to clock service prose doc.
- FEPSDK-2890 Documentation initial redesign

### Bugfixes
- FEPSDK-3203 Correct mpl-header in fep sdk documentation
- FEPSDK-3202 Fix missing documentation snippets
- FEPSDK-3192 fix code example references in participant overview documentation
- FEPSDK-3190 Fixed broken macro links
- FEPSDK-3068 Fixed slave clock macros in clock sync service prose doc
- FEPSDK-3068 FEPSDK-3190 Fixed typos in prose doc
- FEPSDK-3028 Core examples now add the Jobs to the Job Registry in the correct callback, i. e. IElement::load()
- FEPSDK-2988 Adapted prose doc to use correct native scheduler name
- FEPSDK-2917 Fixing documentation of DataRegistry

## [3.0.0]

### Changes
- FEPSDK-2857 Set pause mode as unsupported
- FEPSDK-2768 Rename property cycle_time_ms of clock service 
- FEPSDK-2765 Remove TimeViolationStrategy::set_stm_to_error
- FEPSDK-2727 Perform thorough performance testing
- FEPSDK-2719 Add C wrapper for FEP Component "Participant Info"
- FEPSDK-2710 Move CommandLineParser class to the core library
- FEPSDK-2662 Redesign class DataReaderBacklog : public IDataRegistry::IDataReceiver
- FEPSDK-2652 Deliver migration documentation as part of FEP SDK documentation
- FEPSDK-2585 Prepare FEP3 SDK code to be distributed as OSS
- FEPSDK-2561 [FEP3] Use the new profiles [gcc5, v141] as base for the delivery packages
- FEPSDK-2525 Create createParticipant() variant that parses the argc / argv arguments
- FEPSDK-2491 Correct inconsistency in file names covering "stream types"
- FEPSDK-2489 System name should change the dds domain id
- FEPSDK-2487 Add C wrapper for component Job Registry
- FEPSDK-2406 Support for QoS for user defined stream types
- FEPSDK-2353 Fully documentation for the Logging Service
- FEPSDK-2315 Create documentation for configuration service
- FEPSDK-2262 Add detailed user documentation on FEP Component System
- FEPSDK-2179 Create Core API
- FEPSDK-2146 Documentation of Data Registry and every single usecase of  data accces

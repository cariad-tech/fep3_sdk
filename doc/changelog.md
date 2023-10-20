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

## [3.2.0]

### Changes
- FEPSDK-3651 Copy service_bus symbol files while calling deploy
- FEPSDK-3637 FEP System state transition cancellation
- FEPSDK-3636 Refactor setSystemState() participant state transition error handling
- FEPSDK-3618 Update MPL license header
- FEPSDK-3614 FEP System Logger  configurable with severity and has file sink
- FEPSDK-3609 State Machine does not rollback correctly on component registry or element transition error
- FEPSDK-3608 FEP System provides error code and description of the Participant that failed a state Transition
- FEPSDK-3607 Participant State Machine RPC Server returns error descriptions on failed transitions
- FEPSDK-3606 Fep System library should throw exceptions or log errors on failed rpc client communication
- FEPSDK-3605 Improve error logging on state transitions
- FEPSDK-3604 Add logging sink file_csv
- FEPSDK-3602 Participant init and start priorities are persistent in system library instances
- FEPSDK-3601 Data Triggered Jobs should run on scheduler thread pool
- FEPSDK-3596 Example of signal with ddl structure containing dynamic array
- FEPSDK-3585 Refactor Local Clock Based Scheduler
- FEPSDK-3581 Registering DataReader for a signal that is input to mapping should be possible
- FEPSDK-3577 DDS Discovery messages should send the IP instead of host name in case default Server Url ist not used 
- FEPSDK-3568 Support DevHub2 for build pipelines
- FEPSDK-3557 Provide Convenient Element and Job class
- FEPSDK-3555 Clean up fep sdk participant includes
- FEPSDK-3551 Data Triggered job should reconfigurable via Properties and vice versa
- FEPSDK-3550 Const Job configuration cannot be added to job registry
- FEPSDK-3534 Deliver conan package with Python - C++ 3.8 3.9 3.10 bindings
- FEPSDK-3532 Use dev_essential 1.3.0 integration
- FEPSDK-3523 Manual on ADTF scope usage as debug tool
- FEPSDK-3522 Improve Service Bus logging for debugging
- FEPSDK-3521 ADTF Player Participant time update frequency shall be configurable
- FEPSDK-3520 Smoke Test 32 - 64 Bit Communication
- FEPSDK-3514 FEP3 32 bit
- FEPSDK-3512 Measure and Improve Throughput performance of Data Job
- FEPSDK-3508 Automatically generate release notes
- FEPSDK-3504 Use persistent integration builds in FEP Products
- FEPSDK-3482 Change RTI DDS signal connection default behavior to "wait for writer"
- FEPSDK-3477 Complete Python binding in system library
- FEPSDK-3472 Remove transmission of StreamType for static DataWriter
- FEPSDK-3465 User defined QoS Profiles can be used in participants
- FEPSDK-3464 Stream Type DDL should correctly select Qos Setting according to size
- FEPSDK-3462 Same component loading mechanism in system and participant
- FEPSDK-3454 Investigate timing with slower participants
- FEPSDK-3445 Update FEPSDK to boost 1.79
- FEPSDK-3442 Data triggered job works with multiple signals
- FEPSDK-3425 Provide functionality to disable RTI DDS Domain Tag within RTI DDS SimulationBus plugin
- FEPSDK-3421 Provide Python binding in system library
- FEPSDK-3416 Integrate  Developer Guide into FEP SDK Documentation
- FEPSDK-3407 Deprecate C Plugin Mechanism in Documentation
- FEPSDK-3369 Automatically do safe type-casts i.e. for int32 on int64 properties
- FEPSDK-3359 Improve docs on logging from user perspective
- FEPSDK-3358 Update C Component Interface with new catelyn::ISchedulerService, IScheduler, ISchedulerRegistry
- FEPSDK-3354 Migrate DOC (OLD) FEP Participant Library to current chapters
- FEPSDK-3353 Documentation about Discovery problems and solutions when using Peer List
- FEPSDK-3350 [Feature] Switch to Clipp command line parser
- FEPSDK-3349 Provide setParticipantState and shutdown participant function in fep_system
- FEPSDK-3347 Create doc and minimal example on how to use mocks of FEP Components
- FEPSDK-3342 Add possibility to retrieve loaded components IIDs via RPC
- FEPSDK-3332 Add environment variables for overwriting system and server urls
- FEPSDK-3328 Provide existing gmocks in FEP repositories as part of the FEP Component API
- FEPSDK-3320 Health Service for FEP Participant and FEP System 
- FEPSDK-3315 Deliver DDS Service Discovery as default plugin
- FEPSDK-3292 flipping the order of registering DataReaders and DataWriters within the DataRegistry::tense
- FEPSDK-3291 Adapt RTI DDS Simulation Bus to use AsyncWaitset instead of Waitset for data reception
- FEPSDK-3276 Command line arguments for executables are not aligned with FEP Agent
- FEPSDK-3275 Remove CMake deployment of fep_component_plugin library
- FEPSDK-3274 Rename fep_components_plugin.dll
- FEPSDK-3267 Provide fep_tooling_base, fep_participant, fep_system with conan profile Windows_x64_to_SCALEXIO_Linux_x86_gcc5 for the SCALEXIO
- FEPSDK-3266 Provide convenience API for participant manipulation in FEP System library
- FEPSDK-3265 [PoC] Realization of Multiple sender transferring to single signal with RTI DDS
- FEPSDK-3259 DDS ServiceBus discovery environment
- FEPSDK-3257 Allow System State Transitions on a heterogeneous system
- FEPSDK-3256 Define limits for time intervals of a FEP Participant using discrete timing 
- FEPSDK-3253 Specify requirements for fep3::arya::ISimulationBus::IDataReceiver::operator() towards strong references to samples/types
- FEPSDK-3252 Implement Healthiness struct
- FEPSDK-3239 Provide plugin versions via RPC
- FEPSDK-3233 Add uint32 and uint64 standard property datatypes
- FEPSDK-3220 Remove all unneeded error codes from fep3_errors.h
- FEPSDK-3218 Remove default loading built-in Component Implementation mechanism
- FEPSDK-3193 DOC Chapter about Configuring data exchange 
- FEPSDK-3186 Remove vs2017_v141 build profiles
- FEPSDK-3185 Remove gcc5 build profiles
- FEPSDK-3179 Create Health Service ADR
- FEPSDK-3175 [Feature] Convenient data triggered usage
- FEPSDK-3174 Implement signal waiting for DDS SimBus
- FEPSDK-3173 Support registration of a single instance as multiple FEP Component interfaces from within FEP Component Plugins
- FEPSDK-3171 [DOC] How to create a single FEP Component Plugin which provides multiple FEP Component interfaces
- FEPSDK-3149 Implementation of an alternative discovery mechanism based on unicasting connection
- FEPSDK-3130 Create fep3::system::discoverSystem function which accepts participants and timeout
- FEPSDK-3107 Add popSample's'Before(timestamp) to DataReaderBacklog
- FEPSDK-3102 Evaluate SimBus FEP Component Swap
- FEPSDK-3100 Implementation line based logging sink
- FEPSDK-3083 Copy C-API POC to fep_sdk_system library
- FEPSDK-3081 Mark operator>> of DataReader deprecated
- FEPSDK-3062 Make FEP 3 participant discovery timeout configurable
- FEPSDK-3058 Document all default commandline argument values in built-in help
- FEPSDK-3057 Call ::fep3::cpp::DataJob::initConfiguration from within fep3::cpp::addToComponents
- FEPSDK-3043 Provide command history
- FEPSDK-3033 Improve log message time formatting
- FEPSDK-3031 Change templatization level of fep3::cpp::arya::DataJobElement
- FEPSDK-3030 IRPCRequester::sendRequest interface should be noexcept
- FEPSDK-3018 Move FEP SDK Participant API into non versioning namespaces
- FEPSDK-3009 DOC Chapter about sending and receiving Data
- FEPSDK-3001 Provide ability to set additional DDL files for mapping
- FEPSDK-2993 Add the mapping presentation as an example to the fep_sdk repo
- FEPSDK-2937 Add documentation for signal mapping
- FEPSDK-2934 StreamType big for DDL
- FEPSDK-2922 Document ways on how to create elements
- FEPSDK-2920 Improve robustness of IElement interface towards component usage
- FEPSDK-2856 start- and init-priority should support parallelism
- FEPSDK-2849 [PoC] Add an event distribution mechanism for changes of participant health states
- FEPSDK-2672 Record if logging with logging service fails
- FEPSDK-2580 Improve LoggingService filter configuration
- FEPSDK-2516 Example and Documentation on "How to write own component plugins?"
- FEPSDK-2501 Unexpected behaviour regarding fep control tool help
- FEPSDK-2367 DataRegistry converts StreamType *fileref* for DDL StreamMetaTypes
- FEPSDK-2264 Extend HTTP System Access and its Service Discovery with better unique Service Names
- FEPSDK-2039 Extend the current clock synchronisation with maximal time step check

### Bugfixes
- FEPSDK-3675 cpython version range in conan recipe leads to "missing" packages
- FEPSDK-3655 slave_master_on_demand clock does not always call IEventSink reset events
- FEPSDK-3630 Health Service RPC Service does not respond with valid json object when job registry is empty
- FEPSDK-3622 Fep sdk example rpc client/service: Service error could not register service
- FEPSDK-3619 Default Job API did not remove job from job registry while unloading 
- FEPSDK-3612 Python event_monitor imports wrong fep3_system library
- FEPSDK-3603 Fix error "mutex destroyed while still in use" in DDS ServiceBus 
- FEPSDK-3595 OpenSource: CMake build of FEP SDK examples fails 
- FEPSDK-3593 Fix copy semantics of fep3::arya::PropertyValueWithObserver class
- FEPSDK-3591 Timing master waits for jobs according to their expected completion time
- FEPSDK-3589 job_registry is not a FEP Super Component
- FEPSDK-3576 Discrete Timing time factor scales waiting incorrectly
- FEPSDK-3562 Disable copy of fep3::base::arya::Configuration 
- FEPSDK-3545 FEP SDK Signal Mapping Example is not running
- FEPSDK-3543 Fix setSystemState(Fep3State.UNREACHABLE) in FEP System Library
- FEPSDK-3533 FEP System Library hangs when logs occurs
- FEPSDK-3530 DDS Host name resolving fails on Jenkins
- FEPSDK-3511 No check for valid RPC Clients
- FEPSDK-3495 FEP System Library Python Binding wont work for callRPC and getSystemState
- FEPSDK-3494 Unsuccessful initialization when using feature "waitforsignal"
- FEPSDK-3493 Different timing behaviour in FEP SDK 3.2 during state running
- FEPSDK-3484 Job of the timing master is triggered at wrong time directly after entering the "running" state
- FEPSDK-3483 error message not meaningful while adding/merging a ddl description
- FEPSDK-3478 Mapped signals are not unregistered from mapping engine
- FEPSDK-3476 conan source not working for fep_sdk_participant package
- FEPSDK-3469 Plugin binary names with dot cannot be loaded
- FEPSDK-3468 Libraries depend on generated JSON stubs which is not reflected by CMake targets
- FEPSDK-3463 Problems with switching components from a 3.1 built participant to 3.2 using arya
- FEPSDK-3451 Registering big sized structs as FEP signal failing
- FEPSDK-3433 Usage of commas in description files breaks DDL StreamType
- FEPSDK-3427 Deadlock during RPC Server destruction
- FEPSDK-3424 [FEP 2] FEP System will throw if initialize() returned error code. 
- FEPSDK-3402 Call to setParticipantState with argument shutdown or unreachable crashes in fep system library
- FEPSDK-3362 Logging is deregistered when fep3::System is copied or moved
- FEPSDK-3360 Configuration Service is not accessible from within FEP Component C plugins
- FEPSDK-3352 getPropertyValue optional default_value does not work correctly
- FEPSDK-3346 Error message and usage string for system name are inconsistent
- FEPSDK-3340 Comma separator for getStreamType RPC call creates problems when a value has a comma inside
- FEPSDK-3336 Timing master triggers client and own jobs in parallel
- FEPSDK-3317 Logging sink properties are setable but cannot be queried
- FEPSDK-3294 DataWriter::flushNow method should check if pointer exist
- FEPSDK-3281 Scheduling starts before all FEP Participants are ready to run
- FEPSDK-3277 Race condition in response of getMasterTime (continuous system time)
- FEPSDK-3263 Component loading does not work (in some cases ) if using pthread
- FEPSDK-3261 Enable the discovery on specific network interfaces
- FEPSDK-3243 Logging is delayed on VW WindowsClients
- FEPSDK-3232 JSON file log only valid in shutdown
- FEPSDK-3228 Wrong relative path resolution while loading Component Plugins
- FEPSDK-3217 purgeAndPopSampleBefore does not work as intended (always returns nullptr)
- FEPSDK-3212 ContinuousClock::getTime() leads to infinite recursive loop
- FEPSDK-3203 MPL Header in FEP SDK Documentaion in all .rst files are not correct
- FEPSDK-3202 Documentation snippets missing
- FEPSDK-3192 Documentation Element description example code references are wrong
- FEPSDK-3162 Config cmake files have typo
- FEPSDK-3131 Operating a FEP Participant on CARIAD Windows client may fail
- FEPSDK-3117 Deregistration of signals fails in combination with mapping functionality
- FEPSDK-3097 Race condition in participant http TCP port allocation
- FEPSDK-3092 ServiceBus' logger initialization missing
- FEPSDK-3091 Data Sample documentation improvements
- FEPSDK-3086 Shared libraries are located in bin/ instead of lib/
- FEPSDK-3069 Incorrect wording in FEP Control API for shutdown
- FEPSDK-3068 How to set timing client with discrete clock in FEP3 directly via FEP properties?
- FEPSDK-3066 Improve the robustness of base classes by complying with the "Rule of Five"
- FEPSDK-3065 Error message unclear when triggering pause
- FEPSDK-3064 IElement nonmutable calls are not const-correct
- FEPSDK-3027 Useless error message "Error: (unknown)" when trying to set a non existing property via fep_control
- FEPSDK-2991 Implementation of IServiceBus::getRequester() diverts from API documentation
- FEPSDK-2983 Deinitializing FEP Participant will lead to DDS exception
- FEPSDK-2957 Pressing key UP may execute command
- FEPSDK-2918 Equivalent behaviour of addToDataRegistry and removefromDataRegistry
- FEPSDK-2879 Loss of first samples after start of simulation
- FEPSDK-2786 FEP 3 Timing  provides negative timestamp after re-initialization
- FEPSDK-2752 Multiple "discoverSystem" calls are needed when participants reside on different machines
- FEPSDK-2642 Result::getErrorLabel() mostly returns "(unknown)"
- FEPSDK-2557 DataWriter and DataReader API correction


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

<!--
  Copyright 2023 CARIAD SE.
  
This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
-->

# FEP SDK Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0) and this project adheres to [Semantic Versioning](https://semver.org/lang/en).

<h3><a href="#fep_sdk">FEP SDK</a></h3>

<!-- DOC GENERATOR HEADER -->
* [3.2.1](#FEP_SDK_3_2_1) \| [Changes](#FEP_SDK_3_2_1_changes) \| [Fixes](#FEP_SDK_3_2_1_fixes) \| [Known issues](#FEP_SDK_3_2_1_known_issues) \| Release date: 2023-11-06
* [3.2.0](#FEP_SDK_3_2_0) \| [Changes](#FEP_SDK_3_2_0_changes) \| [Fixes](#FEP_SDK_3_2_0_fixes) \| [Known issues](#FEP_SDK_3_2_0_known_issues) \| Release date: 2023-07-23
* [3.1.0](#FEP_SDK_3_1_0) \| [Changes](#FEP_SDK_3_1_0_changes) \| [Fixes](#FEP_SDK_3_1_0_fixes) \| [Known issues](#FEP_SDK_3_1_0_known_issues) \| Release date: 2022/04/13
* [3.0.1-beta](#FEP_SDK_3_0_1) \| [Changes](#FEP_SDK_3_0_1_changes) \| [Fixes](#FEP_SDK_3_0_1_fixes) \| [Known issues](#FEP_SDK_3_0_1_known_issues) \| Release date: 2022/02/02
* [3.0.0](#FEP_SDK_3_0_0) \| [Changes](#FEP_SDK_3_0_0_changes) \| [Fixes](#FEP_SDK_3_0_0_fixes) \| [Known issues](#FEP_SDK_3_0_0_known_issues) \| Release date: 2021/03/25

<!-- DOC GENERATOR BODY -->

<a name="FEP_SDK_3_2_1"></a>

<a name="FEP_SDK_3_2_1_changes"></a>
#### Changes

_**Done**_

- [FEPSDK-3627][] - <a name="FEPSDK-3627_internal_link"></a> Sim Bus RTI DDS Subscribers/Listeners visible in RTI Admin Console [\[view\]][ef1e5b216cc179ce3a6fcd0abefe4b8d557d2fbe] 
    * Added section 'RTI Admin Console' to prose documentation
     * Fixed typos in Advanced Concepts/Communication
    
    
- [FEPSDK-3656][] - <a name="FEPSDK-3656_internal_link"></a> easy_core_example uses core::DefaultJob API [\[view\]][a57792aedd4bf775746c2710124d979e297750b7] [\[view\]][c046058a3b26356eaa5ab969dc1b619eecee5312] 
    * easy_core_example uses core::DefaultJob API
- [FEPSDK-3731][] - <a name="FEPSDK-3731_internal_link"></a> Update doc regarding signal renaming and property must_be_ready_signals [\[view\]][be13f19ae2604ce0c091546cf56769e0022920dc] 
    * Tested with signal renaming on sender's and receiver's side. Documentation updated
    
    
<a name="FEP_SDK_3_2_1_fixes"></a>
#### Fixes

_**Done**_

- [FEPSDK-3681][] - <a name="FEPSDK-3681_internal_link"></a> Fix potential crash in FEP SDK examples [\[view\]][5c8415df248456b714b1750b986169d2428bb45e] [\[view\]][3ea3d928017d4ab267b74fe5b4b6a53dfd3e8f2d] 
    * Fixed usage of fep3::Optional in FEP SDK examples
    * Fixed documentation typos

<a name="FEP_SDK_3_2_1_known_issues"></a>

<a name="FEP_SDK_3_2_1"></a>
<!--- Issue links -->
[FEPSDK-3681]
[FEPSDK-3627]
[FEPSDK-3731]
[FEPSDK-3656]

<a name="FEP_SDK_3_2_0"></a>

<a name="FEP_SDK_3_2_0_changes"></a>
#### Changes

_**Done**_

- [FEPSDK-2516][] - <a name="FEPSDK-2516_internal_link"></a> Example and Documentation on "How to write own component plugins?" 
- [FEPSDK-2922][] - <a name="FEPSDK-2922_internal_link"></a> Document ways on how to create elements 
    * Added documentation of creating a participant
    * Listing of  createParticipant convenience wrappers 
    * Moved the old state maschine part to this part
    * Custom factory example
    
    
- [FEPSDK-2937][] - <a name="FEPSDK-2937_internal_link"></a> Add documentation for signal mapping 
- [FEPSDK-2993][] - <a name="FEPSDK-2993_internal_link"></a> Add the mapping presentation as an example to the fep_sdk repo 
- [FEPSDK-3009][] - <a name="FEPSDK-3009_internal_link"></a> DOC Chapter about sending and receiving Data 
- [FEPSDK-3193][] - <a name="FEPSDK-3193_internal_link"></a> DOC Chapter about Configuring data exchange  
    * How to configure buffer written
     * How to use stream types written
     * How to change stream types written
     * QoS stream type mapping written
     * QoS configuration written
     * Configuration Service for RTI DDS Simulation Bus written
    
    
- [FEPSDK-3353][] - <a name="FEPSDK-3353_internal_link"></a> Documentation about Discovery problems and solutions when using Peer List 
- [FEPSDK-3354][] - <a name="FEPSDK-3354_internal_link"></a> Migrate DOC (OLD) FEP Participant Library to current chapters 
    * The OLD Participant Library is moved corresponding existing chapters. All old chapters checked.
     * The participant_and_element.rst ist also moved to existing chapters.
     * The Default Components can be found in Appendix and also in chapter &quot;Component&quot;
     * Restructure with minimum change of content.
    
    
- [FEPSDK-3359][] - <a name="FEPSDK-3359_internal_link"></a> Improve docs on logging from user perspective 
- [FEPSDK-3407][] - <a name="FEPSDK-3407_internal_link"></a> Deprecate C Plugin Mechanism in Documentation [\[view\]][b5b89b638f8d7f2cfbffd4b44c942d917be98f8c] [\[view\]][cee4b92565c3e871e49dd0383a841cfd38fd6ce5] [\[view\]][2f104616df44c570c0f0f452137e1c222b6ef109] 
- [FEPSDK-3523][] - <a name="FEPSDK-3523_internal_link"></a> Manual on ADTF scope usage as debug tool 
    * Added fep_debugging section to fep overall doc
        * FEP3 Element ADTF Scope for data visualization
        * Configuration of Participant logging behaviour
        * Configuration of Participant time behaviour
        * FEP3 Element ADTF Recorder to record data
    
    
- [FEPSDK-3618][] - <a name="FEPSDK-3618_internal_link"></a> Update MPL license header [\[view\]][ea4307c838b5708bcafff32fae25413627c2c25d] 
<a name="FEP_SDK_3_2_0_fixes"></a>
#### Fixes

_**Done**_

- [FEPSDK-2991][] - <a name="FEPSDK-2991_internal_link"></a> Implementation of IServiceBus::getRequester() diverts from API documentation [\[view\]][56b2d78c5a993f9397d0d4379e503ac16463e13f] 
    * getRequester() now returns with nullptr instead of exception
     * usage and tests of getRequester() are adjusted to the new behavour
     * documentation corrected
    
    
- [FEPSDK-3068][] - <a name="FEPSDK-3068_internal_link"></a> How to set timing client with discrete clock in FEP3 directly via FEP properties? [\[view\]][61fd21d6cf3a3d820c22529d39b46631dfccf5cc] 
    * FEP 3 Participant
        * Fixed clock sync service doc
    * FEP 3 SDK
        * Fixed typos in prose doc
        * Fixed slave clock macros in clock sync service prose doc
    
    
- [FEPSDK-3091][] - <a name="FEPSDK-3091_internal_link"></a> Data Sample documentation improvements [\[view\]][67d8e66e2dad2fe437eac171837398f2500b4645] [\[view\]][c31131ba8d7e4ab5601749e0b9a0e07c601812da] 
    * Corrected doxygen doc of
    {code:java}
    template <typename T> class DataSampleType<T, typename std::enable_if<std::is_standard_layout<T>::value>::type> : public arya::DataSampleBase, public arya::RawMemoryStandardType<T>{code}
    
     * Added doxygen doc of behavior towards timestamp to ::fep3::core::arya::DataWriter::write(const IDataSample& data_sample)
    
    
- [FEPSDK-3192][] - <a name="FEPSDK-3192_internal_link"></a> Documentation Element description example code references are wrong 
    * typos corrected
     * extra spaces and tabs removed
     * code snippet references with :lines: replaced by :start-at:, :start-after:, :end-at: and :end-before: options, hence fixing the wrong locations
         * already existing code patterns were applied wherever it was possible, otherwise a new pattern as extra comment was inserted to the code snippet
    
    
- [FEPSDK-3202][] - <a name="FEPSDK-3202_internal_link"></a> Documentation snippets missing 
    * Code snippets fixed.
    
    
- [FEPSDK-3203][] - <a name="FEPSDK-3203_internal_link"></a> MPL Header in FEP SDK Documentaion in all .rst files are not correct 
    * replaced '# Copyright' by '.. Copyright' as mentioned in section 'Current behaviour' (see above)
    
    
- [FEPSDK-3545][] - <a name="FEPSDK-3545_internal_link"></a> FEP SDK Signal Mapping Example is not running 
    * *.description files copied installed in bin\demo_data directory
     * Description files are loaded by sender/receiver from the demo_data directory. The path to this demo_data directory is determined by the absolute path of the directory where the sender/receiver executable is located plus the sub-directory demo_data.
    * Fixed some deprecated warnings of other examples e.g. a_util::result::isFailed() / isOK()
    * NOTE: Examples cannot be built with cmake - dependency clipp not found (see FEPSDK-3595)
    
    
- [FEPSDK-3622][] - <a name="FEPSDK-3622_internal_link"></a> Fep sdk example rpc client/service: Service error could not register service 

<a name="FEP_SDK_3_2_0_known_issues"></a>

<a name="FEP_SDK_3_2_0"></a>
<!--- Issue links -->
[FEPSDK-2937]
[FEPSDK-2993]
[FEPSDK-3347]
[FEPSDK-3193]
[FEPSDK-3009]
[FEPSDK-3091]
[FEPSDK-3407]
[FEPSDK-2922]
[FEPSDK-3192]
[FEPSDK-3353]
[FEPSDK-3202]
[FEPSDK-2516]
[FEPSDK-3545]
[FEPSDK-3622]
[FEPSDK-3068]
[FEPSDK-2991]
[FEPSDK-3359]
[FEPSDK-3416]
[FEPSDK-3203]
[FEPSDK-3523]
[FEPSDK-3354]
[FEPSDK-3618]
[FEPSDK-3171]


<a name="FEP_SDK_3_1_0"></a>
## [3.1.0]
<a name="FEP_SDK_3_1_0_changes"></a>
### Changes

- FEPSDK-3309 Based on FEP SDK Participant 3.1.0
- FEPSDK-3309 Based on FEP SDK System 3.1.0
- FEPSDK-3278 switch to c++ 17 standard
- FEPSDK-3081 Replaced deprecated DataReader operator>> in examples and snippets

<a name="FEP_SDK_3_0_1"></a>
## [3.0.1-beta]

<a name="FEP_SDK_3_0_1_changes"></a>

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

<a name="FEP_SDK_3_1_0_fixes"></a>

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

<a name="FEP_SDK_3_0_0"></a>
## [3.0.0]

<a name="FEP_SDK_3_0_0_changes"></a>

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

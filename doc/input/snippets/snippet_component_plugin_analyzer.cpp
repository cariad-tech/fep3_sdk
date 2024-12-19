/**
 * Copyright 2023 CARIAD SE.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */


 //start_include_snippet_component_plugin_analyzer
#include <fep3/base/component_plugin_analyzer/component_plugin_analyzer.h>
//end_include_snippet_component_plugin_analyzer
#include <fep3/components/job_registry/job_registry_intf.h>

const std::string native_components_rel_path = "lib";
const std::string rti_components_rel_path = "lib/rti";

int main()
{
    //start_construct_snippet_component_plugin_analyzer
    fep3::base::ComponentPluginAnalyzer analyzer(
        { {"logging_service.arya.fep3.iid", NATIVE_COMPONENTS_PATH, native_components_rel_path},
         {"configuration_service.arya.fep3.iid",
          NATIVE_COMPONENTS_PATH,
          native_components_rel_path},
         {"service_bus.catelyn.fep3.iid", DDS_SERVICE_BUS_PATH, rti_components_rel_path},
         {"service_bus.arya.fep3.iid", DDS_SERVICE_BUS_PATH, rti_components_rel_path},
         {"participant_info.arya.fep3.iid", NATIVE_COMPONENTS_PATH, native_components_rel_path},
         {"clock_service.arya.fep3.iid", NATIVE_COMPONENTS_PATH, native_components_rel_path},
         {"clock_sync_service.arya.fep3.iid", NATIVE_COMPONENTS_PATH, native_components_rel_path},
         {"data_registry.arya.fep3.iid", NATIVE_COMPONENTS_PATH, native_components_rel_path},
         {"job_registry.catelyn.fep3.iid", NATIVE_COMPONENTS_PATH, native_components_rel_path},
         {"scheduler_service.catelyn.fep3.iid", NATIVE_COMPONENTS_PATH, native_components_rel_path},
         {"health_service.catelyn.fep3.iid", NATIVE_COMPONENTS_PATH, native_components_rel_path},
         {"simulation_bus.arya.fep3.iid", DDS_SIM_BUS_PATH, rti_components_rel_path} });

    //end_construct_snippet_component_plugin_analyzer

    //start_analyze_state_snippet_component_plugin_analyzer
    analyzer.analyzeStateTransition(
        [](std::shared_ptr<fep3::IComponents> components, fep3::arya::IComponent& comp) {
            return comp.createComponent(components);
        },
        "UNLOADED");
    //end_analyze_state_snippet_component_plugin_analyzer

    //start_analyze_comp_function_snippet_component_plugin_analyzer
    // analyze a component function
    analyzer.analyzeComponentFunction(
        std::function([](fep3::IJobRegistry& job_registry) { job_registry.getJobs(); }));
    //end_analyze_comp_function_snippet_component_plugin_analyzer

    analyzer.analyzeStateTransition([](std::shared_ptr<fep3::IComponents>,
        fep3::arya::IComponent& comp) { return comp.initialize(); },
        "INITIALIZED");
    analyzer.analyzeStateTransition([](std::shared_ptr<fep3::IComponents>,
        fep3::arya::IComponent& comp) { return comp.tense(); },
        "INITIALIZED");
    analyzer.analyzeStateTransition([](std::shared_ptr<fep3::IComponents>,
        fep3::arya::IComponent& comp) { return comp.start(); },
        "RUNNING");
    analyzer.analyzeStateTransition([](std::shared_ptr<fep3::IComponents>,
        fep3::arya::IComponent& comp) { return comp.stop(); },
        "INITIALIZED");
    analyzer.analyzeStateTransition([](std::shared_ptr<fep3::IComponents>,
        fep3::arya::IComponent& comp) { return comp.relax(); },
        "UNLOADED");
    analyzer.analyzeStateTransition(
        [](std::shared_ptr<fep3::IComponents>, fep3::arya::IComponent& comp) {
            return comp.deinitialize();
        },
        "UNLOADED");

    analyzer.analyzeStateTransition(
        [](std::shared_ptr<fep3::IComponents>, fep3::arya::IComponent& comp) {
            return comp.destroyComponent();
        },
        "UNLOADED");

    //start_print_snippet_component_plugin_analyzer
    auto model_info = analyzer.getData();
    analyzer.print(model_info);
    //end_print_snippet_component_plugin_analyzer
    return 0;
}
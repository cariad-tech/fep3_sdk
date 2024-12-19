/**
 * Copyright 2023 CARIAD SE.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <iostream>
#include <string>

#include <fep3/core.h>
#include <fep3/core/element_configurable.h>
#include "component_a_intf.h"
#include "component_b_intf.h"

using namespace fep3;
using namespace std::chrono_literals;

class PluginElement : public fep3::core::ElementConfigurable
{
public:
    // Implementation of the CTOR!
    // ElementConfigurable has no default CTOR
    //  you must define a type of your element -> to identify your implementation in a system
    //  you must define a implementation version -> to identify your implementation version in a system
    //  KEEP in MIND THIS IS NOT THE ELEMENT INSTANCE NAME!
    PluginElement()
        : fep3::core::ElementConfigurable("Easy Element",
                                          FEP3_PARTICIPANT_LIBRARY_VERSION_STR)
    {
    }

    fep3::Result load() override
    {
        IComponentA *component_a_interface =
            fep3::getComponent<IComponentA>(*getComponents());

        IComponentB *component_b_interface =
            fep3::getComponent<IComponentB>(*getComponents());

        component_a_interface->set(5);
        std::cout << "Identifier: " << component_a_interface->getIdentifier() << std::endl;
        std::cout << "value: " << component_a_interface->get() << std::endl;

        component_b_interface->set(10);
        std::cout << "Identifier: " << component_b_interface->getIdentifier() << std::endl;
        std::cout << "value: " << component_b_interface->get() << std::endl;

        return {};
    }
};

int main(int argc, const char *argv[])
{
    try
    {
        auto part = core::createParticipant<core::ElementFactory<PluginElement>>(
            argc, argv,
            "My Demo Component Plugin Participant Version 1.0",
            {"demo_component_plugin_participant", "demo_component_plugin_system", ""});
        return part.exec();
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what();
        return 3;
    }
}

/**
 * Copyright 2023 CARIAD SE.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <iostream>

#include <fep3/plugin/cpp/cpp_plugin_impl_arya.hpp>
#include <fep3/plugin/cpp/cpp_plugin_component_factory.h>
#include <fep3/core.h>
#include <fep3/core/element_configurable.h>
#include <fep3/components/base/component.h>

using namespace fep3;
using namespace std::chrono_literals;

//Begin(componentA interface)
class IComponentA
{
protected:
    virtual ~IComponentA() = default;

public:
    FEP_COMPONENT_IID("IComponentA");

public:
    virtual std::string getIdentifier() const = 0;
    virtual int32_t get() = 0;
    virtual void set(int32_t value) = 0;
};
//End(componentA interface)

//Begin(componentB interface)
class IComponentB
{
protected:
    virtual ~IComponentB() = default;

public:
    FEP_COMPONENT_IID("IComponentB");

public:
    virtual std::string getIdentifier() const = 0;
    virtual int32_t get() = 0;
    virtual void set(int32_t value) = 0;
};
//End(componentB interface)

//Begin(componentA)
class ComponentA : public fep3::base::Component<IComponentA>
{
    int32_t _value;

public:
    ComponentA() = default;
    ~ComponentA() = default;

    std::string getIdentifier() const override
    {
        return "components_cpp_plugin:component_a";
    }
    int32_t get() override
    {
        return _value;
    }
    void set(int32_t value)
    {
        _value = value;
    }
};
//End(componentA)

//Begin(componentB)
class ComponentB : public fep3::base::Component<IComponentB>
{
    int32_t _value;

public:
    ComponentB() = default;
    ~ComponentB() = default;

    std::string getIdentifier() const override
    {
        return "components_cpp_plugin:component_b";
    }
    int32_t get() override
    {
        return _value;
    }
    void set(int32_t value) override
    {
        _value = value;
    }
};
//End(componentB)

//Begin(my plugin functions)
void fep3_plugin_getPluginVersion(void (*callback)(void *, const char *), void *destination)
{
    callback(destination, "0.0.1");
}

fep3::plugin::cpp::catelyn::IComponentFactory *fep3_plugin_cpp_catelyn_getFactory()
{
    return new fep3::plugin::cpp::catelyn::ComponentFactory<ComponentA, ComponentB>();
}
//End(my plugin functions)

class PluginElement : public fep3::core::ElementConfigurable
{
public:
    PluginElement()
        : fep3::core::ElementConfigurable("Easy Element",
                                          FEP3_PARTICIPANT_LIBRARY_VERSION_STR)
    {
    }

    //Begin(my plugin get Component)
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
    //End(my plugin get Component)
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
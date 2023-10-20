/**
 * @file
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#include <fep3/plugin/cpp/cpp_plugin_impl_arya.hpp>
#include <fep3/plugin/cpp/cpp_plugin_component_factory.h>
#include <fep3/components/base/component.h>
#include "component_a_intf.h"
#include "component_b_intf.h"

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

void fep3_plugin_getPluginVersion(void (*callback)(void *, const char *), void *destination)
{
    callback(destination, "0.0.1");
}

fep3::plugin::cpp::catelyn::IComponentFactory *fep3_plugin_cpp_catelyn_getFactory()
{
    return new fep3::plugin::cpp::catelyn::ComponentFactory<ComponentA, ComponentB>();
}

/**
 * @file
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

    This Source Code Form is subject to the terms of the Mozilla
    Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

If it is not possible or desirable to put the notice in a particular file, then
You may include the notice in a location (such as a LICENSE file in a
relevant directory) where a recipient would be likely to look for such a notice.

You may add additional accurate notices of copyright ownership.

@endverbatim
 */
#include <fep3/core/participant.h>
#include <fep3/core/element_factory.h>
#include <fep3/core/element_base.h>

#include <fep3/components/service_bus/service_bus_intf.h>
#include <fep3/components/configuration/configuration_service_intf.h>
#include <fep3/base/properties/propertynode.h>
#include <fep3/base/properties/properties.h>

#include <string>
#include <iostream>

namespace {
class EmptyElement : public fep3::core::ElementBase
{
public:
    EmptyElement() : fep3::core::ElementBase("EmptyElement", "1.0.0")
    {
    }
};

} // namespace

int main(int, const char**)
{
    //creating a participant with an empty element
    // in this case we FORCE the Url of the participant server to use!!!
    auto participant = fep3::core::createParticipant<fep3::core::ElementFactory<EmptyElement>>(
                                "demo",
                                "1.0.0",
                                std::string(),
                                "http://localhost:10025");

    //retrieve the configuration service component
    auto configuration_service = participant.getComponent<fep3::IConfigurationService>();
    if (configuration_service)
    {

        //Begin(Create main property)
        auto vehicle = std::make_shared<fep3::base::NativePropertyNode>("Vehicle");
        //End(Create main property)

        //Begin(Create subproperties)
        auto vehicle_size = std::make_shared<fep3::base::NativePropertyNode>("Size");
        vehicle->setChild(vehicle_size);
        auto vehicle_size_length = fep3::base::makeNativePropertyNode<double>("Length", 4.5);
        vehicle_size->setChild(vehicle_size_length);
        auto vehicle_size_width = fep3::base::makeNativePropertyNode<double>("Width", 2.1);
        vehicle_size->setChild(vehicle_size_width);
        auto vehicle_type = fep3::base::makeNativePropertyNode<std::string>("Type", "car");
        vehicle->setChild(vehicle_type);
        auto vehicle_seats = fep3::base::makeNativePropertyNode<int32_t>("Seats", 5);
        vehicle->setChild(vehicle_seats);
        auto vehicle_available = fep3::base::makeNativePropertyNode<bool>("Available", true);
        vehicle->setChild(vehicle_available);
        //End(Create subproperties)

        //Begin(Register main node)
        configuration_service->registerNode(vehicle);
        //End(Register main node)

        //Begin(Set property value by service)
        auto result = fep3::base::setPropertyValue<int32_t>(*configuration_service, "Vehicle/Seats", 3);
        std::cout << "Result: " << result.getErrorCode() << '\n';
        //End(Set property value by service)

        //Begin(Set property value by node)
        result = fep3::base::setPropertyValue<int32_t>(*vehicle_seats, 3);
        std::cout << "Result: " << result.getErrorCode() << '\n';
        //End(Set property value by node)

        //Begin(Reading property by service)
        auto seats = fep3::base::getPropertyValue<int32_t>(*configuration_service, "Vehicle/Seats");
        if (seats.has_value())
        {
            std::cout << "Seats: " << seats.value() << '\n';
        }
        // Alternative syntax
        if (seats)
        {
            std::cout << "Seats: " << *seats << '\n';
        }
        //End(Reading property by service)

        //Begin(Reading property by node)
        auto value = fep3::base::getPropertyValue<int32_t>(*vehicle_seats);
        //End(Reading property by node)

        //Begin(get not existing property)
        auto invalid = fep3::base::getPropertyValue<int32_t>(*configuration_service, "Vehicle/Invalid");
        if (!invalid.has_value())
        {
            std::cout << "Property does not exist" << '\n';
        }
        // Alternative syntax
        if (!invalid)
        {
            std::cout << "Property does not exist" << '\n';
        }
        //End(get not existing property)

        //Begin(retrieve property node)
        auto vehicle_node = configuration_service->getNode("Vehicle");
        auto type_node = vehicle_node->getChild("Type");
        std::cout << type_node->getValue() << '\n';
        std::cout << type_node->getTypeName() << '\n';
        //End(retrieve property node)

        //Begin(retrieve const property node)
        auto vehicle_node_const = configuration_service->getConstNode("Vehicle");
        std::cout << vehicle_node_const->getTypeName() << '\n';
        //End(retrieve const property node)

        //Begin(changing value)
        fep3::base::setPropertyValue<double>(*configuration_service, "Vehicle/Size/Length", 4.6);
        //End(changing value)

        //Begin(changing type)
        result = fep3::base::setPropertyValue<int32_t>(*configuration_service, "Vehicle/Size/Length", 5);
        std::cout << result.getErrorLabel() << '\n';
        //End(changing type)

        //Begin(register property variable)
        fep3::base::PropertyVariable<int32_t> property_variable{0};
        vehicle_seats->registerVariable(property_variable);
        //End(register property variable)

        //Begin(read property variable)
        vehicle_seats->updateObservers();
        std::cout << property_variable << '\n';
        //End(read property variable)

        //Begin(create property observer)
        struct PropertyObserver : public fep3::base::IPropertyObserver
        {
            void onUpdate(fep3::IPropertyNode& property_node) override
            {
                std::cout << "Update of property: " << property_node.getName() << '\n';
            }
        };
        //End(create property observer)

        //Begin(register property observer)
        auto property_observer = std::make_shared<PropertyObserver>();
        vehicle_seats->registerObserver(property_observer);
        //End(register property observer)

        //Begin(trigger property observer)
        vehicle_seats->updateObservers();
        //End(trigger property observer)

        std::cout << std::flush;
    }
    else
    {
        std::cout << "Error: no configuration service found!";
        return -1;
    }
    //wait until exit is send
    return participant.exec();
}


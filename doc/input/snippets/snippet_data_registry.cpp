/**
 * Copyright 2023 CARIAD SE.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */



#include <fep3/core/participant.h>
#include <fep3/core/element_factory.h>
#include <fep3/core/element_base.h>
#include <fep3/base/stream_type/default_stream_type.h>
#include <fep3/base/data_registry/data_registry.h>
#include <fep3/components/data_registry/data_registry_intf.h>

namespace {
class EmptyElement : public fep3::core::ElementBase
{
public:
    EmptyElement() : fep3::core::ElementBase("EmptyElement", "1.0.0")
    {
    }

    fep3::Result load() override
    {
        data_registry = getComponents()->getComponent<fep3::IDataRegistry>();
        return{};
    };

    //Begin(register data signal)
    fep3::Result initialize() override
    {
        fep3::Result res = data_registry->registerDataIn("my_signal_in", fep3::base::StreamTypeDDL("",""));
        if (!res) {
            return res;
        }

        res = data_registry->registerDataOut("my_signal_out", fep3::base::StreamTypeDDL("",""));
        if (!res) {
            return res;
        }
        return res;
    }
    //End(register data signal)

private:
    fep3::IDataRegistry* data_registry;
};
} // namespace

int main(int, const char**)
{
    // creating a participant with an empty element
    // in this case we FORCE the Url of the participant server to use!!!
    auto participant = fep3::core::createParticipant<fep3::core::ElementFactory<EmptyElement>>(
        "demo",
        "1.0.0",
        "demo_system",
        "http://localhost:10025");

    // Retrieve the data registry component
    auto data_registry = participant.getComponent<fep3::IDataRegistry>();
    if (data_registry)
    {
        {
            //Begin(get data reader)
            data_registry->registerDataIn("my_signal_in", fep3::base::StreamTypeDDL("",""));
            auto data_reader = data_registry->getReader("my_signal_in");
            //End(get data reader)
        }
        {            
            //Begin(add data in)
            std::unique_ptr<fep3::IDataRegistry::IDataReader> data_reader;
            fep3::base::addDataIn(*data_registry, data_reader, "my_signal_in", fep3::base::StreamTypeDDL("",""));
            //End(add data in)
        }
    }
}

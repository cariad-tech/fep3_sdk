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
#include <fep3/base/stream_type/default_stream_type.h>

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
        if (fep3::isFailed(res)) {
            return res;
        }

        res = data_registry->registerDataOut("my_signal_out", fep3::base::StreamTypeDDL("",""));
        if (fep3::isFailed(res)) {
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
            auto data_reader = addDataIn(*data_registry, "my_signal_in", fep3::base::StreamTypeDDL("",""));
            //End(add data in)
        }
    }
}
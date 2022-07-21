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
#include <fep3/cpp.h>
#include <chrono>
using namespace fep3;
using namespace std::chrono_literals;

class MinimalDataJob : public cpp::DataJob {
public:
    MinimalDataJob() : cpp::DataJob("multiply", 1s) {
        _reader = addDataIn("small_value",  base::StreamTypePlain<int32_t>());
        _writer = addDataOut("big_value", base::StreamTypePlain<int32_t>());
        registerPropertyVariable(_factor, "multiplication_factor");
    }

    Result process(Timestamp sim_time_of_execution) {
        Optional<int32_t> received_plain_value;
        *_reader >> received_plain_value;
        if (received_plain_value.has_value())
        {
            auto value = received_plain_value.value() * _factor;
            *_writer << value;
        }
        return {};
    }

    cpp::PropertyVariable<int32_t> _factor{ 2 };
    cpp::DataReader* _reader;
    cpp::DataWriter* _writer;
};

int main(int argn, const char* argv[]) {
    auto part = cpp::createParticipant<cpp::DataJobElement<MinimalDataJob>>(
            argn, argv, "my_participant", "my_system");
    return part.exec();
}

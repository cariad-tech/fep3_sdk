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


#include <fep3/core.h>

#include <condition_variable>
#include <mutex>

class NotificationDataReceiver : public fep3::IDataRegistry::IDataReceiver {
public:
    NotificationDataReceiver(std::condition_variable& condition_data_received, std::mutex& mutex_data_received, std::atomic<bool>& data_received, fep3::data_read_ptr<const fep3::IDataSample>& data_sample)
        : _condition_data_received(condition_data_received), _mutex_data_received(mutex_data_received), _data_received(data_received), _data_sample(data_sample) {}

    void operator()(const fep3::data_read_ptr<const fep3::IStreamType>&) override {
    }

    void operator()(const fep3::data_read_ptr<const fep3::IDataSample>& data_sample) override {
        // Add scope to release lock as soon as possible
        {
            std::lock_guard<std::mutex> lock(_mutex_data_received);
            _data_sample = data_sample;
            _data_received = true;
        }
        _condition_data_received.notify_one();
    }

private:
    std::condition_variable& _condition_data_received;
    std::mutex& _mutex_data_received;
    std::atomic<bool>& _data_received;
    fep3::data_read_ptr<const fep3::IDataSample>& _data_sample;
};
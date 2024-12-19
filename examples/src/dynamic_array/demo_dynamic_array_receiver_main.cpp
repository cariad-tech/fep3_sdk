/**
 * Copyright 2023 CARIAD SE.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <iostream>
#include <memory>
#include <string>
#include <chrono>

#include "dynamic_array_data_sample.h"
#include "stream_type_file_ref_big.h"
#include <fep3/cpp.h>

#include <ddl/dd/ddstructure.h>

using namespace fep3;
using namespace std::chrono_literals;


class EasyReceiverJob : public cpp::DataJob
{
public:
    using ExecuteCall = std::function<fep3::Result(fep3::Timestamp)>;

    EasyReceiverJob() : cpp::DataJob("receive_job", 100ms)
        , _data(_lidar_data)
    {
        //Begin(Receiver initialize custom stream type)
        _data_reader_ddl = addDataIn("ddl_data",
            StreamTypeDDLFileRefBig("Vef_SensorView_LidarData", "./demo_data/demo_dynamic_array.description", DynamicArrayDataSample::maxDataSize()));
        //End(Receiver initialize custom stream type)
    }

    fep3::Result process(fep3::Timestamp sim_time_of_execution)
    {
        // Pop all samples from queue _data_reader_ddl
        // Normally you pop a specific amount of samples you expect to arrive during at timepoint sim_time_of_execution
        while (hasDataToProcess(sim_time_of_execution))
        {
            fep3::data_read_ptr<const fep3::arya::IDataSample> ptr = _data_reader_ddl->popSampleOldest();
            if (ptr)
            {
                //Begin(Deserialize the structure)
                auto bytes_received = ptr->read(_data);
                //End(Deserialize the structure)
                FEP3_LOG_INFO("received " + std::to_string(bytes_received) + " bytes");
                FEP3_LOG_INFO("received " + std::to_string(_lidar_data.number_of_points) + " points");
                FEP3_LOG_INFO("scan number: " + std::to_string(_lidar_data.header.scan_number));
                FEP3_LOG_INFO("point 100 length: " + std::to_string(_lidar_data.lidar_points[99].range));
                FEP3_LOG_INFO("last point length: " + std::to_string(_lidar_data.lidar_points[_lidar_data.number_of_points - 1].range));
            }
        }

        return {};
    }

private:
    bool hasDataToProcess(fep3::Timestamp sim_time_of_execution)
    {
        auto time = _data_reader_ddl->getFrontTime();
        if (time)
        {
            return time.value() <= sim_time_of_execution;
        }
        else
        {
            return false;
        }
    }

    std::shared_ptr<EasyReceiverJob> _my_job;
    core::DataReader* _data_reader_ddl;
    LidarDataWithMaxSize _lidar_data;
    DynamicArrayDataSample _data;
};

int main(int argc, const char* argv[])
{
    try
    {
        auto part = cpp::createParticipant<cpp::DataJobElement<EasyReceiverJob>>(
            argc, argv,
            "demo_dynamic_array_receiver",
            "demo_system");
        return part.exec();
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what();
        return 3;
    }
}

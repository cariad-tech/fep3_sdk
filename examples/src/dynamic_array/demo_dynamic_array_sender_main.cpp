/**
 * @file
 * @copyright
 * @verbatim
Copyright @ 2023 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#include <iostream>
#include <string>
#include <chrono>

#include "stream_type_file_ref_big.h"
#include "dynamic_array_data_sample.h"
#include <fep3/cpp.h>
#include <ddl/dd/ddstructure.h>

using namespace fep3;
using namespace std::chrono_literals;

class EasySenderJob : public cpp::DataJob
{
public:
    using ExecuteCall = std::function<fep3::Result(fep3::Timestamp)>;

    EasySenderJob()
        : cpp::DataJob("sender_job", 100ms)
    {
        //Begin(Sender initialize custom stream type)
        _data_writer_ddl = addDataOut("ddl_data",
            StreamTypeDDLFileRefBig("Vef_SensorView_LidarData", "./demo_data/demo_dynamic_array.description", DynamicArrayDataSample::maxDataSize()));
        //End(Sender initialize custom stream type)
    }

    fep3::Result process(fep3::Timestamp sim_time_of_execution) override
    {
        static uint32_t scan_number = 0;

        //write the data to the DDL signal
        _lidar_data.header.scan_number = scan_number++;
        _lidar_data.number_of_points = num_of_points_to_send;
        _lidar_data.lidar_header.number_of_rays_horizontal = 100;
        for (uint32_t i = 0; i < num_of_points_to_send; ++i)
        {
            _lidar_data.lidar_points[i].range = 1.0 * i;
        }
        //Begin(Serialize the structure)
        if (_data_writer_ddl->write(DynamicArrayDataSample(_lidar_data)))
        //End(Serialize the structure)
        {
            FEP3_LOG_INFO("Data sent, " + std::to_string(num_of_points_to_send) + " points");
        }
        else
        {
            FEP3_LOG_INFO("Data not sent");
        }
        num_of_points_to_send += 10;
        return {};
    }

private:
    //in core API you need to deal with everything by yourself
//have a look at the fep3::cpp::DataJob in cpp API
    std::shared_ptr<EasySenderJob> _my_job;
    core::DataWriter* _data_writer_ddl;
    LidarDataWithMaxSize _lidar_data;
    uint32_t num_of_points_to_send = 100;
};

int main(int argc, const char* argv[])
{
    try
    {
        auto part = cpp::createParticipant<cpp::DataJobElement<EasySenderJob>>(
            argc, argv,
            "demo_dynamic_array_sender",
            "demo_system");
        return part.exec();
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what();
        return 3;
    }
}

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

#include <fep3/base/sample/raw_memory_intf.h>
#include <fep3/base/sample/data_sample.h>
#include "demo_dynamic_array.h"

#include <cassert>

inline const size_t max_array_point_size = 3000000;

#if defined(_MSC_VER)

#pragma pack(push,1)
//Begin(Definition of structure with a maximum array size)
__declspec(align(1)) struct LidarDataWithMaxSize
{
    __declspec(align(1)) ::Header header;
    __declspec(align(1)) ::LidarHeader lidar_header;
    __declspec(align(1)) uint32_t number_of_points;
    __declspec(align(1)) ::PolarCoordinate lidar_points[max_array_point_size];
};
//End(Definition of structure with a maximum array size)

#pragma pack(pop)

#elif defined(__GNUC__) || defined(__clang__)

struct LidarDataWithMaxSize
{
    ::Header header __attribute__((aligned(1)));
    ::LidarHeader lidar_header __attribute__((aligned(1)));
    uint32_t number_of_points __attribute__((aligned(1)));
    ::PolarCoordinate lidar_points[max_array_point_size] __attribute__((aligned(1)));
} __attribute__((packed, aligned(1)));

#else

#pragma pack(push,1)

struct LidarDataWithMaxSize
{
    ::Header header;
    ::LidarHeader lidar_header;
    uint32_t number_of_points;
    ::PolarCoordinate lidar_points[max_array_point_size];
};

#pragma pack(pop)

#endif

class DynamicArrayDataSample : public fep3::base::arya::DataSampleBase, public fep3::arya::IRawMemory
{
public:
    DynamicArrayDataSample(const LidarDataWithMaxSize& data_ref) :_lidar_data(data_ref)
    {
    }
    //Begin(Writing to memory)
    size_t write(const fep3::arya::IRawMemory& readable_memory) override
    {
        assert(readable_memory.size() <= sizeof(LidarDataWithMaxSize));
        std::copy((const char*)readable_memory.cdata(),
            std::next((const char*)readable_memory.cdata(),
                readable_memory.size()), (char*)&_lidar_data);
        return readable_memory.size();
    }

    size_t read(fep3::arya::IRawMemory& writeable_memory) const override
    {
        auto bytes_written = writeable_memory.set(&_lidar_data, getSize());
        assert(bytes_written == getSize());
        return bytes_written;
    }
    //End(Writing to memory)

    //Begin(Calculating the size of the structure)
    size_t getSize() const override
    {
        return static_size + point_size * _lidar_data.number_of_points;
    }
    //End(Calculating the size of the structure)
    size_t capacity() const override
    {
        return maxDataSize();
    }

    const void* cdata() const  override
    {
        return &_lidar_data;
    }

    size_t size() const  override
    {
        return getSize();
    }

    size_t set(const void* data, size_t data_size)  override
    {
        assert(data_size <= sizeof(LidarDataWithMaxSize));
        std::copy((const char*)data,
            std::next((const char*)data,
                data_size), (char*)&_lidar_data);
        return data_size;
    }

    size_t resize(size_t data_size)  override
    {
        assert(data_size <= sizeof(LidarDataWithMaxSize));
        return data_size;
    }

    static size_t maxDataSize()
    {
        return sizeof(LidarDataWithMaxSize);
    }
private:
    const LidarDataWithMaxSize& _lidar_data;
    static const size_t  static_size = sizeof(Header) +
        sizeof(LidarHeader) +
        sizeof(decltype(LidarDataWithMaxSize::number_of_points));

    const size_t point_size = sizeof(PolarCoordinate);
};

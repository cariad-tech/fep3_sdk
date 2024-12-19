/**
 * @copyright
 * @verbatim
 * Copyright 2023 CARIAD SE.
 * 
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 * 
 * @endverbatim
 */
// this file is generated, do not edit
#pragma once


#if defined(_MSC_VER)

#pragma pack(push,1)

__declspec(align(1)) struct PolarCoordinate
{
    __declspec(align(1)) double range;
    __declspec(align(1)) double elevation;
    __declspec(align(1)) double azimuth;
};

#pragma pack(pop)

#elif defined(__GNUC__) || defined(__clang__)

struct PolarCoordinate
{
    double range __attribute__((aligned(1)));
    double elevation __attribute__((aligned(1)));
    double azimuth __attribute__((aligned(1)));
} __attribute__((packed, aligned(1)));

#else

#pragma pack(push,1)

struct PolarCoordinate
{
    double range;
    double elevation;
    double azimuth;
};

#pragma pack(pop)

#endif



#if defined(_MSC_VER)

#pragma pack(push,1)

__declspec(align(1)) struct LidarHeader
{
    __declspec(align(1)) uint32_t number_of_rays_horizontal;
    __declspec(align(1)) uint32_t number_of_rays_vertical;
};

#pragma pack(pop)

#elif defined(__GNUC__) || defined(__clang__)

struct LidarHeader
{
    uint32_t number_of_rays_horizontal __attribute__((aligned(1)));
    uint32_t number_of_rays_vertical __attribute__((aligned(1)));
} __attribute__((packed, aligned(1)));

#else

#pragma pack(push,1)

struct LidarHeader
{
    uint32_t number_of_rays_horizontal;
    uint32_t number_of_rays_vertical;
};

#pragma pack(pop)

#endif



#if defined(_MSC_VER)

#pragma pack(push,1)

__declspec(align(1)) struct Header
{
    __declspec(align(1)) uint8_t sensor_id;
    __declspec(align(1)) uint32_t scan_number;
    __declspec(align(1)) uint32_t element;
};

#pragma pack(pop)

#elif defined(__GNUC__) || defined(__clang__)

struct Header
{
    uint8_t sensor_id __attribute__((aligned(1)));
    uint32_t scan_number __attribute__((aligned(1)));
    uint32_t element __attribute__((aligned(1)));
} __attribute__((packed, aligned(1)));

#else

#pragma pack(push,1)

struct Header
{
    uint8_t sensor_id;
    uint32_t scan_number;
    uint32_t element;
};

#pragma pack(pop)

#endif



#if defined(_MSC_VER)

#pragma pack(push,1)

//Begin(ddl definition of a structure containing a dynamic array)
__declspec(align(1)) struct LidarData
{
    __declspec(align(1)) ::Header header;
    __declspec(align(1)) ::LidarHeader lidar_header;
    __declspec(align(1)) uint32_t number_of_points;
    __declspec(align(1)) ::PolarCoordinate lidar_points[];
};
//End(ddl definition of a structure containing a dynamic array)

#pragma pack(pop)

#elif defined(__GNUC__) || defined(__clang__)

struct LidarData
{
    ::Header header __attribute__((aligned(1)));
    ::LidarHeader lidar_header __attribute__((aligned(1)));
    uint32_t number_of_points __attribute__((aligned(1)));
    ::PolarCoordinate lidar_points[] __attribute__((aligned(1)));
} __attribute__((packed, aligned(1)));

#else

#pragma pack(push,1)

struct LidarData
{
    ::Header header;
    ::LidarHeader lidar_header;
    uint32_t number_of_points;
    ::PolarCoordinate lidar_points[];
};

#pragma pack(pop)

#endif


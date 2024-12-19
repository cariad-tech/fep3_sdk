/**
 * Copyright 2023 CARIAD SE.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <cstdint>
#include <string>

namespace fep3
{
namespace examples
{

// Structs to use for mapping.
// Members should be sorted by size if applicable to avoid unnecessary use of padding
// and therefore memory usage.
struct TestStructA
{
    uint16_t first;
    int8_t second;
};

struct TestStructB
{
    uint64_t first;
    uint32_t second;
};

struct TestStructC
{
    uint64_t first;
    int64_t second;
    uint32_t third;
    uint16_t fourth;
    int8_t fifth;
};

/** @} */
const std::string examples_ddl_struct = "tEasyStruct";

} // examples
} // fep3

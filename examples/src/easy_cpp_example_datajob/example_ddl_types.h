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
    
/// Position Sample Struct
struct tEasyPosition
{
    uint32_t x_pos; //! X-position
    uint32_t y_pos; //! Y-position
    uint32_t z_pos; //! Z-position
};
struct tEasyStruct
{
    tEasyPosition pos; //!pos value
    double double_value; //!double value
};

/** @} */
const std::string examples_ddl_struct = "tEasyStruct";

} // examples
} // fep3

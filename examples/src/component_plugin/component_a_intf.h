/**
 * @file
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#pragma once

#include <fep3/components/base/component_iid.h>
#include <string>

class IComponentA
{
protected:
    virtual ~IComponentA() = default;

public:
    FEP_COMPONENT_IID("IComponentA");

public:
    virtual std::string getIdentifier() const = 0;
    virtual int32_t get() = 0;
    virtual void set(int32_t value) = 0;
};

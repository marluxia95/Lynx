#ifndef LYNX_ECS_COMMON_H
#define LYNX_ECS_COMMON_H

#include <stdint.h>
#include <bitset>
#include "lynx_common.h"

namespace Lynx {

    using Signature = std::bitset<256>;
    using EntityID = uint64_t;

}

#endif // LYNX_ECS_COMMON_H
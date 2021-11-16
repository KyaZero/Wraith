file(WRITE "src/Version.h"
"// ============================================
// THIS CODE IS GENERATED - DO NOT EDIT BY HAND
// ============================================

#include <cstdint>
#include <string>

namespace ${PROJECT_NAME}
{
    constexpr uint8_t VERSION_MAJOR = ${Wraith_VERSION_MAJOR};
    constexpr uint8_t VERSION_MINOR = ${Wraith_VERSION_MINOR};
    constexpr uint8_t VERSION_PATCH = ${Wraith_VERSION_PATCH};
    constexpr uint32_t VERSION = VERSION_MAJOR | (VERSION_MINOR << 8) | (VERSION_PATCH << 16);
    constexpr const char* VERSION_TEXT = \"${Wraith_VERSION}\";
    constexpr const char* ENGINE_NAME = \"${PROJECT_NAME}\";
    constexpr const char* PROJECT_NAME = \"${CMAKE_PROJECT_NAME}\";
}  // namespace ${PROJECT_NAME}
")

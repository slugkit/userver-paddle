#pragma once

#include <cstdint>
#include <string_view>

namespace paddle {

/// @brief Verify the signature of the payload
/// @param secret The secret key
/// @param signature The signature header
/// @param payload The payload
/// @param max_age_seconds The maximum age of the signature in seconds. -1 means no limit
/// @return True if the signature is valid, false otherwise
auto VerifySignature(
    std::string_view secret,
    std::string_view signature,
    std::string_view payload,
    std::int32_t max_age_seconds = -1  // -1 means no limit
) -> bool;

}  // namespace paddle
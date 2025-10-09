#pragma once

#include <optional>
#include <string>

namespace paddle::handlers {

enum class HandleResultStatus { kHandled, kIgnored, kError };

struct HandleResult {
    HandleResultStatus status;
    std::optional<std::string> reason;
};

}  // namespace paddle::handlers

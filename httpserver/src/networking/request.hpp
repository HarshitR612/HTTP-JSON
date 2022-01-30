#pragma once

#include "types.h"
#include <string>
#include <optional>

namespace server {

    struct HttpRequest {
        std::string path_;
        std::optional<std::string> query_;
        Method method_;
    };

}

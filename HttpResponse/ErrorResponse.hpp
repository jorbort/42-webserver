#pragma once

#include <string>
#include "ResponseHeader.hpp"

class ErrorResponse : public ResponseHeader {
    public:
        static std::string create404NotFoundResponse(void);
};
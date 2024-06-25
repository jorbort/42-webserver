#pragma once

#include <string>
#include "ResponseHeader.hpp"

class GetResponse: public ResponseHeader {
    public:
        static std::string createGETresponse(int fd);
};
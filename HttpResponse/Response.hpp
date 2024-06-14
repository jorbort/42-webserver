#pragma once

#include <string>
#include "ResponseHeader.hpp"

class Response : public ResponseHeader {
	public:
		~Response();

		std::string createResponse(void);
		//Response for each method
		std::string createGETresponse(int fd);
		//Response for errors
		std::string create404NotFoundResponse(void);
	private:
		//body for GET
		void	setBody(std::string &body, size_t &contentLength, int fd, size_t maxBodySize);
};
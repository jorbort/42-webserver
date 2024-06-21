#pragma once

#include <string>
#include "ResponseHeader.hpp"
#include "../HttpRequest/HTTPRequest.hpp"

class Response : public ResponseHeader {
	public:
		~Response();

		std::string createResponse(HttpRequest &request);
		//Response for each method
		std::string createGETresponse(int fd);
		//Response for errors
		std::string create404NotFoundResponse(void);
	private:
		//body for GET
		void	setBody(std::string &body, size_t &contentLength, int fd, size_t maxBodySize);
		void	run_execve(const char *uri);
		bool	isDirectory(const char *path);
		bool	isCGI(const char *extension);
		bool	isProcessableCGI(const char *extension);
};
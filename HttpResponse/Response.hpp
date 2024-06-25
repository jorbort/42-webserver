#pragma once

#include <string>
#include "ErrorResponse.hpp"
#include "GetResponse.hpp"
#include "HTTPRequest.hpp"

class Response {
	public:
		~Response();

		std::string createResponse(HttpRequest &request);
	private:
		
		void	run_execve(const char *uri, char **envp);
		bool	isDirectory(const char *path);
		bool	isCGI(const char *extension);
		bool	isProcessableCGI(const char *extension);
};
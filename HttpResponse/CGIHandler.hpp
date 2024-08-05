#pragma once
#include <string>

class Response;

class CGIHandler {
	public:
		CGIHandler(Response &response);
		~CGIHandler();
		int	handleCGI(std::string requestBody);

		int fd;
	private:
		char *	_cgiPath;
		char **	_argv;
		char **	_envp;

		char ** initEnvironment(Response &response);
		std::string transformHeaderName(const std::string &header_name);
};
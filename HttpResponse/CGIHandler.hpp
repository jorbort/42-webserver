#pragma once

class Response;

class CGIHandler {
	public:
		CGIHandler(Response &response);
		~CGIHandler();
		int	handleCGI();

		int fd;
	private:
		char *	_cgiPath;
		char **	_argv;
		char **	_envp;

		char ** initEnvironment(Response &response);
};
#include "CGIHandler.hpp"
#include "Response.hpp"
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <signal.h>
#include <sys/wait.h>

CGIHandler::CGIHandler(Response &response) {
	//falta tomar el cgi-bin del config file
	if (strcmp(response.extension, "sh") == 0)
		_cgiPath = strdup("/bin/bash");
	else if (strcmp(response.extension, "py") == 0)
		_cgiPath = strdup("/usr/bin/python3");
	else {
		perror("execve");
		exit(1);
	}
	_argv = (char **)malloc(sizeof(char *) * 3);
	_argv[0] = strdup(_cgiPath);
	_argv[1] = strdup(response.uri);
	_argv[2] = NULL;
	_envp = initEnvironment(response);//todo necesitamos implmentar cookies
	fd = -1;
}

CGIHandler::~CGIHandler() {
	int	i;

	free (_cgiPath);
	i = 0;
	while (_argv != NULL && _argv[i] != NULL)
		free (_argv[i++]);
	free (_argv);
	i = 0;
	while (_envp != NULL && _envp[i] != NULL)
		free (_envp[i++]);
	free (_envp);
	close(fd);
}

int CGIHandler::handleCGI() {

	int		pfd[2];
	pid_t	pid;

	if (pipe(pfd) == -1)
		return 500;
	if ((pid = fork()) == -1)
		return (close(pfd[0]), close(pfd[1]), 500);
	if (pid == 0) {
		close(pfd[0]);
		if (dup2(pfd[1], STDOUT_FILENO) == -1) 
			return (close(pfd[1]), 500);
		close(pfd[1]);
		execve(_cgiPath, _argv, _envp);
		perror("execve");
		exit(1);
	}
	else {
		close(pfd[1]);
		this->fd = pfd[0];
		int status;
		int wpid = waitpid(pid,&status,WNOHANG);
		if (wpid == 0){
			sleep(1);
			wpid = waitpid(pid, &status, WNOHANG);
			if (wpid == 0){
				kill(pid, SIGKILL);
				perror("waitpid");
				return(500);
			}
		}
		else if (wpid < 0 ){
			perror("waitpid");
			return (500);
		}
		if (WEXITSTATUS(status) != 0){
			perror("wexitstatus");
			return (500);
		}
		return (200);
	}
}

// Cookie header Example-> Cookie: yummy_cookie=choco; tasty_cookie=strawberry
char **	CGIHandler::initEnvironment(Response &response) {
	if (response._headers.find("Cookie") == response._headers.end()) {
		return NULL;
	}
	std::string cookieHeader = response._headers["Cookie"];
	std::vector<std::string> cookies;
	size_t pos = 0;
	size_t end;

	while ((end = cookieHeader.find(';', pos)) != std::string::npos) {
		std::string cookie = cookieHeader.substr(pos, end - pos);
		pos = end + 1;
		// Trim whitespace
		size_t start = cookie.find_first_not_of(" \t");
		size_t last = cookie.find_last_not_of(" \t");
		if (start != std::string::npos && last != std::string::npos) {
			cookie = cookie.substr(start, last - start + 1);
		}
		cookies.push_back(cookie);
	}
	// Last cookie (or only cookie if no ';')
	std::string lastCookie = cookieHeader.substr(pos);
	size_t start = lastCookie.find_first_not_of(" \t");
	size_t last = lastCookie.find_last_not_of(" \t");
	if (start != std::string::npos && last != std::string::npos) {
		lastCookie = lastCookie.substr(start, last - start + 1);
	}
	cookies.push_back(lastCookie);

	char** cookie_env = new char*[cookies.size() + 1];
	for (size_t i = 0; i < cookies.size(); ++i) {
		std::cout << cookies[i] << std::endl;
		cookie_env[i] = strdup(cookies[i].c_str());
	}
	cookie_env[cookies.size()] = NULL; // Null-terminate the array

	return cookie_env;
}

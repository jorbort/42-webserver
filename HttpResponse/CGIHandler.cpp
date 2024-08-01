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
	_envp = initEnvironment();//todo necesitamos implmentar cookies
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
			if (wpid){
				kill(pid, SIGKILL);
				return(500);
			}
		}
		else if (wpid < 0 ){
			perror("waitpid");
			return (500);
		}
		if (WEXITSTATUS(status) != 0){
			return (500);
		}
		return (200);
	}
}

char **	CGIHandler::initEnvironment() {
	//TODO
	return NULL;
}
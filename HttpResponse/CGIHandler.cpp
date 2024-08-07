#include "CGIHandler.hpp"
#include "Response.hpp"
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <signal.h>
#include <sys/wait.h>
#include <algorithm>


CGIHandler::CGIHandler(Response &response) {
	_cgiPath = selectCGIBIN(response.extension, response.server->getLocation("./cgi-bin")->cgiPath);
	if (_cgiPath == NULL){
		perror("cgipath");
		throw std::runtime_error("CGIHandler: CGI path not found");
	}
	_argv = (char **)malloc(sizeof(char *) * 3);
	_argv[0] = strdup(_cgiPath);
	_argv[1] = strdup(response.uri);
	_argv[2] = NULL;
	_envp = initEnvironment(response);
	fd = -1;
}

char *CGIHandler::selectCGIBIN(char *ext, std::vector<std::string> &cgiPaths){
	std::string extension = ext;
	std::vector<std::string> possibleCGI;
    possibleCGI.push_back("pl");
    possibleCGI.push_back("py");
    possibleCGI.push_back("sh");
    possibleCGI.push_back("exe");
    possibleCGI.push_back("php");
    possibleCGI.push_back("rb");
    possibleCGI.push_back("tcl");
	possibleCGI.push_back("js");
	size_t i = 0;
	while (possibleCGI.size() > i){
		if (extension == possibleCGI[i]){
			break;
		}
		i++;
	}
	switch (i){
		case 0:{
			std::vector<std::string>::iterator itv = std::find(cgiPaths.begin(), cgiPaths.end(), "/usr/bin/perl");
			if (itv != cgiPaths.end()){
				std::string perl = *itv;
				return strdup(perl.c_str());
			}
			else{
				return NULL;
			}	
		}
		case 1:{
			std::vector<std::string>::iterator itv = std::find(cgiPaths.begin(), cgiPaths.end(), "/usr/bin/python3");
			if (itv != cgiPaths.end()){
				std::string python = *itv;
				return strdup(python.c_str());
			}
			else{
				return NULL;
			}
		}
		case 2:{
			std::vector<std::string>::iterator itv = std::find(cgiPaths.begin(), cgiPaths.end(), "/bin/bash");
			if (itv != cgiPaths.end()){
				std::string bash = *itv;
				return strdup(bash.c_str());
			}
			else{
				return NULL;
			}
		}
		case 3:{
			std::vector<std::string>::iterator itv = std::find(cgiPaths.begin(), cgiPaths.end(), "exe");
			if (itv != cgiPaths.end()){
				std::string exe = *itv;
				return strdup(exe.c_str());
			}
			else{
				return NULL;
			}
		}
		case 4:{
			std::vector<std::string>::iterator itv = std::find(cgiPaths.begin(), cgiPaths.end(), "/usr/bin/php");
			if (itv != cgiPaths.end()){
				std::string php = *itv;
				return strdup(php.c_str());
			}
			else{
				return NULL;
			}
		}
		case 5:{
			std::vector<std::string>::iterator itv = std::find(cgiPaths.begin(), cgiPaths.end(), "/usr/bin/ruby");
			if (itv != cgiPaths.end()){
				std::string ruby = *itv;
				return strdup(ruby.c_str());
			}
			else{
				return NULL;
			}
		}
		case 6:{
			std::vector<std::string>::iterator itv = std::find(cgiPaths.begin(), cgiPaths.end(), "/usr/bin/tclsh");
			if (itv != cgiPaths.end()){
				std::string tcl = *itv;
				return strdup(tcl.c_str());
			}
			else{
				return NULL;
			}
		}
		case 7:{
			std::vector<std::string>::iterator itv = std::find(cgiPaths.begin(), cgiPaths.end(), "/usr/bin/node");
			if (itv != cgiPaths.end()){
				std::string node = *itv;
				return strdup(node.c_str());
			}
			else{
				return NULL;
			}
		}
		default:
			return NULL;
	}
	
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
	if(_envp)
		delete []_envp;
	close(fd);
}

int CGIHandler::handleCGI(std::string requestBody) {

	int		pfd[2];
	FILE	*filein = tmpfile();
	long	fdin = fileno(filein);
	int 	savedSTDIn = dup(STDIN_FILENO);
	pid_t	pid;

	int written = write(fdin, requestBody.c_str(), requestBody.size());

	if (written == -1){
		perror("write");
		fclose(filein);
		return 500;
	}
	lseek(fdin, 0, SEEK_SET);

	if (pipe(pfd) == -1)
		return 500;
	if ((pid = fork()) == -1)
		return (close(pfd[0]), close(pfd[1]),fclose(filein), 500);
	if (pid == 0) {
		close(pfd[0]);
		if (dup2(fdin, STDIN_FILENO) == -1)
			return (close(pfd[1]), close(fdin), 500);
		close(fdin);
		if (dup2(pfd[1], STDOUT_FILENO) == -1){
			return (close(pfd[1]), 500);
		} 
		close(pfd[1]);
		execve(_cgiPath, _argv, _envp);
		perror("execve");
		exit(1);
	}
	else {
		close (fdin);
		dup2(savedSTDIn, STDIN_FILENO);
		fclose(filein);
		close(savedSTDIn);
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

char **	CGIHandler::initEnvironment(Response &response) {
	// if (response._headers.find("Cookie") == response._headers.end()) {
	// 	return NULL;
	// }
	// std::string cookieHeader = response._headers["Cookie"];
	// std::vector<std::string> cookies;
	// size_t pos = 0;
	// size_t end;

	// while ((end = cookieHeader.find(';', pos)) != std::string::npos) {
	// 	std::string cookie = cookieHeader.substr(pos, end - pos);
	// 	pos = end + 1;
	// 	// Trim whitespace
	// 	size_t start = cookie.find_first_not_of(" \t");
	// 	size_t last = cookie.find_last_not_of(" \t");
	// 	if (start != std::string::npos && last != std::string::npos) {
	// 		cookie = cookie.substr(start, last - start + 1);
	// 	}
	// 	cookies.push_back(cookie);
	// }
	// // Last cookie (or only cookie if no ';')
	// std::string lastCookie = cookieHeader.substr(pos);
	// size_t start = lastCookie.find_first_not_of(" \t");
	// size_t last = lastCookie.find_last_not_of(" \t");
	// if (start != std::string::npos && last != std::string::npos) {
	// 	lastCookie = lastCookie.substr(start, last - start + 1);
	// }
	// cookies.push_back(lastCookie);

	// char** cookie_env = new char*[cookies.size() + 1];
	// for (size_t i = 0; i < cookies.size(); ++i) {
	// 	cookie_env[i] = strdup(cookies[i].c_str());
	// }
	// cookie_env[cookies.size()] = NULL; // Null-terminate the array

	// return cookie_env;
    if (response._headers.empty()) {
        return NULL;
    }

    std::vector<std::string> env_vars;
    std::map<std::string, std::string>::iterator it;
    for (it = response._headers.begin(); it != response._headers.end(); ++it) {
        std::string env_name = transformHeaderName(it->first);
        std::string env_value = env_name + "=" + it->second;
        env_vars.push_back(env_value);
    }

    char** env_array = new char*[env_vars.size() + 1];
    for (size_t i = 0; i < env_vars.size(); ++i) {
        env_array[i] = strdup(env_vars[i].c_str());
    }
    env_array[env_vars.size()] = NULL; // Null-terminate the array

    return env_array;

}

std::string CGIHandler::transformHeaderName(const std::string &header_name) {
    std::string result = "HTTP_";
    for (size_t i = 0; i < header_name.size(); ++i) {
        char c = header_name[i];
        if (c == '-') {
            result += '_';
        } else {
            result += std::toupper(c);
        }
    }
    return result;
}
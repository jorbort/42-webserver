#include "Response.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <ctime>
#include <cstring>
#include <cstdlib>

Response::~Response() {}

std::string Response::createResponse(HttpRequest &request) {
	if (request.getMethod() == "GET") {
		const char	*uri;
		const char	*extension;
		int			fd;
		std::string response;

		/*
		Check if URL is valid and it's file.
		If not then return corresponding error response.
		*/
		uri = request._URI_path.c_str();
		std::cout << strrchr(uri, '.') << std::endl;
		if (strrchr(uri, '.') == NULL) {
			//TODO create invalid uri response
			return "Invalid uri";
		} else if (isDirectory(uri)) {
			//TODO create directory uri response
			return "Is a directory";
		}

		/*
		Based on uri's extension check if uri means cgi or not.
		If uri is cgi then check if this cgi is processable by our server.
		If not then return corresponding error response.
		*/
		extension = strrchr(uri, '.') + 1;
		if (isCGI(extension)) {
			if (!isProcessableCGI(extension)) {
				//TODO create unprocessable cgi response
				return "UnprocessableCGI";
			}
			int pfd[2];
			pid_t pid;

			if (pipe(pfd) == -1) {
				//TODO Internal Server Error response
				return "Internal Server Error";
			}
			pid = fork();
			if (pid == -1) {
				//TODO Internal Server Error response
				close(pfd[0]);
				close(pfd[1]);
				return "Internal Server Error";
			} else if (pid == 0) {
				close(pfd[0]);
				if (dup2(pfd[1], STDOUT_FILENO) == -1) {
					//TODO Internal Server Error response
					close(pfd[1]);
					return "Internal Server Error";
				}
				close(pfd[1]);
				run_execve(uri, NULL);
				exit(-1);
			} else {
				close(pfd[1]);
				fd = pfd[0];
				response = GetResponse::createGETresponse(fd);
				close(fd);
				return response;
			}
		} else {
			fd = open(uri, O_RDONLY);
			/*
			Check if file exists and it has right permission.
			If not then return corresponding error response.
			*/
			if (fd < 0) {
				//TODO "create invalid uri response"
				switch (errno) {
					case ENOENT:
						std::cerr << "Error: El archivo '" << uri << "' no existe." << std::endl;
						break;
					case EACCES:
						std::cerr << "Error: No tiene permisos para acceder al archivo '" << uri << "'." << std::endl;
						break;
					default:
						std::cerr << "Error al abrir '" << uri << "': " << strerror(errno) << std::endl;
						break;
				}
				return "Error opening file";
			}
			response = GetResponse::createGETresponse(fd);
			close(fd);
			return response;
		}
	}
	else if (request.getMethod() == "POST") {
		return "POST";
	}
	else if (request.getMethod() == "DELETE") {
		return "DELETE";
	}
	else
		return "ERROR";
}

void	Response::run_execve(const char *uri, char **envp) {
	char	*cgiPath;
	char	**args;
	
	cgiPath = strdup("/bin/sh");
	args = (char **)malloc(sizeof(char *) * 3);
	args[0] = cgiPath;
	args[1] = (char *)uri;
	args[2] = NULL;
	execve(cgiPath, args, envp);
}

bool	Response::isDirectory(const char *path) {
   struct stat statbuf;
   if (stat(path, &statbuf) != 0)
       return 0;
   return S_ISDIR(statbuf.st_mode);
}

bool	Response::isCGI(const char *extension) {
	if (strcmp(extension, "html") == 0 || \
		strcmp(extension, "json") == 0 || \
		strcmp(extension, "xml") == 0) {
		return false;
	} else {
		return true;
	}
}

bool	Response::isProcessableCGI(const char *extension) {
	//TODO check extension based on config.
	if (strcmp(extension, "py") == 0 || \
		strcmp(extension, "sh") == 0)
		return true;
	else
		return false;
}
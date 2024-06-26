#include "Response.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <cstring>

Response::Response(HttpRequest &request) {
	this->method = getMethod(request._method);
	this->uri = strdup(request._URI_path.c_str());
	this->extension = getExtension(this->uri);
	this->_statusCode = 200;
	this->_isCGI = false;
	this->_CGIhandler = NULL;
	setDefaultErrorBody();
	initErrorPageMap();	
}

Response::~Response() {
	delete (this->uri);
	delete (this->extension);
	delete (this->_CGIhandler);
}

std::string	Response::createResponse() {
	if (!isURIAcceptable(this->uri))
		return errorResponse();
	if (isCGI(this->extension)) {
		if (!isProcessableCGI(this->extension))
			return errorResponse();
		if (method != GET && method != POST) {
			this->_statusCode = 405;
			return errorResponse();
		}
		this->_isCGI = true;
		this->_CGIhandler = new CGIHandler(*this);
		if ((this->_statusCode = this->_CGIhandler->handleCGI()) != 200)
			return errorResponse();
	}
	if (method == GET) {
		if (this->_isCGI)
			_body = readBody(this->_CGIhandler->fd);
		else
			_body = readBody(this->uri);
		this->_contentLength = _body.size();

		std::string	response;

		response = addStatusLine(this->_statusCode);
		response += addDateHeader();
		response += addContentTypeHeader(HTML);
		response += addContentLengthHeader(this->_contentLength);
		response += "\r\n";
		response += this->_body;
		return response;
	}
	return "Not Implemented";
}

Response::Method	Response::getMethod(std::string method) {
	if (method.compare("GET") == 0)
		return GET;
	else if (method.compare("POST") == 0)
		return POST;
	else if (method.compare("DELETE") == 0)
		return DELETE;
	else
		return UNKNOWN;
}

char *	Response::getExtension(char *uri) {
	char *tmp;

	if ((tmp = strrchr(uri, '.')) == NULL)
		return NULL;
	return (strdup(tmp + 1));
}

void	Response::setDefaultErrorBody() {
	this->_defaultErrorBody = "<html>\n";
	this->_defaultErrorBody += "\t<body>\n";
	this->_defaultErrorBody += "\t\t<h1>\n";
	this->_defaultErrorBody += "\t\t\t500 Internal Server Error\n";
	this->_defaultErrorBody += "\t\t<\\h1>\n";
	this->_defaultErrorBody += "\t<\\body>\n";
	this->_defaultErrorBody += "<\\html>";
}

void	Response::initErrorPageMap() {
	this->_errorPageMap[400] = "docs/web/error_pages/400.html";
	this->_errorPageMap[403] = "docs/web/error_pages/403.html";
	this->_errorPageMap[404] = "docs/web/error_pages/404.html";
	this->_errorPageMap[405] = "docs/web/error_pages/405.html";
	this->_errorPageMap[500] = "docs/web/error_pages/500.html";
	this->_errorPageMap[502] = "docs/web/error_pages/502.html";
}

std::string Response::getDirName(const std::string &path) {
    size_t pos = path.find_last_of("/\\");
    if (pos != std::string::npos) {
        return path.substr(0, pos);
    } else {
        return ".";
    }
}

bool	Response::isURIAcceptable(const char *uri) {
	struct stat info;
	if (stat(uri, &info) != 0) {
		if (errno == ENOENT) {
            std::string path_str(uri);
            std::string dir = getDirName(path_str);
			if (stat(dir.c_str(), &info) != 0) {
				this->_statusCode = 404;
				return false;
			} else if (method != POST) {
				this->_statusCode = 404;
				return false;
			} else {
				return true;
			}
		}
	} else if (info.st_mode & S_IFDIR) {
		//Given uri points to directory.
		if (method == POST || method == DELETE) {
			this->_statusCode = 405;
			return false;
		}
		else if (method == GET) {
			//TODO it depends to server's configuration.
			this->_statusCode = 403;
			return false;
		}
		else {
			this->_statusCode = 405;
			return false;
		}
	}
	return true;
}

bool	Response::isCGI(const char *extension) {
	if (extension == NULL)
		return false;
	if (strcmp(extension, "html") == 0 || \
		strcmp(extension, "json") == 0 || \
		strcmp(extension, "xml") == 0) {
		return false;
	} else {
		return true;
	}
}

bool	Response::isProcessableCGI(const char *extension) {
	if (extension == NULL) {
		this->_statusCode = 500;
		return false;
	}
	if (strcmp(extension, "py") == 0 || \
		strcmp(extension, "sh") == 0)
		return true;
	else {
		this->_statusCode = 502;
		return false;
	}
}

std::string	Response::errorResponse() {
	this->_body = readBody(getErrorPage().c_str());
	this->_contentLength = _body.size();

	std::string	response;

	response = addStatusLine(this->_statusCode);
	response += addDateHeader();
	response += addContentTypeHeader(HTML);
	response += addContentLengthHeader(this->_contentLength);
	response += "\r\n";
	response += this->_body;
	return response;
}

std::string Response::getErrorPage() {
	return this->_errorPageMap[this->_statusCode];
}

std::string Response::readBody(const char *path) {
	int	fd;

	if ((fd = open(path, O_RDONLY)) < 0) {
		switch (errno) {
			case ENOENT:
				this->_statusCode = 404;
				break;
			case EACCES:
				this->_statusCode = 403;
				break;
			default:
				this->_statusCode = 500;
		}
		if ((fd = open(getErrorPage().c_str(), O_RDONLY)) < 0)
			return this->_defaultErrorBody;
	}
	return this->readBody(fd);
}

std::string	Response::readBody(const int fd) {
	std::string body;
	char		buffer[1024];
	ssize_t		bytesRead;

	while ((bytesRead = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
		buffer[bytesRead] = '\0';
		body.append(buffer);
		buffer[0] = '\0';
	}
	return body;
}